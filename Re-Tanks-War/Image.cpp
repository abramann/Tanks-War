#include "Image.h"
#include "IRenderer.h"
#include "ITexture.h"


CImage::CImage(const wchar_t * texFileName)
{
	m_scaling = Vertex(1, 1);
	m_color = makeColor(255, 255, 255);
	m_relativeCamera = false; 
	m_pTexture = g_pRenderer->loadTextureFromFile(texFileName);
	createVertexBuffer();
	createIndexBuffer();	
	g_pRenderer->registerImage(this);
}

CImage::~CImage()
{
	IImage::~IImage();
	g_pRenderer->removeImage(this);
}

void CImage::createVertexBuffer()
{
	Rect8 pInitData = makeVertexBufferData();
	m_pVertexBuffer = g_pRenderer->createVertexBuffer(8, &pInitData.vert1, IRenderer::BUFFER_WRITE_ONLY);
}

void CImage::createIndexBuffer()
{
	int initData[] = { 2, 1, 0,
					2, 0, 3 };

	m_pIndexBuffer = g_pRenderer->createIndexBuffer(sizeof(initData), initData, IRenderer::BUFFER_IMMUTABLE);
}

void CImage::updateVertexBuffer()
{
	Rect8 vbData = makeVertexBufferData();
	m_pVertexBuffer->writeData(&vbData.vert1, sizeof(Rect8));
}

Rect8 CImage::makeVertexBufferData()
{
	int texWidth = m_pTexture->getWidth();
	int texHeight = m_pTexture->getHeight();

	Rect8 vbData;
	vbData.vert1 = m_position;
	vbData.vert3 = Vertex((vbData.vert1.x + texWidth), vbData.vert1.y);
	vbData.vert5 = Vertex(vbData.vert3.x, vbData.vert1.y + texHeight);
	vbData.vert7 = Vertex(vbData.vert1.x, vbData.vert5.y); 
	if (!m_relativeCamera)
	{
		makeUnitSquare(vbData.vert1);
		makeUnitSquare(vbData.vert3);
		makeUnitSquare(vbData.vert5);
		makeUnitSquare(vbData.vert7);
	}
	vbData.vert2 = Vertex(0, 0);
	vbData.vert4 = Vertex(1, 0);
	vbData.vert6 = Vertex(1, 1);
	vbData.vert8 = Vertex(0, 1);
	return vbData;
}
