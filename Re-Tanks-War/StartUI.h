#pragma once

#include "UI.h"
#include "Button.h"

class CStartUI : public UI
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void handleEvent(ISystemComponent* pWidget, int eventCode, void* event) override;

	static int s_fontSize;
	static ImVec2 s_butsBeginPos;
	// Buttons style
	static float s_butsBorderSize;
	static ImVec2 s_butsAlign;
	static ImVec2 s_butsSize;
	// Buttons Colors
	static Color s_butsColor;
	static Color s_butsHoverColor;
	static Color s_butsOnClickColor;
	static Color s_butsBorderColor;

private:
	void beginUI() override;
	void endUI() override;
	void setupProperties() override;
};

extern UI* g_pStartUI;