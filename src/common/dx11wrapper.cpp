// dx11wrapper.h
// Author: abramann

#include "dx11wrapper.h"
#include "game.h"
#include "imgui\imgui_impl_dx11.h"
#include "shader.h"
#include "fileio.h"
#include <D3DX11.h>

using namespace Microsoft::WRL;

Dx11Wrapper::Dx11Wrapper() : m_initialzed(false)
{
}

Dx11Wrapper::~Dx11Wrapper()
{
	if (m_initialzed)
		ImGui_ImplDX11_Shutdown();
}

bool Dx11Wrapper::initialize(const Game * game)
{
	m_hwnd = game->getHwnd();
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	initSwapChain(swapChainDesc);
	uint32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	Result hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		0, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_lpSwapChain,
		&m_lpDevice, NULL, &m_lpDeviceContext);
	if (FAILED(hr))
		return false;

	bool r = ImGui_ImplDX11_Init(m_lpDevice.Get(), m_lpDeviceContext.Get());
	if (!r)
		return false;

	m_initialzed = true;
	return true;
}

bool Dx11Wrapper::d3dInitialize()
{
	Result hr;
	ComPtr<ID3D11Texture2D> lpBackBuffer;
	hr = m_lpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)lpBackBuffer.GetAddressOf());
	hr = m_lpDevice->CreateRenderTargetView(lpBackBuffer.Get(), NULL, &m_lpRenderTargetView);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->OMSetRenderTargets(1, m_lpRenderTargetView.GetAddressOf(), NULL);
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	initDepthStencil(depthStencilDesc);
	m_lpDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_lpDepthBuffer);
	hr = m_lpDevice->CreateDepthStencilView(m_lpDepthBuffer.Get(), NULL, &m_lpDepthStencilView);
	if (FAILED(hr))
		return false;

	D3D11_VIEWPORT viewport;
	initViewport(viewport);
	m_lpDeviceContext->RSSetViewports(1, &viewport);
	hr = m_lpDevice->CreateVertexShader(g_VS_Start,
		ARRAYSIZE(g_VS_Start),
		0, &m_lpVShader);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->VSSetShader(m_lpVShader.Get(), 0, 0);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_lpDevice->CreateInputLayout(layout, 2, g_VS_Start,
		ARRAYSIZE(g_VS_Start), &m_lpInputLayout);
	m_lpDeviceContext->IASetInputLayout(m_lpInputLayout.Get());
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	hr = m_lpDevice->CreatePixelShader(g_PS_Start,
		ARRAYSIZE(g_PS_Start),
		0, &m_lpPShader);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->PSSetShader(m_lpPShader.Get(), 0, 0);
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Matrix);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	hr = m_lpDevice->CreateBuffer(&desc, NULL, &m_lpVSConstBuffer);
	D3D11_SAMPLER_DESC sampDesc = {};
	initSampleState(sampDesc);
	m_lpDevice->CreateSamplerState(&sampDesc, &m_lpSampleState);
	m_lpDeviceContext->PSSetSamplers(0, 1, m_lpSampleState.GetAddressOf());
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
	initBlend(rtbd);
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_lpDevice->CreateBlendState(&blendDesc, &m_lpBlendState);
	m_lpDeviceContext->OMSetBlendState(m_lpBlendState.Get(), blendFactor, 0xffffffff);
	return true;
}

void Dx11Wrapper::d3dBegin()
{
	ImGui_ImplDX11_NewFrame();
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_lpDeviceContext->ClearRenderTargetView(m_lpRenderTargetView.Get(), bgColor);
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Dx11Wrapper::d3dEnd()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Dx11Wrapper::d3dPresent()
{
	Result r = m_lpSwapChain->Present(g_gameInfo.vsync, 0);
	if (SUCCEEDED(r))
		::g_frameCounter++;
}

void Dx11Wrapper::d3dDraw(uint32 vertexCount, uint32 startVertex)
{
	m_lpDeviceContext->Draw(vertexCount, startVertex);
}

void Dx11Wrapper::d3dSetSRV(ID3D11ShaderResourceView * pSrv, uint32 numViews, uint32 startSlot)
{
	m_lpDeviceContext->PSSetShaderResources(startSlot, numViews, &pSrv);
}

bool Dx11Wrapper::loadSRVFromFile(const std::string & file, ID3D11ShaderResourceView *& pSrv, int32& width, int32& height)
{
	Result r;
	D3DX11_IMAGE_INFO info;
	r = D3DX11GetImageInfoFromFileA(file.c_str(), 0, &info, &r);
	if (FAILED(r))
		return false;

	width = info.Width;
	height = info.Height;

	D3DX11CreateShaderResourceViewFromFileA(m_lpDevice.Get(), file.c_str(), 0, 0, &pSrv, &r);
	if (FAILED(r))
		return false;

	return true;
}

void Dx11Wrapper::initSwapChain(DXGI_SWAP_CHAIN_DESC & swapChainDesc)
{
	DXGI_MODE_DESC bufferDesc = {};
	bufferDesc.Width = g_gameInfo.width;
	bufferDesc.Height = g_gameInfo.height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.Windowed = g_gameInfo.windowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void Dx11Wrapper::initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc)
{
	depthStencilDesc.Width = g_gameInfo.width;
	depthStencilDesc.Height = g_gameInfo.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
}

void Dx11Wrapper::initViewport(D3D11_VIEWPORT & viewport)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g_gameInfo.width;
	viewport.Height = g_gameInfo.height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void Dx11Wrapper::initSampleState(D3D11_SAMPLER_DESC & sampDesc)
{
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
}

void Dx11Wrapper::initBlend(D3D11_RENDER_TARGET_BLEND_DESC & rtbd)
{
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.DestBlendAlpha = D3D11_BLEND_ONE;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

void Dx11Wrapper::setVSConstBuffer(const void * data)
{
	m_lpDeviceContext->UpdateSubresource(m_lpVSConstBuffer.Get(), 0, 0, data, 0,
		0);
	m_lpDeviceContext->VSSetConstantBuffers(0, 1, m_lpVSConstBuffer.GetAddressOf());
}

void Dx11Wrapper::streamVertexBuffer(LPVertexBuffer vb)
{
	uint32 stride = sizeof(Vertex), offset = 0;
	m_lpDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
}

ID3D11Buffer * Dx11Wrapper::createBuffer(uint32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, uint32 cpuAccess, void* initialData,
	uint32 miscFlag, uint32 stride) const
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = bindFlag;
	desc.MiscFlags = miscFlag; usage;
	desc.CPUAccessFlags = cpuAccess;
	desc.ByteWidth = size;
	desc.Usage = usage;
	D3D11_SUBRESOURCE_DATA srData = {};
	ID3D11Buffer* buf;
	if (initialData == 0)
		m_lpDevice->CreateBuffer(&desc, 0, &buf);
	else
	{
		D3D11_SUBRESOURCE_DATA srData;
		srData.pSysMem = initialData;
		m_lpDevice->CreateBuffer(&desc, &srData, &buf);
	}

	return buf;
}

ID3D11Buffer * Dx11Wrapper::createVertexBuffer(uint32 vertices, Vertex * data)
{
	uint32 size = vertices*sizeof(Vertex);
	D3D11_USAGE usage = (data) ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;

	uint32 cpuAccess = (data) ? 0 : D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer* buf = createBuffer(size, usage, D3D11_BIND_VERTEX_BUFFER, cpuAccess, data);
	return buf;
}

void Dx11Wrapper::d3dStreamVertexBuffer(ID3D11Buffer * vb, uint32 strides, uint32 offset, uint32 numBuffers, uint32 startSlot)
{
	m_lpDeviceContext->IASetVertexBuffers(startSlot, numBuffers, &vb, &strides, &offset);
}

void Dx11Wrapper::setVertexBuffer(ID3D11Buffer* vb, Vertex * vertex, uint32 vertices)
{
	D3D11_MAPPED_SUBRESOURCE data;
	m_lpDeviceContext->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, vertex, sizeof(Vertex)*vertices);
	m_lpDeviceContext->Unmap(vb, 0);
}

void Dx11Wrapper::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8_t len)
{
	D3D11_MAPPED_SUBRESOURCE data;
	uint32 size;
	Vertex* oldData = getVertexBufferData(vb, size);
	m_lpDeviceContext->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	Vertex* v = (Vertex*)data.pData;
	memcpy(v, oldData, size);
	for (int i = 0; i < len; i++)
	{
		v[i].u = vertex[i].u;
		v[i].v = vertex[i].v;
	}

	m_lpDeviceContext->Unmap(vb, 0);
}

Vertex* Dx11Wrapper::getVertexBufferData(ID3D11Buffer* vb, uint32& size) const
{
	D3D11_BUFFER_DESC stagingBufferDesc;
	D3D11_BUFFER_DESC desc;
	vb->GetDesc(&desc);
	size = desc.ByteWidth;
	stagingBufferDesc.ByteWidth = size;
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.BindFlags = 0;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingBufferDesc.MiscFlags = 0;
	ID3D11Buffer* stagingBuffer;
	m_lpDevice->CreateBuffer(&stagingBufferDesc, 0, &stagingBuffer);
	m_lpDeviceContext->CopyResource(stagingBuffer, vb);
	D3D11_MAPPED_SUBRESOURCE data;
	Result HR = m_lpDeviceContext->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &data);
	Vertex* v = (Vertex*)data.pData;
	m_lpDeviceContext->Unmap(stagingBuffer, 0);
	stagingBuffer->Release();
	return v;
}