#include "UISystem.h"
#include "StartUI.h"
#include "Renderer.h"
#include "ImGui/imgui.h"

void CStartUI::update()
{
}

void CStartUI::reset()
{
}

void CStartUI::beginUI()
{
	int bbw = g_pRenderer->getBackbufferWidth();
	int bbh = g_pRenderer->getBackbufferHeight();

	// Filling window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(bbw, bbh));

	ImGui::Begin("StartUI", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	Font* pFont = g_pUISystem->requestFont(32);
	ImGui::PushFont(pFont);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 5);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);
	ImGui::BeginChild("ChildStartUI", ImVec2(bbw / 2, bbh / 1.5f), true, ImGuiWindowFlags_NoTitleBar);
	ImGui::PopStyleVar(2);
}

void CStartUI::drawUI()
{
	/*int bbw = g_pRenderer->getBackbufferWidth();
	int bbh = g_pRenderer->getBackbufferHeight();

	ImVec2 butSize = ImVec2(bbw / 2.0f, bbh / 5.0f);
	ImVec2 butPos = ImVec2((bbw / 2.0f) - butSize.x / 2.0f, bbh / 10.0f);
	SetCursorPos(butPos.x, butPos.y);
#ifdef _CLIENT_BUILD
	if (button("Play", butSize))
		setActivity(PLAYMODE_ACTIVITY);
#else
	if (button("Multiplayer", butSize))
		setActivity(MULTIPLAYER_ACTIVITY);
#endif

	butPos.y += butSize.y + butSize.y * MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Settings", butSize))
		setActivity(SETTINGS_ACTIVITY);

	butPos.y += butSize.y + butSize.y * MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Quit", butSize))
		setActivity(QUIT_ACTIVITY);

	PushFont(m_pFont[FONTSIZE_MED]);
	SetCursorPos(Vec2(0, g_pGameSettings->height * 0.9));
	if (button("About", Vec2(butSize.x / 4, butSize.y / 4)))
		setActivity(ABOUT_ACTIVITY);

	PopFont();
	endActivity();
	return nullptr;
	*/
}

void CStartUI::endUI()
{
}
