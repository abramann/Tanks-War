// Author: Wefaq
// Brief: CLogicSystem: handle all world interactions
#pragma once

#include "Subsystem.h"
#include "Types.h"
#include <vector>

class IEvent;

class CLogicSystem : public ISubsystem
{
public:
	enum EventType
	{
		// Entity events
		ATTACK,
		MOVE,
		DESTROY,
		HEAL,
		RESET,
		FREE,
	};

	std::string getName() const override { return "LogicSystem"; }
	
	CLogicSystem();
	~CLogicSystem();
	
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void shutdown() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pObject) override;
	void unregisterComponent(ISystemComponent* pObject) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

private:
	std::vector<IEvent*> m_pEvents;
};

extern CLogicSystem* g_pLogicSystem;
