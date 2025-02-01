#include "GameSystem.h"
#include "PlayersSystem.h"
#include "LevelSystem.h"
#include "UISystem.h"
#include "LogicSystem.h"
#include "ViewSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"

static CGameSystem gameSystem;
CGameSystem* g_pGameSystem = &gameSystem;

CGameSystem::CGameSystem()
{
}

CGameSystem::~CGameSystem()
{
}

void CGameSystem::startup()
{
	registerComponent(g_pRenderSystem);
	registerComponent(g_pInputSystem);
	registerComponent(g_pUISystem);
	registerComponent(g_pPlayersSystem);
	registerComponent(g_pLevelSystem);
	registerComponent(g_pLogicSystem);
	registerComponent(g_pViewSystem);
	registerComponent(g_pTimeSystem);

	// Testing
	g_pRenderSystem->setRenderState(CRenderSystem::RenderType::UI, true);
	//g_pRenderSystem->setRenderState(CRenderSystem::RenderType::WORLD, true);
	//g_pRenderSystem->setRenderState(CRenderSystem::RenderType::MODEL, true);

	subsystemsDo(m_pSubSystems, startup);
	subsystemsDo2(m_pSubSystems, setSystemHandler, this);
}

void CGameSystem::update()
{
	systemsDo(m_pSubSystems, update);
}

void CGameSystem::reset()
{
	systemsDo(m_pSubSystems, reset);
}

void CGameSystem::perform()
{
	subsystemsDo(m_pSubSystems, perform);
}

void CGameSystem::shutdown()
{
	subsystemsDo(m_pSubSystems, shutdown);
}

void CGameSystem::onStartGame()
{
	subsystemsDo(m_pSubSystems, onStartGame);
}

void CGameSystem::onQuitGame()
{
	subsystemsDo(m_pSubSystems, onQuitGame);
}

void CGameSystem::onPauseGame()
{
	subsystemsDo(m_pSubSystems, onPauseGame);
}

void CGameSystem::onResumGame()
{
	subsystemsDo(m_pSubSystems, onResumGame);
}

void CGameSystem::registerComponent(ISystemComponent* pSubSystem)
{
	subsystemRegister(m_pSubSystems, dynamic_cast<ISubsystem*>(pSubSystem));
}

void CGameSystem::unregisterComponent(ISystemComponent* pSubSystem)
{
	subsystemUnregister(m_pSubSystems, dynamic_cast<ISubsystem*>(pSubSystem));
}

void CGameSystem::handleEvent(ISystemComponent* pSubSystem, int eventCode, void* event)
{
}

void CGameSystem::run()
{
	update();

	perform();
}

