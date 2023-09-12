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
#include "inlined.inl"
#include "dx11wrapper.h"

using Microsoft::WRL::ComPtr;

uint64  g_frameCounter = 0;

Graphics::Graphics()
{
	m_pCamera = std::make_shared<Camera>();
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
	m_pWVMBuffer = m_pDx11Wrapper->createBuffer(D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, sizeof(Matrix));
	m_pObjectMatrixBuffer = m_pDx11Wrapper->createBuffer(D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, sizeof(float[3]) * 3 + 12); // 36 + 12 = 48 size should be of twice 16
	return true;
}

void Graphics::beginRender()
{
	m_pDx11Wrapper->d3dBegin();
}

void Graphics::drawImage(const Image* image)
{
	int16 vertices = image->getVertices();
	DxBuffer* pVB = image->getVertexBuffer();
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
	m_pDx11Wrapper->streamVertexBuffer(pVB);
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
	/*Matrix rot = gameMathNS::V3ToMatrix(rotate, MATRIX_TYPE_ROTATE),
		rotCent = gameMathNS::V3ToMatrix(rotateCenter, MATRIX_TYPE_TRANSLATE),
		rotCentN = gameMathNS::V3ToMatrix(rotateCenter * -1, MATRIX_TYPE_TRANSLATE),
		scal = gameMathNS::V3ToMatrix(scall, MATRIX_TYPE_SCALL),
		pos = gameMathNS::V3ToMatrix(position, MATRIX_TYPE_TRANSLATE);
	Matrix prop;
	prop = scal*rot*pos;
	//prop = scal*rotCentN*rot*rotCent*pos; // to center rotate
	//prop *= m_wvp;
	prop.m[0][0];
	setObjectMatrix(&prop);*/
	float v[] = { position.x, position.y, position.z
		, scall.x, scall.y, scall.z,
		rotate.x, rotate.y, rotate.z, 0.0f, 0.0f, 0.0f };
	setObjectConstBuffer(v);
}

bool Graphics::loadTexture(const std::string& file, int32& width, int32& height, LPTextureD3D& texture)
{
	return m_pDx11Wrapper->createSRVFromFile(file, texture, width, height);
}

void Graphics::setObjectConstBuffer(void* pData)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	//gameMathNS::matrixTranspose(pObjectMatrix, pObjectMatrix);
	m_pDx11Wrapper->vsSetConstBuffer(m_pObjectMatrixBuffer.Get(), pData, 1);
#endif
}

void Graphics::setWorldViewMatrix(Matrix * pWorldViewMatrix)
{
	//gameMathNS::matrixTranspose(pWorldViewMatrix, pWorldViewMatrix);
	m_pDx11Wrapper->vsSetConstBuffer(m_pWVMBuffer.Get(), pWorldViewMatrix, 0);
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
	m_pDx11Wrapper->psSetSRV(texture);
#endif
}