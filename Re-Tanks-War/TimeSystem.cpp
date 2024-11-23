#include "TimeSystem.h"
#include "ITimer.h"

void CTimeSystem::startup()
{
	g_pTimer->startup();
}

void CTimeSystem::update()
{
	g_pTimer->update();
	g_pTimer->getTime();
}

void CTimeSystem::reset()
{
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

void CTimeSystem::registerComponent(ISystemComponent* pRenderComp)
{
}

void CTimeSystem::unregisterComponent(ISystemComponent* pRenderComp)
{
}

void CTimeSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
