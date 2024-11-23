#include "TimeSystem.h"
#include "Timer.h"
#include "ITime.h"
#include "FPSTimer.h"

static CTimeSystem timeSystem;
CTimeSystem* g_pTimeSystem = &timeSystem;

void CTimeSystem::startup()
{
	registerComponent(g_pFPSTimer);

	g_pTime->startup();

	m_prevTime = ITimer::s_frameTime = g_pTime->getTime();
}

void CTimeSystem::update()
{
	ITimer::s_frameTime = g_pTime->getTime() - m_prevTime;

	subsystemsDo(m_pTimers, update);
}

void CTimeSystem::reset()
{
	subsystemsDo(m_pTimers, reset);
}

void CTimeSystem::perform()
{
}

void CTimeSystem::shutdown()
{
}

void CTimeSystem::onStartGame()
{
}

void CTimeSystem::onQuitGame()
{
}

void CTimeSystem::onPauseGame()
{
}

void CTimeSystem::onResumGame()
{
}

void CTimeSystem::registerComponent(ISystemComponent* pTimer)
{
	subsystemRegister(m_pTimers, dynamic_cast<ITimer*>(pTimer));
}

void CTimeSystem::unregisterComponent(ISystemComponent* pTimer)
{
	subsystemUnregister(m_pTimers, dynamic_cast<ITimer*>(pTimer));
}

void CTimeSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
