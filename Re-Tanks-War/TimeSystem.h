#pragma once

#include "Subsystem.h"

class ITimer;

class CTimeSystem : public ISubsystem
{
public:
	std::string getName() const override { return "TimeSystem"; }

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void shutdown() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pRenderComp) override;
	void unregisterComponent(ISystemComponent* pRenderComp) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

private:
	std::vector<ITimer*> m_pTimers;
	int64_t m_prevTime;
};

extern CTimeSystem* g_pTimeSystem;