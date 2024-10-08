#include "UISystem.h"

std::shared_ptr<CUISystem> g_pUISystem;

CUISystem::CUISystem()
{
}

void CUISystem::startup()
{
}

void CUISystem::update()
{
	if (m_pCurrentUI)
	{
		m_pCurrentUI = m_pCurrentUI->show();
	}
}

void CUISystem::reset()
{
}

void CUISystem::registerUI(UI* pUI)
{
}

void CUISystem::unregisterUI(UI* pUI)
{
}

void CUISystem::activateUI(UI* pUI)
{
}

void CUISystem::printMessage(UIMessage* pMessage)
{
}
