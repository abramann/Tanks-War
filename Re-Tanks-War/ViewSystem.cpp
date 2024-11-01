#include "ViewSystem.h"
#include "Camera.h"

std::shared_ptr<CViewSystem> g_pViewSystem;

CViewSystem::CViewSystem()
{
	g_pCamera = std::make_shared<CCamera>();
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
