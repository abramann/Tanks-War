#include "UI.h"

void UI::perform()
{
	beginUI();

	subsystemsDo(m_pWidgets, draw);

	endUI();
}

void UI::registerComponent(ISystemComponent* pWidget)
{
	subsystemRegister(m_pWidgets, dynamic_cast<IWidget*>(pWidget));
}

void UI::unregisterComponent(ISystemComponent* pWidget)
{
	subsystemUnregister(m_pWidgets, dynamic_cast<IWidget*>(pWidget));
}
