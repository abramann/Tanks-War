#pragma once

#include "System.h"

class ISystemComponent;

class CEventSystem : public ISystem
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pComponent) override;
	void unregisterComponent(ISystemComponent* pComponent) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;
};

extern CEventSystem* m_pEventSystem;
