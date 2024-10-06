#include "IRenderer.h"
#include "GameError.h"


void IRenderer::beginRendering()
{
}

void IRenderer::renderSprites()
{
	for (auto pSprite : m_pRenderedSprites)
	{
		drawSprite(pSprite);
	}
}

void IRenderer::renderModels()
{
	for (auto pModel : m_pRenderedModels)
	{
		drawModel(pModel);
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

void IRenderer::registerSprite(const CSprite * pSprite)
{
	m_pRenderedSprites.emplace(pSprite);
}

void IRenderer::removeSprite(const CSprite * pSprite)
{
	auto itSprite = m_pRenderedSprites.find(pSprite);
	if (itSprite._Ptr)
	{
		m_pRenderedSprites.erase(itSprite);
	}
	else
	{
		CHECK_ERROR(true, "Trying to remove invalid sprite!");
	}
}

void IRenderer::registerModel(const CModel* pModel)
{
	m_pRenderedModels.emplace(pModel);
}

void IRenderer::removeObject(const CModel* pModel)
{
	auto itMdl = m_pRenderedModels.find(pModel);
	if (itMdl._Ptr)
	{
		m_pRenderedModels.erase(itMdl);
	}
	else
	{
		CHECK_ERROR(true, "Trying to remove invalid model!");
	}
}
