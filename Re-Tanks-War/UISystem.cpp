#include "UISystem.h"
#include "UI.h"
#include "ImGui/imgui.h"
#include "GameData.h"

std::shared_ptr<CUISystem> g_pUISystem;

CUISystem::CUISystem()
{
}

void CUISystem::startup()
{
}

void CUISystem::update()
{
	for (auto& pUI : m_pRegestredUIs)
	{
		pUI->update();
	}
}

void CUISystem::reset()
{
	for (auto& pUI : m_pRegestredUIs)
	{
		pUI->reset();
	}
}

void CUISystem::perform()
{
	m_pCurrentUI->show();
}

void CUISystem::registerUI(UI* pUI)
{
	m_pRegestredUIs.emplace(pUI);
}

void CUISystem::unregisterUI(UI* pUI)
{
	m_pRegestredUIs.erase(pUI);
}

void CUISystem::activateUI(UI* pUI)
{
	m_pPreviousUI = m_pCurrentUI;
	m_pCurrentUI = pUI;
}

void CUISystem::printMessage(UIMessage* pMessage)
{
}

Font* CUISystem::requestFont(int size)
{
	Font* pFont = m_loadedFonts[size];
	if (pFont != nullptr)
		return pFont;

	ImGuiIO& io = ImGui::GetIO();
	pFont = io.Fonts->AddFontFromFileTTF(fileNS::FONT_PATH, size);
	
	m_loadedFonts[size] = pFont;

	return pFont;
}

bool CUISystem::requestButtonClick()
{
	return false;
}
