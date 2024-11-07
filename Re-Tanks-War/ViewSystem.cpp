#include "ViewSystem.h"
#include "Camera.h"

static CViewSystem viewSystem;
CViewSystem* g_pViewSystem = &viewSystem;

CViewSystem::CViewSystem()
{
}

void CViewSystem::startup()
{
	g_pCamera->startup();
}

void CViewSystem::update()
{
}

void CViewSystem::reset()
{
}

void CViewSystem::perform()
{
}

void CViewSystem::onStartGame()
{
}

void CViewSystem::onQuitGame()
{
}

void CViewSystem::onPauseGame()
{
}

void CViewSystem::onResumGame()
{
}

void CViewSystem::registerComponent(ISystemComponent* pViewComp)
{
}

void CViewSystem::unregisterComponent(ISystemComponent* pViewComp)
{
}

void CViewSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
