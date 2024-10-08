#pragma once

#include "System.h"
#include <string>
#include <memory>
#include <set>

class IPlayer;

class CGameSystem : ISystem
{
public:
	CGameSystem();
	~CGameSystem();

	void startup() override;
	void update() override;
	void reset() override;

	void onStartGame();
	void onQuitGame();

	void setMap(std::string map);
	
private:
	std::string m_map;
	bool m_runningGame;
};

extern std::shared_ptr<CGameSystem> g_pGameSystem;
