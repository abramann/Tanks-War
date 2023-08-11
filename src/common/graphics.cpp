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
#include "dx11wrapper.h"

#ifdef _BUILD_WITH_D3D9
#include "imgui\imgui_impl_dx9.h"
#else ifdef _BUILD_WITH_D3D11
#include "imgui\imgui_impl_dx11.h"
#include "shader.h"

using Microsoft::WRL::ComPtr;
#endif

uint64  g_frameCounter = 0;

Graphics::Graphics()
{
	m_pCamera = std::make_shared<Camera>();
}

Graphics::~Graphics()
{
	release();
}

bool Graphics::initialize(const Game* game)
{
	m_hwnd = game->getHwnd();
	m_pDx11Wrapper = game->getDx11Wrapper();
	bool result = m_pDx11Wrapper->d3dInitialize();
	m_pCamera->initialize(game);
	return result;
}

LPVertexBuffer Graphics::createVertexBuffer(uint32 vertices, Vertex* data)
{
	return m_pDx11Wrapper->createVertexBuffer(vertices, data);
}

void Graphics::release()
{
#ifdef _BUILD_WITH_D3D9
	safeRelease(m_lpDirect3d);
	safeRelease(m_lpDevice3d);
	if (m_lpDevice3d)
		ImGui_ImplDX9_Shutdown();
#else ifdef _BUILD_WITH_D3D11
#endif
}

void Graphics::beginRender()
{
	m_pDx11Wrapper->d3dBegin();
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
	m_pDx11Wrapper->d3dStreamVertexBuffer(vb, sizeof(Vertex), 0);
	setTexture(texture);
	m_pDx11Wrapper->d3dDraw(vertices, 0);
	
#endif
}

void Graphics::endRender()
{
	m_pDx11Wrapper->d3dEnd();
}

void Graphics::showBackbuffer()
{
	m_pDx11Wrapper->d3dPresent();
}

void Graphics::drawPrimitive(uint32 startVertex, uint32 count)
{
	m_pDx11Wrapper->d3dDraw(count * 3, startVertex);
}

void Graphics::setDrawProperties(V3 position, V3 scall, V3 rotate, V3 rotateCenter)
{
	Matrix rot = V3ToMatrix(rotate, MATRIX_TYPE_ROTATE),
		rotCent = V3ToMatrix(rotateCenter, MATRIX_TYPE_TRANSLATE),
		rotCentN = V3ToMatrix(rotateCenter * -1, MATRIX_TYPE_TRANSLATE),
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

bool Graphics::loadTexture(const char* file, int32& width, int32& height, LPTextureD3D& texture)
{
	bool result = m_pDx11Wrapper->loadSRVFromFile(file, texture, width, height);
	return result;
}

void Graphics::setVertexBuffer(LPVertexBuffer vb, Vertex * vertex, int32_t vertices)
{
#ifdef _BUILD_WITH_D3D9
	Vertex* ptr;
	vb->Lock(0, 0, (void**)&ptr, 0);
	memcpy(ptr, vertex, vertices*sizeof(Vertex));
	vb->Unlock();
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->setVertexBuffer(vb, vertex, vertices);
#endif
}

void Graphics::streamVertexBuffer(LPVertexBuffer vb)
{
	m_pDx11Wrapper->streamVertexBuffer(vb);
}

void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8 len)
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
	m_pDx11Wrapper->setVertexBufferUV(vb, vertex, len);
	
#endif
}

void Graphics::setWorldMatrix(Matrix* worldMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->setVSConstBuffer(worldMatrix);
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
	GameInfo info = FileIO::readGameInfo();
	//info.windowed = -1;
	//info.vsync = -1;
	info.width = resolution.width;
	info.height = resolution.height;
	FileIO::createGameInfo(info);
	//resize(1, 1);
}

void Graphics::setTexture(LPTextureD3D texture)
{
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->SetTexture(0, texture);
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->d3dSetSRV(texture);
#endif
}

bool Graphics::isWindowed() const
{
	bool windowed = (FileIO::readGameInfo().windowed > 0);
	return windowed;
}

void Graphics::setWindowed(const bool& windowed)
{
	GameInfo info = FileIO::readGameInfo();
	info.windowed = windowed;
	FileIO::createGameInfo(info);
}

void Graphics::resize(uint16_t width, uint16_t height)
{
#ifdef _BUILD_WITH_D3D11
	/*m_lpSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

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
	*/
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
