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

Graphics::Graphics() : m_ppcSuppResol(0)
{
	m_pCamera = std::make_shared<Camera>();
}

Graphics::~Graphics()
{
	for (size_t i = 0; i < m_suppModes.size(); i++)
		safeDeleteArray(m_ppcSuppResol[i]);

	safeDeleteArray(m_ppcSuppResol);
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

size_t Graphics::getCurrentAdapterMode() const
{
	for (size_t i = 0; i < m_suppModes.size(); i++)
		if (m_suppModes[i].width == g_gameInfo.width &&
			m_suppModes[i].height == g_gameInfo.height)
			return i;

	return 0;
}

void Graphics::setDrawProperties(V3 position, V3 scall, V3 rotate, V3 rotateCenter)
{
	Matrix rot = gameMathNS::V3ToMatrix(rotate, MATRIX_TYPE_ROTATE),
		rotCent = gameMathNS::V3ToMatrix(rotateCenter, MATRIX_TYPE_TRANSLATE),
		rotCentN = gameMathNS::V3ToMatrix(rotateCenter * -1, MATRIX_TYPE_TRANSLATE),
		scal = gameMathNS::V3ToMatrix(scall, MATRIX_TYPE_SCALL),
		pos = gameMathNS::V3ToMatrix(position, MATRIX_TYPE_TRANSLATE);
	static int i = 0;
	Matrix prop;
	prop = scal*rot*pos;
	//prop = scal*rotCentN*rot*rotCent*pos;
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
	m_pDx11Wrapper->setVertexBuffer(vb, vertex, vertices);
}

void Graphics::streamVertexBuffer(LPVertexBuffer vb)
{
	m_pDx11Wrapper->streamVertexBuffer(vb);
}

void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8 len)
{
	m_pDx11Wrapper->setVertexBufferUV(vb, vertex, len);
}

/*
void Graphics::resize(Resolution resol)
{
	RECT clientRect;
	GetClientRect(m_hwnd, &clientRect);
	MoveWindow(m_hwnd,
		0,                                           // Left
		0,                                           // Top
		g_gameInfo.width + (g_gameInfo.width - clientRect.right),    // Right
		g_gameInfo.height + (g_gameInfo.height - clientRect.bottom), // Bottom
		TRUE);
}

void Graphics::setWindowedMode(bool fullscreen)
{
}
*/

void Graphics::setWorldMatrix(Matrix* worldMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->setVSConstBuffer(worldMatrix);
#endif
}

std::vector<Resolution> Graphics::getSupportedAdapterModes() const
{
	if (!m_suppModes.empty())
		return m_suppModes;

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
			m_suppModes.push_back(resol);
		}

	safeDeleteArray(displayModeList);
	safeRelease(factory);
	safeRelease(adapter);
	safeRelease(adapterOutput);
	return m_suppModes;
}

char** Graphics::getSupportedAdapterModesAsCArray() const
{
	if (m_ppcSuppResol != nullptr)
		return m_ppcSuppResol;

	auto supResol = getSupportedAdapterModes();
	int8 supResols = static_cast<int8>(getSupportedAdapterModes().size());

	m_ppcSuppResol = new char*[supResols];
	for (int i = 0; i < supResols; i++)
	{
		m_ppcSuppResol[i] = new char[9];
		memset(m_ppcSuppResol[i], 0, 9);
		sprintf(m_ppcSuppResol[i], "%dx%d", supResol[i].width, supResol[i].height);
	}

	return m_ppcSuppResol;
}

void Graphics::setTexture(LPTextureD3D texture)
{
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->SetTexture(0, texture);
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->d3dSetSRV(texture);
#endif
}

bool Graphics::checkFullscreenSupport() const
{
	std::vector<Resolution> resolution = getSupportedAdapterModes();
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