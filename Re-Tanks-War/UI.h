#pragma once

#include "Subsystem.h"
#include "String.h"
#include "Widget.h"
#include "Values.h"
#include <map>

typedef ImFont Font;

class IWidget;

class UI : public ISubsystem
{
public:

	enum UIEvents
	{
		BUTTON_CLICKED,
	};

	virtual std::string getName() const { return "UI"; }

	//virtual void update() = 0;
	//virtual void reset() = 0;
	void perform() override;

	void shutdown() override {};
	void onStartGame() override {};
	void onQuitGame() override {};
	void onPauseGame() override {};
	void onResumGame() override {};

	void registerComponent(ISystemComponent* pWidget) override;
	void unregisterComponent(ISystemComponent* pWidget) override;

protected:
	virtual void beginUI() = 0;
	virtual void endUI() = 0;
	virtual void setupProperties() = 0;

	static ImFont* getFont(int size);
protected:
	const int ImGuiWindow_FillScreen =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoResize;
	std::vector<IWidget*> m_pWidgets;

	static std::map<int, ImFont*> s_fontmap;
};

