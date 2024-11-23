#pragma once


#include "UI.h"

class ITexture;

class CIntroUI : public UI
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
	ITexture* m_pLogo = nullptr;
};

extern UI* g_pIntroUI;