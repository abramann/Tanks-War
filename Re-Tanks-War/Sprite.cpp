#include "Renderer.h"
#include "ITexture.h"
#include "Sprite.h"


CSprite::CSprite(const wchar_t * texFileName)
{
	m_rotateDegree = 0;
	m_color = Color(255, 255, 255, 255);
	m_position = Vertex(0, 0, 0);
	m_scaling = Vertex(1, 1, 1);
	m_pTexture = g_pRenderer->loadTextureFromFile(texFileName);
	createVertexBuffer();
	createIndexBuffer();	
	g_pRenderer->registerSprite(this);
}

CSprite::~CSprite()
{
	g_pRenderer->releaseTexture(m_pTexture);
	g_pRenderer->releaseBuffer(m_pVertexBuffer);
	g_pRenderer->releaseBuffer(m_pIndexBuffer);
	g_pRenderer->removeSprite(this);
}

void CSprite::setPosition(Vertex position)
{
	m_position = position;
	m_rotateCenter = m_position;
	updateVertexBuffer();
}

Vertex CSprite::getPosition() const
{
	return m_position;
}

void CSprite::setScaling(Vertex newScaling)
{
	m_scaling = newScaling;
	updateVertexBuffer();
}

Vertex CSprite::getScaling() const
{
	return m_scaling;
}

void CSprite::makeRotate(float degree)
{
	m_rotateDegree += degree;
	updateVertexBuffer();
}

void CSprite::setRotate(float newRotate)
{
	m_rotateDegree = newRotate;
	updateVertexBuffer();
}

float CSprite::getRotate() const
{
	return m_rotateDegree;
}

void CSprite::setColor(Color newColor)
{
	m_color = newColor;
}

Color CSprite::getColor() const
{
	return m_color;
}

ITexture* CSprite::getTexture() const
{
	return m_pTexture;
}

IBuffer* CSprite::getVertexBuffer() const
{
	return m_pVertexBuffer;
}

IBuffer* CSprite::getIndexBuffer() const
{
	return m_pIndexBuffer;
}

void CSprite::createVertexBuffer()
{
	Rect pVertices = makeVertexBufferData();
	m_pVertexBuffer = g_pRenderer->createVertexBuffer(4, &pVertices.vert1, IRenderer::BUFFER_WRITE_ONLY);
}

void CSprite::createIndexBuffer()
{
	uint initData[] = { 2, 1, 0,
					2, 0, 3 };

	m_pIndexBuffer = g_pRenderer->createIndexBuffer(sizeof(initData), initData, IRenderer::BUFFER_IMMUTABLE);
}

void CSprite::updateVertexBuffer()
{
	Rect vbData = makeVertexBufferData();
	m_pVertexBuffer->writeData(&vbData.vert1, sizeof(Rect));
}

Rect CSprite::makeVertexBufferData()
{	
	int texWidth = m_pTexture->getWidth();
	int texHeight = m_pTexture->getHeight();

	Rect vbData;
	vbData.vert1 = m_position;
	vbData.vert1.u = 0;
	vbData.vert1.v = 1;
	vbData.vert2 = Vertex(vbData.vert1.x + (texWidth * m_scaling.x), vbData.vert1.y, 0);
	vbData.vert2.u = 1;
	vbData.vert2.v = 1;
	vbData.vert3 = Vertex(vbData.vert2.x, vbData.vert1.y + (texHeight * m_scaling.y), 0);
	vbData.vert3.u = 1; vbData.vert3.v = 0;
	vbData.vert4 = Vertex(vbData.vert1.x, vbData.vert3.y, 0);
	vbData.vert4.v = 0;
	vbData.vert4.u = 0;


	vbData.vert2 = rotateVertex(vbData.vert2, m_rotateDegree, m_rotateCenter);
	vbData.vert3 = rotateVertex(vbData.vert3, m_rotateDegree, m_rotateCenter);
	vbData.vert4 = rotateVertex(vbData.vert4, m_rotateDegree, m_rotateCenter);

	makeUnitSquare(vbData.vert1);
	makeUnitSquare(vbData.vert2);
	makeUnitSquare(vbData.vert3);
	makeUnitSquare(vbData.vert4);

	return vbData;
}
