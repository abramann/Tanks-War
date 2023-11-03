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
#include "line.h"
#include "dx11wrapper.h"
#include "inlined.inl"

using Microsoft::WRL::ComPtr;

uint64  g_frameCounter = 0;

Graphics::Graphics()
{
	m_pCamera = std::make_shared<Camera>();
	m_pLine = std::make_shared<Line>();
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(const Game* pGame)
{
	m_hwnd = pGame->getHwnd();
	m_pDx11Wrapper = pGame->getDx11Wrapper();
	if (!m_pDx11Wrapper->d3dInitialize())
		return false;

	m_pCamera->initialize(pGame);
	m_pWVMBuf = m_pDx11Wrapper->createBuffer(D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, sizeof(Matrix));
	m_pImageMatrixBuf = m_pDx11Wrapper->createBuffer(D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, sizeof(float[4]) * 4); // 36 + 12 = 48 size should be of twice 16
	uint32 imageIndex[] = { 0, 1, 2, 2, 1, 3 };
	m_pImageIndexBuf = m_pDx11Wrapper->createIndexBuffer(sizeof(uint32) * 6, 0, imageIndex);
	m_pLine->initialize(pGame);
	return true;
}

void Graphics::beginRender()
{
	m_pDx11Wrapper->d3dBegin();
}

void Graphics::drawImage(const Image* pImage)
{
	DxBuffer* pVB = pImage->getVertexBuffer();
	LPTextureD3D texture = pImage->getTexture();
	V3 position = pImage->getPosition(),
		&scalling = pImage->getScalling(),
		rotate = pImage->getRotate(),
		rotateCenter = pImage->getRotateCenter();
	setDrawProperties(position, scalling, rotate, rotateCenter);
	m_pDx11Wrapper->streamVertexBuffer(pVB);
	m_pDx11Wrapper->iaSetIndexBuffer(m_pImageIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);
	setTexture(texture);
	m_pDx11Wrapper->d3dDrawIndexed(6, 0, 0);
	if (g_pGameSettings->debugMode && pImage != m_pLine.get())
		drawBox(pImage->getSpace());
}

void Graphics::drawLine(const Vector3D & vector)
{
	m_pLine->draw(vector);
}

void Graphics::drawBox(const Space & space, const float& size)
{
	std::vector<Vector3D> boxVector;
	boxVector.push_back(Vector3D(space.v1, space.v2, size));
	boxVector.push_back(Vector3D(space.v2, space.v3, size));
	boxVector.push_back(Vector3D(space.v3, space.v4, size));
	boxVector.push_back(Vector3D(space.v4, space.v1, size));
	for (const auto& vector : boxVector)
		drawLine(vector);
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
	float v[] = {
		position.x, position.y, position.z, 0,
		scall.x, scall.y, scall.z, 0,
		rotate.x, rotate.y, rotate.z, 0,
		rotateCenter.x, rotateCenter.y, rotateCenter.z, 0 };
	setObjectConstBuffer(v);
}

bool Graphics::loadTexture(const std::string& file, uint32& width, uint32& height, LPTextureD3D& texture)
{
	return m_pDx11Wrapper->createSRVFromFile(file, &texture, width, height);
}

void Graphics::setObjectConstBuffer(void* pData)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	//gameMathNS::matrixTranspose(pObjectMatrix, pObjectMatrix);
	m_pDx11Wrapper->vsSetConstBuffer(m_pImageMatrixBuf.Get(), pData, 1);
#endif
}

void Graphics::setWorldViewMatrix(Matrix * pWorldViewMatrix)
{
	m_pDx11Wrapper->vsSetConstBuffer(m_pWVMBuf.Get(), pWorldViewMatrix, 0);
}

std::vector<std::string> Graphics::getSupportedResolutionAsString() const
{
	static std::vector<std::string> resolution;
	if (resolution.empty())
	{
		std::vector<DXGI_MODE_DESC> mode = m_pDx11Wrapper->enurmerateAdapterMode();
		for (auto m : mode)
			resolution.push_back(strFormat("%dx%d", m.Width, m.Height));
	}

	return resolution;
}

int32 Graphics::getIndexCurrentResolution() const
{
	auto suppResol = getSupportedResolutionAsString();
	int32 index = std::find(suppResol.begin(), suppResol.end(),
		strFormat("%dx%d", g_pGameSettings->width, g_pGameSettings->height)) - suppResol.begin();
	return index;
}

void Graphics::onResize()
{
	m_pCamera->updatePerspectiveMatrix();
}

void Graphics::setTexture(LPTextureD3D texture)
{
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->SetTexture(0, texture);
#else ifdef _BUILD_WITH_D3D11
	m_pDx11Wrapper->psSetSRV(&texture);
#endif
}