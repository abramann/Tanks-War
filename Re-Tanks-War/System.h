// Brief: System: the root interface reacts with SystemComponents 
#pragma once

class ISystemComponent;

class ISystem
{
public:
	virtual void startup() = 0;
	virtual void update() = 0;
	virtual void reset() = 0;
	virtual void perform() = 0;
	virtual void shutdown() = 0;
	virtual void onStartGame() = 0;
	virtual void onQuitGame() = 0;
	virtual void onPauseGame() = 0;
	virtual void onResumGame() = 0;
	virtual void registerComponent(ISystemComponent* pComponent) = 0;
	virtual void unregisterComponent(ISystemComponent* pComponent) = 0;
	virtual void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) = 0;
};

#define doSystem(pSystem, act) \
	ISystem* pSys = pSystem; \
	pSys->##act();

#define systemsDo(pSystems, act) \
	 std::for_each(pSystems.begin(), pSystems.end(), [](ISystem* pSystem) { pSystem->##act(); });
