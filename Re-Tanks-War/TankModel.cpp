#include "TankModel.h"
#include "IRenderer.h"
#include "GameError.h"


CTankModel::CTankModel()
{
	m_pBodyTex = nullptr;
	m_pGunTex = nullptr;
	m_ppChainsTexs = nullptr;
	m_color = makeColor(255, 255, 255);
	m_scaling = Vertex(1, 1);
	m_bodyRotateDegree = 0;
	m_gunRotateDegree = 0;
}

CTankModel::~CTankModel()
{
	g_pRenderer->releaseTexture(m_pBodyTex);
	g_pRenderer->releaseTexture(m_pGunTex);
	for (int i = 0; i < m_numChainsTexs; i++)
	{
		g_pRenderer->releaseTexture(m_ppChainsTexs[i]);
	}
}

ITexture* const * CTankModel::getChainsTextures(int& num) const
{
	return m_ppChainsTexs;
}

ITexture * CTankModel::getBodyTexture() const
{
	return m_pBodyTex;
}

ITexture * CTankModel::getGunTexture() const
{
	return m_pGunTex;
}

void CTankModel::makeScaling(Vertex scaling)
{
	m_scaling += scaling;
}

void CTankModel::setScaling(Vertex newScaling)
{
	m_scaling = newScaling;
}

Vertex CTankModel::getScaling() const
{
	return m_scaling;
}

void CTankModel::setColor(Color newColor)
{
	m_color = newColor;
}

Color CTankModel::getColor() const
{
	return m_color;
}

float CTankModel::getBodyRotate() const
{
	return m_bodyRotateDegree;
}

void CTankModel::makeBodyRotate(float degree)
{
	m_bodyRotateDegree += degree;;
}

void CTankModel::setBodyRotate(float newRotateDegree)
{
	m_bodyRotateDegree = newRotateDegree;
}

float CTankModel::getGunRotate() const
{
	return m_gunRotateDegree;
}

void CTankModel::makeGunRotate(float degree)
{
	m_gunRotateDegree += degree;
}

void CTankModel::setGunRotate(float newRotateDegree)
{
	m_gunRotateDegree = newRotateDegree;
}

void CTankModel::makeToTargetGunRotate(Vertex target)
{
}

void CTankModel::setBodyRotateCenter(Vertex newPosition)
{
	m_bodyRotateCenterPosition = newPosition;
}

Vertex CTankModel::getBodyRotateCenter() const
{
	return m_bodyRotateCenterPosition;
}

void CTankModel::setGunRotateCenter(Vertex newPosition)
{
	m_gunRotateCenterPosition = newPosition;
}

Vertex CTankModel::getGunRotateCenter() const
{
	return m_gunRotateCenterPosition;
}

void CTankModel::assignBodyTex(ITexture * pBodyTex)
{
	m_pBodyTex = pBodyTex;
}

void CTankModel::assignGunTex(ITexture * pGunTex)
{
	m_pGunTex = pGunTex;
}

void CTankModel::assignChainsTexs(ITexture ** ppChainsTexs, int num)
{
	m_ppChainsTexs = ppChainsTexs;
	m_numChainsTexs = num;
}

void CTankModel::assginVertexBuffer(IBuffer * pVertexBuffer)
{
	m_pVertexBuffer = pVertexBuffer;
}

void CTankModel::assginIndexBuffer(IBuffer * pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}

void CTankModel::createVertexBuffer()
{
}

void CTankModel::createIndexBuffer()
{
}

void CTankModel::updateVertexBuffer()
{
}
