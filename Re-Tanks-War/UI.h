#pragma once

#include "SystemComponent.h"

class UI : public ISystemComponent
{
public:
	UI() : m_pParent(nullptr) {}

	virtual std::string getName() const { return "UI"; }

	UI* getParent() const { return m_pParent; }
	void assignParent(UI* pParent) { m_pParent = pParent; };

	void show()
	{
		beginUI();
		drawUI();
		endUI();
	}

protected:
	virtual void beginUI() = 0;
	virtual void drawUI() = 0;
	virtual void endUI() = 0;

private:
	UI* m_pParent;
};
//static UI* buildStartMenuUI();
//static UI* buildStartGameUI();
//static UI* buildSettingsUI();
