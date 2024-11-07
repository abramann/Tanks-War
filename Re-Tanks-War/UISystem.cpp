#include "UISystem.h"
#include "UI.h"
#include "Values.h"
#include "StartUI.h"

static CUISystem uiSystem;
CUISystem* g_pUISystem = &uiSystem;

CUISystem::CUISystem() :
	m_pCurrentUI(nullptr),
	m_pPreviousUI(nullptr)
{
}

void CUISystem::startup()
{
	registerComponent(g_pStartUI);

	subsystemsStartup<UI>(this, m_pUIs);

	switchUI(g_pStartUI);
}

void CUISystem::update()
{
	doSystem(m_pCurrentUI, update);
}

void CUISystem::reset()
{
	systemsDo(m_pUIs, reset);
}

void CUISystem::perform()
{
	m_pCurrentUI->perform();
}

void CUISystem::onStartGame()
{
	// m_pCurrentUI = &ingameUI;
}

void CUISystem::onQuitGame()
{
	//m_pCurrentUI = &quitgameUI;
}

void CUISystem::onPauseGame()
{
}

void CUISystem::onResumGame()
{
}

void CUISystem::registerComponent(ISystemComponent* pUIComp)
{
	subsystemRegister(m_pUIs, dynamic_cast<UI*>(pUIComp));
}

void CUISystem::unregisterComponent(ISystemComponent* pUIComp)
{
	subsystemUnregister(m_pUIs, dynamic_cast<UI*>(pUIComp));
}

void CUISystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}

void CUISystem::switchUI(UI* pUI)
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
	pFont = io.Fonts->AddFontFromFileTTF(values::FONT_PATH, size);
	
	m_loadedFonts[size] = pFont;

	return pFont;
}
