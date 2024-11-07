#pragma once

#include "Subsystem.h"
#include "UI.h"
#include "Types.h"
#include "String.h"
#include "ImGui/imgui.h"
#include <map>


typedef ImFont Font;

struct UIMessage
{
	std::string text;
	Color color;
	float liveSeconds;
};

class UI;

class CUISystem : public ISubsystem
{
public:
	enum Events
	{
		SWITCH_UI,
	};

	std::string getName() const override { return "UISystem"; }
	
	CUISystem();

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pUIComp) override;
	void unregisterComponent(ISystemComponent* pUIComp) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;
	
	Font* requestFont(int size);

private:
	Font* createFont();
	void switchUI(UI* pUI);
	void printMessage(UIMessage* pMessage);
private:
	UI* m_pCurrentUI;
	UI* m_pPreviousUI;
	std::map<int, Font*> m_loadedFonts;
	std::vector<UI*> m_pUIs;
};

extern CUISystem* g_pUISystem;
