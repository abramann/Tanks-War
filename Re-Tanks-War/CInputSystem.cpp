#include "CInputSystem.h"
#include "Input.h"

CInputSystem inputSystem;
CInputSystem* g_pInputSystem;

void CInputSystem::startup()
{
	g_pInput->initialize();
}

void CInputSystem::update()
{
}

void CInputSystem::reset()
{
}

void CInputSystem::perform()
{
}

void CInputSystem::onStartGame()
{
}

void CInputSystem::onQuitGame()
{
}

void CInputSystem::onPauseGame()
{
}

void CInputSystem::onResumGame()
{
}

void CInputSystem::registerComponent(ISystemComponent* pInputComponent)
{
}

void CInputSystem::unregisterComponent(ISystemComponent* pInputComponent)
{
}

void CInputSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}
