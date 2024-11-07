#pragma once

#include "Subsystem.h"

class CViewSystem : public ISubsystem
{
public:
	std::string getName() const override { return "ViewSystem"; }

	CViewSystem();

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pViewComp) override;
	void unregisterComponent(ISystemComponent* pViewComp) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;
};

extern CViewSystem* g_pViewSystem;

