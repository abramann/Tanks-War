#include "GameSystem.h"
#include "Camera.h"
#include "Map.h"
#include "GameError.h"
#include "PlayersSystem.h"
#include "LevelSystem.h"
#include "UISystem.h"
#include "LogicSystem.h"
#include "ViewSystem.h"

std::shared_ptr<CGameSystem> g_pGameSystem;

CGameSystem::CGameSystem()
{
	g_pUISystem = std::make_shared<CUISystem>();
	g_pPlayersSystem = std::make_shared<CPlayersSystem>();
	g_pLevelSystem = std::make_shared<CLevelSystem>();
	g_pLogicSystem = std::make_shared<CLogicSystem>();
	g_pViewSystem = std::make_shared<CViewSystem>();
	g_pMap = std::make_shared<CMap>();
}

CGameSystem::~CGameSystem()
{
}

void CGameSystem::startup()
{
	g_pMap->initialize();
}

void CGameSystem::update()
{
	g_pUISystem->update();
	if (m_runningGame)
	{
		g_pLevelSystem->update();
		g_pLogicSystem->update();
		g_pPlayersSystem->update();
	}
}

void CGameSystem::reset()
{
	g_pLevelSystem->reset();
	g_pLogicSystem->reset();
	g_pPlayersSystem->reset();

	g_pMap->reset();
}

void CGameSystem::onStartGame()
{
	if (m_runningGame)
		reset();

	g_pLevelSystem->startup();
	g_pLogicSystem->startup();
	g_pPlayersSystem->startup();
	m_runningGame = true;
}

void CGameSystem::onQuitGame()
{
	g_pLevelSystem->reset();
	g_pLevelSystem->startup();
	g_pLogicSystem->startup();
	g_pPlayersSystem->startup();
}

void CGameSystem::setMap(std::string map)
{
	m_map = map;
}
