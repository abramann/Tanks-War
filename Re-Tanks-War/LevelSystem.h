#pragma once

#include "Subsystem.h"


class CLevelSystem : public ISubsystem
{
public:
	std::string getName() const override { return "LevelSystem"; }

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

extern CLevelSystem* g_pLevelSystem;