#pragma once
#include "Subsystem.h"
#include <vector>

class IPlayer;

class CPlayersSystem : public ISubsystem
{
public:
	std::string getName() const override { return "PlayersSystem"; }

	// System
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pPlayer) override;
	void unregisterComponent(ISystemComponent* pPlayer) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

private:
	std::vector<IPlayer*>::iterator& findPlayer(IPlayer* pPlayer);
	
	std::vector<IPlayer*> m_pPlayers;
};

extern CPlayersSystem* g_pPlayersSystem;