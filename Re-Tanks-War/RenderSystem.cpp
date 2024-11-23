#include "GameError.h"
#include "RenderComponent.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Model.h"
#include "ImGui/imgui_impl_dx11.h"
#include "WorldSystem.h"

static CRenderSystem renderSystem;
CRenderSystem* g_pRenderSystem = &renderSystem;

CRenderSystem::CRenderSystem() :
	m_doRenderSprites(false),
	m_doRenderModels(false),
	m_doRenderMap(false),
	m_doRenderUI(true)
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
				if (m_doRenderModels)
					renderModel(dynamic_cast<IModel*>(pRenderComp));
				break;
			case SPRITE:
				if (m_doRenderSprites)
					renderSprite(dynamic_cast<CSprite*>(pRenderComp));
				break;
			default:
				break;
			}
		});

	if (m_doRenderMap)
		renderWorld();

	renderUI(m_doRenderUI);

	g_pRenderer->renderFrame();

	// Begin a new frame.
	g_pRenderer->newFrame();
}

void CRenderSystem::shutdown()
{
}

void CRenderSystem::onStartGame()
{
	m_doRenderSprites = true;
	m_doRenderModels = true;
	m_doRenderMap = true;
}

void CRenderSystem::onQuitGame()
{
	m_doRenderSprites = false;
	m_doRenderModels = false;
	m_doRenderMap = false;
}

void CRenderSystem::onPauseGame()
{
	m_doRenderMap = false;
	m_doRenderModels = false;
	m_doRenderSprites = false;
	m_doRenderUI = true;
}

void CRenderSystem::onResumGame()
{
	m_doRenderMap = true;
	m_doRenderModels = true;
	m_doRenderSprites = true;
	m_doRenderUI = false;
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

void CRenderSystem::setRenderState(RenderType renderType, bool state)
{
	switch (renderType)
	{
	case MODEL:
		m_doRenderModels = state;
		break;
	case SPRITE:
		m_doRenderSprites = state;
		break;
	case WORLD:
		m_doRenderMap = state;
		break;
	case UI:
		m_doRenderUI = state;
		break;
	default:
		REPORT_ERROR("Invalid state");
	}
}

void CRenderSystem::renderSprite(CSprite* pSprite)
{
	g_pRenderer->drawSprite(pSprite);
}

void CRenderSystem::renderModel(IModel* pModel)
{
	g_pRenderer->drawModel(pModel);
}

void CRenderSystem::renderUI(bool state)
{
	ImGui::EndFrame();

	if (state)
	{
		g_pRenderer->renderUI();
	}
}

void CRenderSystem::renderWorld()
{
	g_pRenderer->drawMesh(g_pWorldSystem->getWorldMesh());
}

CRenderSystem::RenderType CRenderSystem::getRenderType(ISystemComponent* pRenderComponent) const
{
	std::string name = pRenderComponent->getName();

	if (name.compare("Sprite") == 0)
		return SPRITE;
	else if (name.compare("Model") == 0)
		return MODEL;
	else
		REPORT_ERROR("Not render-able component");
}
