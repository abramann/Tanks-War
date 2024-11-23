#include "IntroUI.h"
#include "Image.h"

static CIntroUI introUI;
UI* g_pIntroUI = &introUI;

static CImage devLogo;

void CIntroUI::startup()
{
	registerComponent(&devLogo);
	setupProperties();
}

void CIntroUI::update()
{
}

void CIntroUI::reset()
{
}

void CIntroUI::handleEvent(ISystemComponent* pWidget, int eventCode, void* event)
{
}

void CIntroUI::beginUI()
{
}

void CIntroUI::endUI()
{
}

void CIntroUI::setupProperties()
{
}
