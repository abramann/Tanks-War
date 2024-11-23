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

void CUISystem::shutdown()
{
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

void CUISystem::setupStyles()
{
	auto& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	ImGui::StyleColorsDark();

	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Rounding
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.FramePadding = ImVec2(8.0f, 5.0f);
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
}

