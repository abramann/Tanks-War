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
	CHECK_ERROR(pSprite == nullptr, "Registring nullptr!");
	m_pRenderedSprites.emplace(pSprite);
}

void IRenderer::removeSprite(const CSprite * pSprite)
{
	auto itSprite = m_pRenderedSprites.find(pSprite);
	CHECK_ERROR(itSprite._Ptr == nullptr, "Trying to remove invalid sprite!");
	m_pRenderedSprites.erase(itSprite);
}

void IRenderer::registerModel(const CModel* pModel)
{
	CHECK_ERROR(pModel == nullptr , "Registring nullptr!");
	m_pRenderedModels.emplace(pModel);
}

void IRenderer::removeObject(const CModel* pModel)
{
	auto itMdl = m_pRenderedModels.find(pModel);
	CHECK_ERROR(itMdl._Ptr == nullptr, "Trying to remove invalid model!");
	m_pRenderedModels.erase(itMdl);
}
