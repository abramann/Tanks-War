#include "IObject2D.h"
#include "IRenderer.h"


IObject2D::IObject2D()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}

IObject2D::~IObject2D()
{
	g_pRenderer->releaseBuffer(m_pVertexBuffer);
	g_pRenderer->releaseBuffer(m_pIndexBuffer);
}

void IObject2D::setPosition(Vertex newPosition)
{
	m_position = newPosition;
}

Vertex IObject2D::getPosition() const
{
	return m_position;
}

IBuffer * IObject2D::getVertexBuffer() const
{
	return m_pVertexBuffer;
}

IBuffer * IObject2D::getIndexBuffer() const
{
	return m_pIndexBuffer;
}
