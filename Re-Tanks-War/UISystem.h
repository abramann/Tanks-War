#pragma once

#include "System.h"
#include "UI.h"
#include "Types.h"
#include "ImGui/imgui.h"
#include <string>
#include <memory>
#include <map>
#include <set>


typedef ImFont Font;

struct UIMessage
{
	std::string text;
	uchar r, g, b, a;
	float period;
};

class UI;

class CUISystem : public ISystem
{
public:
	CUISystem();

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;

	void registerUI(UI* pUI);
	void unregisterUI(UI* pUI);
	void activateUI(UI* pUI);
	void printMessage(UIMessage* pMessage);
	Font* requestFont(int size);
	bool requestButtonClick();

private:
	Font* createFont();

	UI* m_pCurrentUI;
	UI* m_pPreviousUI;
	std::map<int, Font*> m_loadedFonts;
	std::set<UI*> m_pRegestredUIs;
};

extern std::shared_ptr<CUISystem> g_pUISystem;
