#include "UISystem.h"
#include "StartUI.h"
#include "Image.h"
#include "Renderer.h"
#include "PlayUI.h"

static CStartUI startui;
UI* g_pStartUI = &startui;

// CStartUI components
static CImage uiGameLogo;
CImage* g_pUIGameLogo = &uiGameLogo;

static CButton playBut;
static CButton optionBut;
static CButton aboutBut;
static CButton exitBut;

void CStartUI::startup()
{
	m_pWidgets.reserve(5);

	registerComponent(&playBut);
	registerComponent(&optionBut);
	registerComponent(&aboutBut);
	registerComponent(&exitBut); 
	registerComponent(g_pUIGameLogo);

	setupProperties();
}

void CStartUI::update()
{
	subsystemsDo(m_pWidgets, update);
}

void CStartUI::reset()
{
	subsystemsDo(m_pWidgets, reset);

	m_pWidgets.clear();

	startup();
}

void CStartUI::handleEvent(ISystemComponent* pWidget, int eventCode, void* event)
{
	if (pWidget == &playBut)
	{
		m_pSystemHandler->handleEvent(this, CUISystem::SWITCH_UI, g_pPlayUI);
	}
	else if (pWidget == &optionBut)
	{
		//g_pUISystem->switchUI(g_pOptionsUI);
	}
	else if (pWidget == &aboutBut)
	{
		//g_pUISystem->switchUI(g_pAboutUI);
	}
	else if (pWidget == &exitBut)
	{
		// TODO
	}
}

void CStartUI::beginUI()
{
	//Font* pFont = getFont(s_fontSize);
	//ImGui::PushFont(pFont);

	int bbw = g_pRenderer->getBackbufferWidth();
	int bbh = g_pRenderer->getBackbufferHeight();

	// Filling window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(bbw, bbh));
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
	ImGui::Begin("StartUI", NULL, ImGuiWindow_FillScreen);
	//ImGui::PopStyleColor();
}

void CStartUI::endUI()
{
	ImGui::End();
	//ImGui::PopFont();
}

void CStartUI::setupProperties()
{
	s_butsSize.x = 20;
	s_butsSize.y = 10;
	g_pRenderer->getRelativeScreenPoint(s_butsSize.x, s_butsSize.y);

	s_butsBeginPos.x = 40;
	s_butsBeginPos.y = 20;
	g_pRenderer->getRelativeScreenPoint(s_butsBeginPos.x, s_butsBeginPos.y);

	size_t i = 0;
	for (auto& pWidget : m_pWidgets)
	{
		if (pWidget->getName() == "Image")
		{

		}
		else
		{
			const std::string butsText[] = { "Play", "Options", "About", "Exit" };
			const int butsPadding = s_butsSize.y * 1.5f;

			CButton* pButton = dynamic_cast<CButton*>(pWidget);

			pButton->setText(butsText[i]);
			pButton->setPosition(s_butsBeginPos.x, s_butsBeginPos.y + butsPadding * i);
			pButton->setSize(s_butsSize);
			pButton->setSystemHandler(this);
			i++;
		}
	}
}
