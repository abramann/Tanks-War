// dx11wrapper.h
// Author: abramann

#include "dx11wrapper.h"
#include "game.h"
#include "imgui\imgui_impl_dx11.h"
#include "fileio.h"
#include "types.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "inlined.inl"
#include <D3DX11.h>

using namespace Microsoft::WRL;

Dx11Wrapper::Dx11Wrapper() : m_initialzed(false)
{
}

Dx11Wrapper::~Dx11Wrapper()
{
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, nullptr);
	if (m_initialzed)
		ImGui_ImplDX11_Shutdown();
}

bool Dx11Wrapper::initialize(const Game * pGame)
{
	m_hwnd = pGame->getHwnd();
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	initSwapChain(swapChainDesc);
	uint32 flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain,
		&m_pDevice, NULL, &m_pDeviceContext)))
		return false;

	if (!m_initialzed)
		if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get()))
			return false;

	m_initialzed = true;
	return true;
}

bool Dx11Wrapper::d3dInitialize()
{
	ComPtr<ID3D11Texture2D> pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf());
	m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &m_pRenderTargetView);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	initDepthStencil(depthStencilDesc);
	m_pDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthBuffer);
	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), NULL, &m_pDepthStencilView)))
		return false;

	D3D11_VIEWPORT viewport;
	initViewport(viewport);
	m_pDeviceContext->RSSetViewports(1, &viewport);
	m_pDevice->CreateVertexShader(g_pVertexShader, ARRAYSIZE(g_pVertexShader), 0, &m_pVShader);
	m_pDeviceContext->VSSetShader(m_pVShader.Get(), 0, 0);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_pDevice->CreateInputLayout(layout, 2, g_pVertexShader, ARRAYSIZE(g_pVertexShader), &m_pInputLayout);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevice->CreatePixelShader(g_pPixelShader, ARRAYSIZE(g_pPixelShader), 0, &m_pPShader);
	m_pDeviceContext->PSSetShader(m_pPShader.Get(), 0, 0);
	D3D11_SAMPLER_DESC sampDesc = {};
	initSampleState(sampDesc);
	m_pDevice->CreateSamplerState(&sampDesc, &m_pSampleState);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSampleState.GetAddressOf());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
	initBlend(rtbd);
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
	return true;
}

void Dx11Wrapper::d3dBegin()
{
	ImGui_ImplDX11_NewFrame();
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
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
	if (SUCCEEDED(m_pSwapChain->Present(g_pGameSettings->vsync, 0)))
		::g_frameCounter++;
}

void Dx11Wrapper::d3dDraw(uint32 vertexCount, uint32 startVertex)
{
	m_pDeviceContext->Draw(vertexCount, startVertex);
}

void Dx11Wrapper::d3dDrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex)
{
	m_pDeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);
}

void Dx11Wrapper::psSetSRV(ID3D11ShaderResourceView ** ppSRV, uint32 numViews, uint32 startSlot)
{
	m_pDeviceContext->PSSetShaderResources(startSlot, numViews, ppSRV);
}

bool Dx11Wrapper::createSRVFromFile(const std::string & file, ID3D11ShaderResourceView ** ppSRV, uint32& width, uint32& height)
{
	D3DX11_IMAGE_INFO info;
	if (FAILED(D3DX11GetImageInfoFromFileA(file.c_str(), 0, &info, 0)))
		return false;

	width = info.Width;
	height = info.Height;
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice.Get(), file.c_str(), 0, 0, ppSRV, 0)))
		return false;

	return true;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Dx11Wrapper::createBuffer(D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, uint32 cpuAccess, uint32 byteWidth, uint32 stride, void * initialData, uint32 miscFlag) const
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = bindFlag;
	desc.MiscFlags = miscFlag; usage;
	desc.CPUAccessFlags = cpuAccess;
	desc.ByteWidth = byteWidth;
	desc.Usage = usage;
	desc.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA srData = {};
	ComPtr<ID3D11Buffer> pBuf;
	if (initialData == 0)
		m_pDevice->CreateBuffer(&desc, 0, &pBuf);
	else
	{
		D3D11_SUBRESOURCE_DATA srData;
		srData.pSysMem = initialData;
		m_pDevice->CreateBuffer(&desc, &srData, &pBuf);
	}

	if (pBuf == nullptr)
	{
		debuggerBreak(true, "m_pDevice->CreateBuffer failed");
		throw GameError(gameErrorNS::FATAL_ERROR, "m_pDevice->CreateBuffer failed");
	}

	return pBuf;
}

void Dx11Wrapper::initSwapChain(DXGI_SWAP_CHAIN_DESC & swapChainDesc)
{
a:
	DXGI_MODE_DESC bufferDesc = {};
	if (g_pGameSettings->windowed)
	{
		bufferDesc.Width = g_pGameSettings->width;
		bufferDesc.Height = g_pGameSettings->height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Windowed = true;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc = bufferDesc;
	}
	else
	{
		auto mode = enurmerateAdapterMode();
		for (uint32 i = 0; i < mode.size(); i++)
		{
			DXGI_MODE_DESC m = mode[i];
			if (m.Width == g_pGameSettings->width && m.Height == g_pGameSettings->height)
			{
				swapChainDesc.BufferDesc = m;
				break;
			}
			else if (i == mode.size() - 1)
			{
				messageBoxOk(strFormat("Graphics adapter does not support fullscreen %dx%d mode\n Switching to windowed mode", g_pGameSettings->width, g_pGameSettings->height), "WARNING");
				g_pGameSettings->windowed = true;
				goto a;
			}
		}
	}

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void Dx11Wrapper::initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc)
{
	depthStencilDesc.Width = g_pGameSettings->width;
	depthStencilDesc.Height = g_pGameSettings->height;
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
	viewport.Width = static_cast<float>(g_pGameSettings->width);
	viewport.Height = static_cast<float>(g_pGameSettings->height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void Dx11Wrapper::initSampleState(D3D11_SAMPLER_DESC & sampDesc)
{
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0.0f;	// use full resolution of the texture
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

void Dx11Wrapper::vsSetConstBuffer(ID3D11Buffer* pVSConstBuffer, const void * pData, uint32 slot)
{
	m_pDeviceContext->VSSetConstantBuffers(slot, 1, &pVSConstBuffer);
	m_pDeviceContext->UpdateSubresource(pVSConstBuffer, 0, 0, pData, 0,
		0);
}

void Dx11Wrapper::onResize(int32 width, int32 height)
{
	setFullScreen(!g_pGameSettings->windowed);
	if (m_pSwapChain)
	{
		m_pDeviceContext->OMSetRenderTargets(0, 0, 0);
		m_pRenderTargetView.Reset();
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		if (!g_pGameSettings->windowed)
		{
			auto mode = enurmerateAdapterMode();
			for (auto m : mode)
				if (m.Width == g_pGameSettings->width && m.Height == g_pGameSettings->height)
				{
					format = m.Format;
					break;
				}
		}

		m_pRenderTargetView.Reset();
		m_pSwapChain->ResizeBuffers(1, 0, 0, format, 0);
		ComPtr<ID3D11Texture2D> pBuffer;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)pBuffer.GetAddressOf());
		m_pDevice->CreateRenderTargetView(pBuffer.Get(), NULL,
			&m_pRenderTargetView);
		m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f, vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0, vp.TopLeftY = 0;
		m_pDeviceContext->RSSetViewports(1, &vp);
	}
}

std::vector<DXGI_MODE_DESC> Dx11Wrapper::enurmerateAdapterMode()
{
	static std::vector<DXGI_MODE_DESC> enurmAdapter;
	if (enurmAdapter.empty())
	{
		ComPtr<IDXGIFactory> pFactory;
		CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
		ComPtr<IDXGIAdapter> pAdapter;
		pFactory->EnumAdapters(0, &pAdapter);
		ComPtr<IDXGIOutput> pAdapterOutput;
		pAdapter->EnumOutputs(0, &pAdapterOutput);
		uint32 numModes;
		pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes, NULL);
		DXGI_MODE_DESC* pDisplayModeList = new DXGI_MODE_DESC[numModes];
		pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes, pDisplayModeList);
		for (uint32 i = 0; i < numModes; i += 2)
			if (pDisplayModeList[i].Width >= gameNS::MIN_RESOLUTION_WIDTH && pDisplayModeList[i].Height >= gameNS::MIN_RESOLUTION_HEIGHT)
				enurmAdapter.push_back(pDisplayModeList[i]);

		safeDeleteArray(pDisplayModeList);
	}

	return enurmAdapter;
}

void Dx11Wrapper::setFullScreen(bool fullscreen) const
{
	m_pSwapChain->SetFullscreenState(fullscreen, nullptr);
}

void Dx11Wrapper::streamVertexBuffer(ID3D11Buffer* pVB)
{
	uint32 stride = sizeof(Vertex), offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Dx11Wrapper::createVertexBuffer(uint32 vertices, uint32 cpuAccess, Vertex * pData)
{
	D3D11_USAGE usage = (pData) ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	return createBuffer(usage, D3D11_BIND_VERTEX_BUFFER, cpuAccess, vertices*sizeof(Vertex), sizeof(Vertex), pData);
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Dx11Wrapper::createIndexBuffer(uint32 byteSize, uint32 cpuAccess, void * pData)
{
	return createBuffer(D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, cpuAccess, byteSize,
		0, pData);
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Dx11Wrapper::createStructuredBuffer(uint32 elementSize, uint32 count, void * pInitData, int32 cpuAccess)
{
	uint32 bindFlag = 0, usage = 0;
	if (cpuAccess == D3D11_CPU_ACCESS_WRITE)
	{
		usage = D3D11_USAGE_DYNAMIC;
		bindFlag = D3D11_BIND_SHADER_RESOURCE;
	}
	else
		bindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

	return createBuffer((D3D11_USAGE)usage, (D3D11_BIND_FLAG)bindFlag, cpuAccess, elementSize*count, elementSize,
		pInitData, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Dx11Wrapper::createStagingBuffer(D3D11_CPU_ACCESS_FLAG cpuAccess, uint32 elementSize, uint32 count, void * pInitData)
{
	return createBuffer(D3D11_USAGE_STAGING, (D3D11_BIND_FLAG)0, cpuAccess, elementSize*count, elementSize, pInitData, 0);
}

void Dx11Wrapper::copyResourceToResource(ID3D11Resource * pDest, ID3D11Resource * pSource)
{
	m_pDeviceContext->CopyResource(pDest, pSource);
}

void Dx11Wrapper::copyResource(void * pDest, ID3D11Resource * pSource, uint32 size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pDeviceContext->Map(pSource, 0, D3D11_MAP_READ, 0, &mappedResource);
	memcpy(pDest, mappedResource.pData, size);
	m_pDeviceContext->Unmap(pSource, 0);
}

void Dx11Wrapper::copyToResource(ID3D11Resource * pDest, void * pSource, uint32 size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pDeviceContext->Map(pDest, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, pSource, size);
	m_pDeviceContext->Unmap(pDest, 0);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Dx11Wrapper::createBufferSRV(ID3D11Buffer * pBuffer)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	m_pDevice->CreateShaderResourceView(pBuffer, &desc, &pSRV);
	return pSRV;
}

Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> Dx11Wrapper::createBufferUAV(ID3D11Buffer * pBuffer)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUAV;
	m_pDevice->CreateUnorderedAccessView(pBuffer, &desc, &pUAV);
	return pUAV;
}

Microsoft::WRL::ComPtr<ID3D11ComputeShader> Dx11Wrapper::createComputeShader(const BYTE* pByteCode, size_t byteCodeSize)
{
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
	m_pDevice->CreateComputeShader(pByteCode, byteCodeSize, NULL, &pComputeShader);
	return pComputeShader;
}

void Dx11Wrapper::runComputeShader(ID3D11ComputeShader * pComputeShader, uint32 SRVs, ID3D11ShaderResourceView ** ppSRV, uint32 uavs, ID3D11UnorderedAccessView ** ppUnorderedAccessView, uint32 x, uint32 y, uint32 z)
{
	m_pDeviceContext->CSSetShader(pComputeShader, 0, 0);
	m_pDeviceContext->CSSetShaderResources(0, SRVs, ppSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, uavs, ppUnorderedAccessView, 0);
	m_pDeviceContext->Dispatch(x, y, z);
}

void Dx11Wrapper::iaSetStreamBuffer(ID3D11Buffer ** ppVB, uint32 strides, uint32 offset, uint32 numBuffers, uint32 startSlot)
{
	m_pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, ppVB, &strides, &offset);
}

void Dx11Wrapper::iaSetIndexBuffer(ID3D11Buffer * pIB, DXGI_FORMAT format, uint32 offset)
{
	m_pDeviceContext->IASetIndexBuffer(pIB, format, offset);
}