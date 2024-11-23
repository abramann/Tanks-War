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

private:
	void beginUI() override;
	void endUI() override;
	void setupProperties() override;

private:
	ImVec2 s_butsBeginPos;
	ImVec2 s_butsSize;
};

extern UI* g_pStartUI;