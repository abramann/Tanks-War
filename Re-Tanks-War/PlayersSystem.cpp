#include "PlayersSystem.h"
#include "Player.h"
#include "GameError.h"

std::shared_ptr<CPlayersSystem> g_pPlayersSystem;

void CPlayersSystem::startup()
{
}

void CPlayersSystem::update()
{
}

void CPlayersSystem::reset()
{
	m_pPlayers.clear();
}

void CPlayersSystem::registerPlayer(IPlayer* pPlayer)
{
	int id = pPlayer->getID();
	CHECK_ERROR(hasPlayerRegistered(pPlayer), "Trying to register player twice!");
	m_pPlayers.emplace(pPlayer);
}

void CPlayersSystem::removePlayer(IPlayer* pPlayer)
{
	int id = pPlayer->getID();
	CHECK_ERROR(!hasPlayerRegistered(pPlayer), "Trying to remove unregistred player!");
	auto itpPlayer = m_pPlayers.find(pPlayer);
	m_pPlayers.erase(itpPlayer);
}

bool CPlayersSystem::hasPlayerRegistered(IPlayer* pPlayer)
{
	int id = pPlayer->getID();
	for (auto pPlayer : m_pPlayers)
	{
		if (pPlayer->getID() == id)
		{
			return true;
		}
	}
	return false;
}
