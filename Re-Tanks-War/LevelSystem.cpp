#include "LevelSystem.h"

CLevelSystem levelSystem;
CLevelSystem* g_pLevelSystem = &levelSystem;

void CLevelSystem::startup()
{
}

void CLevelSystem::update()
{
}

void CLevelSystem::reset()
{
}

void CLevelSystem::perform()
{
}

void CLevelSystem::onStartGame()
{
}

void CLevelSystem::onQuitGame()
{
}

void CLevelSystem::onPauseGame()
{
}

void CLevelSystem::onResumGame()
{
}

void CLevelSystem::registerComponent(ISystemComponent* pComponent)
{
}

void CLevelSystem::unregisterComponent(ISystemComponent* pComponent)
{
}

void CLevelSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
