// graphics.cpp
// Author: abramann
// Note this file is influenced by graphics.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#include "graphics.h"
#include "texture.h"
#include "fileio.h"
#include "image.h"
#include "camera.h"
#include "game.h"
#include "gamemath.h"
#ifdef _BUILD_WITH_D3D9
#include "imgui\imgui_impl_dx9.h"
#else ifdef _BUILD_WITH_D3D11
#include "imgui\imgui_impl_dx11.h"
#include "shader.h"

using Microsoft::WRL::ComPtr;
#endif

uint64  g_frameCounter = 0;

Graphics::Graphics() : m_lpDevice3d(NULL), m_deviceState(NULL)
{
	m_pCamera = std::make_shared<Camera>();
#ifdef _BUILD_WITH_D3D9
	m_lpDirect3d = NULL;
	m_presentParameter = {};
#else ifdef _BUILD_WITH_D3D11
	/*	m_lpDeviceContext = NULL;
		m_lpRenderTargetView = NULL;
		//m_lpSwapChain = NULL;
		m_lpDevice3d = 0;
		m_lpDepthStencilView = 0;
		m_lpDepthBuffer = 0;
		m_lpVSConstBuffer = 0;
		m_lpVShader = 0;
		m_lpPShader = 0;
		m_lpInputLayout = 0;
		m_lpSampleState = 0;
		m_lpBlendState = 0;*/
#endif
}

Graphics::~Graphics()
{
	release();
}

bool Graphics::initialize(const Game* game)
{
	m_hwnd = game->getHwnd();
#ifdef _BUILD_WITH_D3D9
	m_lpDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_lpDirect3d))
		return false;

	m_presentParameter.BackBufferWidth = ::g_gameInfo.width;
	m_presentParameter.BackBufferHeight = ::g_gameInfo.height;
	m_presentParameter.BackBufferCount = 1;
	m_presentParameter.hDeviceWindow = hwnd;
	m_presentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (!g_gameInfo.windowed)
	{
		m_presentParameter.BackBufferFormat = D3DFMT_X8R8G8B8;
		if (checkFullscreenSupport())
			m_presentParameter.Windowed = FALSE;
		else
			return false;
	}
	else
	{
		m_presentParameter.BackBufferFormat = D3DFMT_UNKNOWN;
		m_presentParameter.Windowed = TRUE;
	}

	Result r = m_lpDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_presentParameter,
		&m_lpDevice3d);
	if (FAILED(r))
		return false;

	bool result = ImGui_ImplDX9_Init(m_lpDevice3d);
	if (!result)
		return false;

	Matrix proj;
	D3DXMatrixPerspectiveFovLH(&proj, PROJECT_FOV, g_gameInfo.width / g_gameInfo.height, PROJECT_NEAR_PLANE, PROJECT_FAR_PLANE);
	m_lpDevice3d->SetTransform(D3DTS_PROJECTION, &proj);
	m_lpDevice3d->SetRenderState(D3DRS_LIGHTING, false);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);

#else ifdef _BUILD_WITH_D3D11
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	initSwapChain(swapChainDesc);
	uint32 flags = 0;// D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
	Result hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		0, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_lpSwapChain,
		&m_lpDevice3d, NULL, &m_lpDeviceContext);
	if (FAILED(hr))
		return false;

	bool r = ImGui_ImplDX11_Init(m_lpDevice3d.Get(), m_lpDeviceContext.Get());
	if (!r)
		return false;

	ComPtr<ID3D11Texture2D> lpBackBuffer;
	hr = m_lpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)lpBackBuffer.GetAddressOf());
	hr = m_lpDevice3d->CreateRenderTargetView(lpBackBuffer.Get(), NULL, &m_lpRenderTargetView);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->OMSetRenderTargets(1, m_lpRenderTargetView.GetAddressOf(), NULL);
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	initDepthStencil(depthStencilDesc);
	m_lpDevice3d->CreateTexture2D(&depthStencilDesc, NULL, &m_lpDepthBuffer);
	hr = m_lpDevice3d->CreateDepthStencilView(m_lpDepthBuffer.Get(), NULL, &m_lpDepthStencilView);
	if (FAILED(hr))
		return false;

	D3D11_VIEWPORT viewport;
	initViewport(viewport);
	m_lpDeviceContext->RSSetViewports(1, &viewport);
	hr = m_lpDevice3d->CreateVertexShader(g_VS_Start,
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
	m_lpDevice3d->CreateInputLayout(layout, 2, g_VS_Start,
		ARRAYSIZE(g_VS_Start), &m_lpInputLayout);
	m_lpDeviceContext->IASetInputLayout(m_lpInputLayout.Get());
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	hr = m_lpDevice3d->CreatePixelShader(g_PS_Start,
		ARRAYSIZE(g_PS_Start),
		0, &m_lpPShader);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->PSSetShader(m_lpPShader.Get(), 0, 0);
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(D3DXMATRIX);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	hr = m_lpDevice3d->CreateBuffer(&desc, NULL, &m_lpVSConstBuffer);
	D3D11_SAMPLER_DESC sampDesc = {};
	initSampleState(sampDesc);
	m_lpDevice3d->CreateSamplerState(&sampDesc, &m_lpSampleState);
	m_lpDeviceContext->PSSetSamplers(0, 1, m_lpSampleState.GetAddressOf());
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
	initBlend(rtbd);
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_lpDevice3d->CreateBlendState(&blendDesc, &m_lpBlendState);
	m_lpDeviceContext->OMSetBlendState(m_lpBlendState.Get(), blendFactor, 0xffffffff);
#endif
	m_pCamera->initialize(game);
	return true;
}

LPVertexBuffer Graphics::createVertexBuffer(uint32 vertices, VB_USAGE usage, Vertex* data)
{
	LPVertexBuffer vb = 0;
	size_t size = vertices*sizeof(Vertex);
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->CreateVertexBuffer(size, usage, VERTEX_FVF, D3DPOOL_DEFAULT, &vb, 0);
	if (data)
		setVertexBuffer(vb, data, vertices);
#else ifdef _BUILD_WITH_D3D11
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = vertices*sizeof(Vertex);
	desc.Usage = (D3D11_USAGE)usage;
	uint32 cpuAccess = 0;
	if (usage == VB_USAGE_WRITE)
		cpuAccess = D3D11_CPU_ACCESS_WRITE;

	desc.CPUAccessFlags = cpuAccess;
	if (data == 0)
		m_lpDevice3d->CreateBuffer(&desc, 0, &vb);
	else
	{
		D3D11_SUBRESOURCE_DATA bufData;
		bufData.pSysMem = data;
		m_lpDevice3d->CreateBuffer(&desc, &bufData, &vb);
	}
#endif
	return vb;
}

void Graphics::release()
{
#ifdef _BUILD_WITH_D3D9
	safeRelease(m_lpDirect3d);
	safeRelease(m_lpDevice3d);
	if (m_lpDevice3d)
		ImGui_ImplDX9_Shutdown();
#else ifdef _BUILD_WITH_D3D11
	ImGui_ImplDX11_Shutdown();
#endif
}

Result Graphics::begin()
{
	Result r = S_OK;
#ifdef _BUILD_WITH_D3D9
	ImGui_ImplDX9_NewFrame();
	m_lpDevice3d->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, NULL);
	m_lpDevice3d->SetFVF(VERTEX_FVF);
	r = m_lpDevice3d->BeginScene();
#else ifdef _BUILD_WITH_D3D11
	ImGui_ImplDX11_NewFrame();
	float bgColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_lpDeviceContext->ClearRenderTargetView(m_lpRenderTargetView.Get(), bgColor);
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return r;
}

void Graphics::drawImage(const Image* image)
{
	int16 vertices = image->getVertices();
	LPVertexBuffer vb = image->getVertexBuffer();
	LPTextureD3D texture = image->getTexture();
	V3 position = image->getPosition(),
		&scalling = image->getScalling(),
		rotate = image->getRotate(),
		rotateCenter = image->getRotateCenter();
	setDrawProperties(position, scalling, rotate, rotateCenter);
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetStreamSource(0, vb, 0, sizeof(Vertex));
	setTexture(texture);
	m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices / 3);
#else ifdef _BUILD_WITH_D3D11
	streamVertexBuffer(vb);
	setTexture(texture);
	m_lpDeviceContext->Draw(vertices, 0);
#endif
}

Result Graphics::end()
{
	Result r = S_OK;
	ImGui::EndFrame();
	ImGui::Render();
#ifdef _BUILD_WITH_D3D9
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	r = m_lpDevice3d->EndScene();
#else _BUILD_WITH_D3D11
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	return r;
}

LPDevice Graphics::getDevice() const
{
#ifdef _BUILD_WITH_D3D9
	return m_lpDevice3d;
#else ifdef _BUILD_WITH_D3D11
	return m_lpDevice3d.Get();
#endif
}

Result Graphics::showBackbuffer()
{
	Result r;
#ifdef _BUILD_WITH_D3D9
	r = m_lpDevice3d->Present(NULL, NULL, NULL, NULL);
#else #ifdef _BUILD_WITH_D3D11
	r = m_lpSwapChain->Present(0, 0);
#endif

	if (SUCCEEDED(r))
		::g_frameCounter++;
	return r;
}

Result Graphics::drawPrimitive(UINT startVertex, UINT count)
{
	Result r = S_OK;
#ifdef _BUILD_WITH_D3D9
	r = m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, count);
#else ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->Draw(count * 3, startVertex);
#endif
	return r;
}

void Graphics::setDrawProperties(V3 position, V3 scall, V3 rotate, V3 rotateCenter)
{
	Matrix rot = V3ToMatrix(rotate, MATRIX_TYPE_ROTATE),
		rotCent = V3ToMatrix(rotateCenter, MATRIX_TYPE_TRANSLATE),
		rotCentN = V3ToMatrix(-1 * rotateCenter, MATRIX_TYPE_TRANSLATE),
		scal = V3ToMatrix(scall, MATRIX_TYPE_SCALL),
		pos = V3ToMatrix(position, MATRIX_TYPE_TRANSLATE);

	Matrix prop = scal*rot*pos;
	//	Matrix prop = scal*rotCentN*rot*rotCent*pos;

#ifdef _BUILD_WITH_D3D9
	setWorldMatrix(prop);
#else ifdef _BUILD_WITH_D3D11
	prop *= m_wvp;
	gameMathNS::matrixTranspose(&prop, &prop);
	setWorldMatrix(&prop);
#endif
}

bool Graphics::loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPTextureD3D& texture)
{
	Result r;
#ifdef _BUILD_WITH_D3D9
	D3DXIMAGE_INFO info;
	r = D3DXGetImageInfoFromFileA(file, &info);
	if (FAILED(r))
		return false;

	width = info.Width;
	height = info.Height;
	ZeroMemory(&texture, sizeof(texture));
	r = D3DXCreateTextureFromFileExA(m_lpDevice3d, file, width,
		height, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transparency,
		&info, NULL, &texture);
	if (FAILED(r))
		return false;
	return true;
#else ifdef _BUILD_WITH_D3D11
	D3DX11_IMAGE_INFO info;
	D3DX11GetImageInfoFromFileA(file, 0, &info, &r);

	if (FAILED(r))
		return false;

	width = info.Width;
	height = info.Height;
	/*D3D11_SUBRESOURCE_DATA data = {};
	uint32 size;
	char* memFile = FileIO::loadFileInMemory(file, size);
	data.pSysMem = memFile;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	auto rr = m_lpDevice3d->CreateBuffer(&bufferDesc,&data, &buffer);
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.
	rr = m_lpDevice3d->CreateShaderResourceView(buffer, 0, &texture);*/
	D3DX11CreateShaderResourceViewFromFileA(m_lpDevice3d.Get(), file, 0, 0, &texture, &r);
	if (FAILED(r))
		return false;

	return true;
#endif
}

void Graphics::setVertexBuffer(LPVertexBuffer vb, Vertex * vertex, int32_t vertices)
{
#ifdef _BUILD_WITH_D3D9
	Vertex* ptr;
	vb->Lock(0, 0, (void**)&ptr, 0);
	memcpy(ptr, vertex, vertices*sizeof(Vertex));
	vb->Unlock();
#else ifdef _BUILD_WITH_D3D11
	D3D11_MAPPED_SUBRESOURCE data;
	m_lpDeviceContext->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, vertex, sizeof(Vertex)*vertices);
	m_lpDeviceContext->Unmap(vb, 0);
#endif
}

void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8_t len)
{
#ifdef _BUILD_WITH_D3D9
	Vertex* v;
	vb->Lock(0, 0, (void**)&v, 0);
	for (int i = 0; i < len; i++)
	{
		v[i].u = vertex[i].u;
		v[i].v = vertex[i].v;
	}

	vb->Unlock();
#else ifdef _BUILD_WITH_D3D11

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
#endif
}

void Graphics::setWorldMatrix(Matrix* worldMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	setVSConstBuffer(worldMatrix);
#endif
}

std::vector<Resolution> Graphics::getSupportedResolutions() const
{
	std::vector<Resolution> resolution;
#ifdef _BUILD_WITH_D3D9
	int modes = m_lpDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_presentParameter.BackBufferFormat);
	D3DDISPLAYMODE dMode;
	for (auto i = 0; i <= modes; i++)
	{
		m_lpDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT, m_presentParameter.BackBufferFormat, i, &dMode);
		if (dMode.Width < MIN_RESOLUTION_WIDTH ||
			dMode.Height < MIN_RESOLUTION_HEIGHT ||
			dMode.Height >= dMode.Width ||
			dMode.Format != m_presentParameter.BackBufferFormat)
			continue;

		Resolution resol;
		resol.width = dMode.Width, resol.height = dMode.Height;
		resolution.push_back(resol);
	}

#else ifdef _BUILD_WITH_D3D11
	IDXGIFactory* factory;
	CreateDXGIFactory(IID_PPV_ARGS(&factory));
	IDXGIAdapter* adapter;
	factory->EnumAdapters(0, &adapter);
	IDXGIOutput* adapterOutput;
	adapter->EnumOutputs(0, &adapterOutput);
	uint32 numModes;
	DXGI_MODE_DESC* displayModeList;
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL);
	displayModeList = new DXGI_MODE_DESC[numModes];
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);

	for (uint32 i = 0; i < numModes; i += 2)
		if (displayModeList[i].Width >= gameNS::MIN_RESOLUTION_WIDTH && displayModeList[i].Height >= gameNS::MIN_RESOLUTION_HEIGHT)
		{
			Resolution resol;
			resol.width = displayModeList[i].Width,
				resol.height = displayModeList[i].Height;
			resolution.push_back(resol);
		}

	safeDeleteArray(displayModeList);
	safeRelease(factory);
	safeRelease(adapter);
	safeRelease(adapterOutput);
#endif
	return resolution;
}

Resolution Graphics::getResolution()
{
	Resolution resolution;
	GameInfo info = FileIO::readGameInfo();
	resolution.width = info.width;
	resolution.height = info.height;
	return resolution;
}

void Graphics::setResolution(const Resolution& resolution)
{
	GameInfo info;
	info.windowed = -1;
	info.width = resolution.width;
	info.height = resolution.height;
	FileIO::createGameInfo(info);
	resize(1, 1);
}

void Graphics::setTexture(LPTextureD3D texture)
{
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->SetTexture(0, texture);
#else ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->PSSetShaderResources(0, 1, &texture);
#endif
}

bool Graphics::isWindowed() const
{
	bool windowed = (FileIO::readGameInfo().windowed > 0);
	return windowed;
}

void Graphics::setWindowed(const bool& windowed)
{
	GameInfo info;
	info.width = -1;
	info.height = -1;
	info.windowed = windowed;
	FileIO::createGameInfo(info);
}

void Graphics::resize(uint16_t width, uint16_t height)
{
#ifdef _BUILD_WITH_D3D11
	m_lpSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// Create a new render target view
	ComPtr<ID3D11Texture2D> backBuffer;
	m_lpSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	m_lpDevice3d->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_lpRenderTargetView);

	// Create a new depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	ComPtr<ID3D11Texture2D> lpDepthStencilBuffer;
	m_lpDevice3d->CreateTexture2D(&depthStencilDesc, nullptr, &lpDepthStencilBuffer);
	m_lpDevice3d->CreateDepthStencilView(lpDepthStencilBuffer.Get(), nullptr, &m_lpDepthStencilView);

	// Set the render target and viewport
	m_lpDeviceContext->OMSetRenderTargets(1, &m_lpRenderTargetView, m_lpDepthStencilView.Get());
	D3D11_VIEWPORT viewport = {};
	initViewport(viewport);
	m_lpDeviceContext->RSSetViewports(1, &viewport);

	if (g_gameInfo.windowed)
	{
		RECT clientRect;
		GetClientRect(m_hwnd, &clientRect);   // get size of client area of window
		MoveWindow(m_hwnd,
			0,                                           // Left
			0,                                           // Top
			width + (width - clientRect.right),    // Right
			height + (height - clientRect.bottom), // Bottom
			TRUE);
	}
#endif
}

void Graphics::streamVertexBuffer(LPVertexBuffer vb)
{
	UINT stride = sizeof(Vertex), offset = 0;
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetStreamSource(0, vb, 0, stride);
#else #ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
#endif
}

bool Graphics::checkFullscreenSupport() const
{
	std::vector<Resolution> resolution = getSupportedResolutions();
	for (auto resol : resolution)
		if (g_gameInfo.width == resol.width && g_gameInfo.height == g_gameInfo.height)
			return true;

	return false;
}

void Graphics::setViewMatrix(Matrix* viewMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_VIEW, &viewMatrix);
#else ifdef _BUILD_WITH_D3D11
	m_wvp = *viewMatrix;
#endif
}

void Graphics::initSwapChain(DXGI_SWAP_CHAIN_DESC & swapChainDesc)
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

void Graphics::initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc)
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

void Graphics::initViewport(D3D11_VIEWPORT & viewport)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g_gameInfo.width;
	viewport.Height = g_gameInfo.height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void Graphics::initSampleState(D3D11_SAMPLER_DESC & sampDesc)
{
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
}

void Graphics::initBlend(D3D11_RENDER_TARGET_BLEND_DESC & rtbd)
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

Matrix Graphics::V3ToMatrix(const V3 v3, MatrixType type)
{
	Matrix mat;
	const float &x = v3.x, &y = v3.y, &z = v3.z;
	switch (type)
	{
	case MATRIX_TYPE_TRANSLATE:
		gameMathNS::matrixTranslation(&mat, x, y, z);
		break;
	case MATRIX_TYPE_SCALL:
		gameMathNS::matrixScaling(&mat, x, y, z);
		break;
	case MATRIX_TYPE_ROTATE:
		gameMathNS::matrixRotationX(&mat, x);
		gameMathNS::matrixRotationY(&mat, y);
		gameMathNS::matrixRotationZ(&mat, z);
		break;
	default:
		break;
	}

	return mat;
}

#ifdef _BUILD_WITH_D3D11
void Graphics::setVSConstBuffer(const void * data)
{
	m_lpDeviceContext->UpdateSubresource(m_lpVSConstBuffer.Get(), 0, 0, data, 0,
		0);
	m_lpDeviceContext->VSSetConstantBuffers(0, 1, m_lpVSConstBuffer.GetAddressOf());
}

Vertex* Graphics::getVertexBufferData(ID3D11Buffer* vb, uint32& size) const
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
	m_lpDevice3d->CreateBuffer(&stagingBufferDesc, 0, &stagingBuffer);
	m_lpDeviceContext->CopyResource(stagingBuffer, vb);
	D3D11_MAPPED_SUBRESOURCE data;
	Result HR = m_lpDeviceContext->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &data);
	Vertex* v = (Vertex*)data.pData;
	m_lpDeviceContext->Unmap(stagingBuffer, 0);
	stagingBuffer->Release();
	return v;
}

#endif