#include "PlayersSystem.h"
#include "Player.h"
#include "GameError.h"
#include <algorithm>

static CPlayersSystem playersSystem;
CPlayersSystem* g_pPlayersSystem = &playersSystem;

void CPlayersSystem::startup()
{
}

void CPlayersSystem::update()
{
	std::for_each(m_pPlayers.begin(), m_pPlayers.end(), [](IPlayer* pPlayer) {pPlayer->update(); });
}

void CPlayersSystem::reset()
{
	m_pPlayers.clear();
}

void CPlayersSystem::perform()
{
}

void CPlayersSystem::onStartGame()
{
}

void CPlayersSystem::onQuitGame()
{
}

void CPlayersSystem::onPauseGame()
{
}

void CPlayersSystem::onResumGame()
{
}

void CPlayersSystem::registerComponent(ISystemComponent* pPlayer)
{
#ifdef _TEST
	auto it = findPlayer(static_cast<IPlayer*>(pPlayer));
	CHECK_ERROR(it != m_pPlayers.end(), "Trying to register player twice!");
#endif
	m_pPlayers.push_back(static_cast<IPlayer*>(pPlayer));
}

void CPlayersSystem::unregisterComponent(ISystemComponent* pPlayer)
{
	auto it = findPlayer(static_cast<IPlayer*>(pPlayer));
#ifdef _TEST
	CHECK_ERROR(it == m_pPlayers.end(), "Trying to remove unregistered player!");
#endif
	m_pPlayers.erase(it);
}

void CPlayersSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}

 std::vector<IPlayer*>::iterator& CPlayersSystem::findPlayer(IPlayer* pPlayer)
{
	auto it = std::find_if(m_pPlayers.begin(), m_pPlayers.end(),
		[pPlayer](IPlayer* pExist) {
			return pExist->getComponentID() == pPlayer->getComponentID();
		});
	return it;
}
