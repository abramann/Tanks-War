#pragma once
#include "System.h"
#include <memory>
#include <set>

class IPlayer;

class CPlayersSystem : public ISystem
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;

	void registerPlayer(IPlayer* pPlayer);
	void removePlayer(IPlayer* pPlayer);
	bool hasPlayerRegistered(IPlayer* pPlayer);

private:
	std::set<IPlayer*> m_pPlayers;
};

extern std::shared_ptr<CPlayersSystem> g_pPlayersSystem;