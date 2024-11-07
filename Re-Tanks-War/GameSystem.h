// Brief GameSystem: the end system of systems.
#pragma once

#include "System.h"
#include <string>
#include <vector>

class ISubsystem;

class CGameSystem : public ISystem
{
public:
	CGameSystem();
	~CGameSystem();

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pSystem) override;
	void unregisterComponent(ISystemComponent* pSystem) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

	void run();
	//void setMap(std::string map); Move to worldsystem
	
private:
	std::vector<ISubsystem*> m_pSubSystems;
	//std::string m_map; Move to worldsystem
	bool m_runningGame;
};

extern CGameSystem* g_pGameSystem;
