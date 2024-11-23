#pragma once

#include "Subsystem.h"
#include <vector>

class IInputStream;

class CInputSystem : public ISubsystem
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void shutdown() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pInputComp) override;
	void unregisterComponent(ISystemComponent* pInputComp) override;
	void handleEvent(ISystemComponent* pInputComp, int eventCode, void* event) override;

private:
	std::vector<IInputStream*> m_streams;
};

extern CInputSystem* g_pInputSystem;
