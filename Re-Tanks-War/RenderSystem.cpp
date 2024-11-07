#include "GameError.h"
#include "RenderComponent.h"
#include "RenderSystem.h"
#include "Renderer.h"

static CRenderSystem renderSystem;
CRenderSystem* g_pRenderSystem = &renderSystem;

CRenderSystem::CRenderSystem() :
	m_renderSprites(false),
	m_renderObjects(false),
	m_renderMap(false),
	m_renderUI(true)
{
}

void CRenderSystem::startup()
{
	g_pRenderer->initialize();

	// Begin the first frame.
	g_pRenderer->newFrame();
}

void CRenderSystem::update()
{
}

void CRenderSystem::reset()
{
}

void CRenderSystem::perform()
{
	std::for_each(m_pRenderedComponents.begin(), m_pRenderedComponents.end(),
		[this](IRenderComponent* pRenderComp)
		{
			auto rtype = getRenderType(pRenderComp);
			switch (rtype)
			{
			case MODEL:
				if (m_renderObjects);
				//g_pRenderer->drawModel(pRenderComp);
				break;
			case SPRITE:
				if (m_renderSprites);
				//g_pRenderer->drawSprite(pRenderComp);
				break;
			default:
				break;
			}
		});

	if (m_renderMap)
		g_pRenderer->renderMap();

	g_pRenderer->renderUI(m_renderUI);

	g_pRenderer->renderFrame();

	// Begin a new frame.
	g_pRenderer->newFrame();
}

void CRenderSystem::onStartGame()
{
	m_renderSprites = true;
	m_renderObjects = true;
	m_renderMap = true;
}

void CRenderSystem::onQuitGame()
{
	m_renderSprites = false;
	m_renderObjects = false;
	m_renderMap = false;
}

void CRenderSystem::onPauseGame()
{
	m_renderMap = false;
	m_renderObjects = false;
	m_renderSprites = false;
	m_renderUI = true;
}

void CRenderSystem::onResumGame()
{
	m_renderMap = true;
	m_renderObjects = true;
	m_renderSprites = true;
	//m_renderUI = false;
}

void CRenderSystem::registerComponent(ISystemComponent* pRenderComp)
{
	subsystemRegister(m_pRenderedComponents, dynamic_cast<IRenderComponent*>(pRenderComp));
}

void CRenderSystem::unregisterComponent(ISystemComponent* pRenderComp)
{
	subsystemUnregister(m_pRenderedComponents, dynamic_cast<IRenderComponent*>(pRenderComp));
}

void CRenderSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}

CRenderSystem::RenderComp CRenderSystem::getRenderType(ISystemComponent* pComponent) const
{
	std::string name = pComponent->getName();

	if (name.compare("Sprite") == 0)
		return SPRITE;
	else if (name.compare("Model") == 0)
		return MODEL;
	else
		REPORT_ERROR("Not render-able component");
}
