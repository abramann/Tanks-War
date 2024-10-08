#pragma once

#include "System.h"
#include "UI.h"
#include "Types.h"
#include <string>
#include <memory>
#include <vector>



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

	void registerUI(UI* pUI);
	void unregisterUI(UI* pUI);
	void activateUI(UI* pUI);
	void printMessage(UIMessage* pMessage);

private:
	UI* m_pCurrentUI;
};

extern std::shared_ptr<CUISystem> g_pUISystem;

