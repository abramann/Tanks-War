#include "Line.h"
#include "IRenderer.h"
#include "IBuffer.h"
#include "GameError.h"
#include <algorithm>


CLine::CLine() : IImage::IImage()
{
	m_thickness = DEFAULT_THICKNESS;
	g_pRenderer->registerImage(this);
}

CLine::CLine(Vertex begin, Vertex end) : IImage::IImage()
{
	m_thickness = DEFAULT_THICKNESS;
	createVertexBuffer();
	createIndexBuffer();
	g_pRenderer->registerImage(this);
}

CLine::~CLine()
{
	IImage::~IImage();
	g_pRenderer->removeImage(this);
}

void CLine::setBegin(Vertex newBegin)
{
	m_begin = newBegin;
	updateVertexBuffer();
}

void CLine::setEnd(Vertex newEnd)
{
	m_end = newEnd;
	updateVertexBuffer();
}

void CLine::setBeginEnd(Vertex newBegin, Vertex newEnd)
{
	m_begin = newBegin;
	m_end = newEnd;
	updateVertexBuffer();
}

Vertex CLine::getBegin() const
{
	return m_begin;
}

Vertex CLine::getEnd() const
{
	return m_end;
}

void CLine::setThickness(float newThickness)
{
	m_thickness = newThickness;
	updateVertexBuffer();
}

float CLine::getThickness() const
{
	return m_thickness;
}

void CLine::createVertexBuffer()
{
	Rect rect = makeRect();
	Rect8 vbData = makeVertexBufferData(rect);
	m_pVertexBuffer = g_pRenderer->createVertexBuffer(sizeof(vbData), &vbData.vert1, IRenderer::BUFFER_WRITE_ONLY);
}

void CLine::createIndexBuffer()
{
	int index[] = { 0, 1, 2,
					2, 3, 0 };
	m_pIndexBuffer = g_pRenderer->createIndexBuffer(ARRAYSIZE(index)* sizeof(int), index, IRenderer::BUFFER_IMMUTABLE);
}

void CLine::updateVertexBuffer()
{
	if (!m_pIndexBuffer)
	{
		createIndexBuffer();
	}
	if (!m_pVertexBuffer)
	{
		createVertexBuffer();
	}
	else
	{
		Rect rect = makeRect();
		Rect8 vbData = makeVertexBufferData(rect);
		m_pVertexBuffer->writeData(&vbData.vert1, sizeof(vbData));
	}
}

Rect8 CLine::makeVertexBufferData(Rect rect)
{
	Rect8 rect8;
	setNull(rect8);
	rect8.vert1 = rect.vert1;
	rect8.vert3 = rect.vert2;
	rect8.vert5 = rect.vert3;
	rect8.vert7 = rect.vert4;
	return rect8;
}

Rect CLine::makeRect()
{
	Vector2D u = makeVector2D(m_begin, m_end);
	Vector2D v = u.perpendicular();;
	Vector2D vNormalization = v.normalization();

	Rect rect;
	rect.vert1 = m_end + (vNormalization * m_thickness);
	rect.vert2 = m_begin + (vNormalization * m_thickness);
	rect.vert3 = m_begin - (vNormalization * m_thickness);
	rect.vert4 = m_end - (vNormalization * m_thickness);
	if (!m_relativeCamera)
	{
		makeUnitSquare(rect.vert1);
		makeUnitSquare(rect.vert2);
		makeUnitSquare(rect.vert3);
		makeUnitSquare(rect.vert4);
	}
	//makeClockwiseOrder(&rect.vert1, 4);

	return rect;
}

static Vertex origin;

int compareClockwiseOrder(const void* pVert1, const void* pVert2)
{
	Vertex& vert1 = *(Vertex*)pVert1;
	Vertex& vert2 = *(Vertex*)pVert2;

	if (vert1 == vert2)
		return 0;

	Vertex firstOffset = vert1 - origin;
	Vertex secondOffset = vert2 - origin;
	float angle1 = atan2(firstOffset.y, firstOffset.x);
	float angle2 = atan2(secondOffset.y, secondOffset.x);

	if (angle1 < angle2)
		return -1;
	if (angle1 > angle2)
		return 1;

	// Check to see which point is closest
	return firstOffset.distance(origin) < secondOffset.distance(origin) ? -1 : 1;
}

void CLine::makeClockwiseOrder(Vertex* pVertices, int numVertices)
{
	float maxX = -FLT_MAX;
	float minX = FLT_MAX;
	float maxY = -FLT_MAX;
	float minY = FLT_MAX;

	for (int i = 0; i < numVertices; i++)
	{
		maxX = max(maxX, pVertices[i].x);
		minX = min(minX, pVertices[i].x);
		maxY = max(maxY, pVertices[i].y);
		minY = min(minY, pVertices[i].y);
	}
	origin.x = minX + (maxX - minX) / 2.0f;
	origin.y = minY + (maxY - minY) / 2.0f;

	qsort((void*)pVertices, numVertices, sizeof(Vertex), compareClockwiseOrder);
}
