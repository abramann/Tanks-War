#include "DxBuffer.h"
#include "DxRenderer.h"


CDxBuffer::CDxBuffer() : m_pDXOBuffer(nullptr)
{
}


CDxBuffer::~CDxBuffer()
{
	m_pDXOBuffer->Release();
}

void CDxBuffer::readData(Vertex * pData, uint32_t size)
{
	g_pDxRenderer->copyResource(pData, m_pDXOBuffer, size);
}

void CDxBuffer::writeData(Vertex * pData, uint32_t size)
{
	g_pDxRenderer->copyToResource(m_pDXOBuffer, pData, size);
}

void* CDxBuffer::getBufferObject()
{
	return static_cast<void*>(m_pDXOBuffer);
}

void CDxBuffer::assign(ID3D11Buffer * pDXOBuffer)
{
	m_pDXOBuffer = pDXOBuffer;
}
