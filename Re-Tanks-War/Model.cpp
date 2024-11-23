#include "Model.h"
#include "Renderer.h"
#include "Values.h"
#include "DiskLoader.h"

IModel::~IModel()
{
	//g_pRenderer->releaseBuffer(m_pVertexBuffer);
	//g_pRenderer->releaseBuffer(m_pIndexBuffer);

	//for (auto pTexture : m_pTextures)
	//	g_pRenderer->releaseTexture(pTexture);
}

void IModel::parse(const std::string& modelPath)
{
	DiskLoader::parseModel(this, modelPath);
}

const std::vector<Poly>& IModel::getPolys() const
{
	return m_polys;
}

void IModel::assignPolys(const std::vector<Poly>& quads)
{
	m_polys = quads;
}

void IModel::assignVertexBuffer(IBuffer* pBuffer)
{
	m_pVB = pBuffer;
}

void IModel::assignIndexBuffer(IBuffer* pBuffer)
{
	m_pIB = pBuffer;
}
