#include "IRenderer.h"
#include "GameError.h"


void IRenderer::beginRendering()
{
	for (auto pImage : m_pRenderedImages)
	{
		drawImage(pImage);
	}
	for (auto pTankModel : m_pRenderedTanks)
	{
		drawTank(pTankModel);
	}
}

int IRenderer::getBackbufferWidth() const
{
	return m_backbufferWidth;
}

int IRenderer::getBackbufferHeight() const
{
	return m_backbufferHeight;
}

void IRenderer::registerImage(const IImage * pImage)
{
	m_pRenderedImages.emplace(pImage);
}

void IRenderer::removeImage(const IImage * pImage)
{
	auto itImg = m_pRenderedImages.find(pImage);
	if (itImg._Ptr)
	{
		m_pRenderedImages.erase(itImg);
	}
	else
	{
		CHECK_ERROR(true, "Trying to remove invalid image!");
	}
}

void IRenderer::registerTank(const CTankModel* pTankModel)
{
	m_pRenderedTanks.emplace(pTankModel);
}

void IRenderer::removeTank(const CTankModel* pTankModel)
{
	auto itImg = m_pRenderedTanks.find(pTankModel);
	if (itImg._Ptr)
	{
		m_pRenderedTanks.erase(itImg);
	}
	else
	{
		CHECK_ERROR(true, "Trying to remove invalid image!");
	}
}
