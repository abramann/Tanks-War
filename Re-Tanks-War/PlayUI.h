#pragma once
#include "UI.h"

class CPlayUI : public UI
{
public:
	void startup() override;
	void update() override;
	void reset() override; 
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

protected:
	void beginUI() override;
	void endUI() override;
	void setupProperties() override;
};

extern CPlayUI* g_pPlayUI;

