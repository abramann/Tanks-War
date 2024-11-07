#include "GameEvents.h"

CEventSystem eventSystem;
CEventSystem* m_pEventSystem = &eventSystem;

void CEventSystem::startup()
{
}

void CEventSystem::update()
{
}

void CEventSystem::reset()
{
}

void CEventSystem::perform()
{
}

void CEventSystem::onStartGame()
{
}

void CEventSystem::onQuitGame()
{
}

void CEventSystem::onPauseGame()
{
}

void CEventSystem::onResumGame()
{
}

void CEventSystem::registerComponent(ISystemComponent* pComponent)
{
}

void CEventSystem::unregisterComponent(ISystemComponent* pComponent)
{
}

void CEventSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
