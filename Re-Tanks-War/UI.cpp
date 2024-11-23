#include "UI.h"

std::map<int, ImFont*> UI::s_fontmap;

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

Font* UI::getFont(int size)
{
	if (s_fontmap.find(size) == s_fontmap.end())
	{
		std::string fontpath = strFormat("%s%s%s", values::FONTS_DIR, values::USED_FONT, ".ttf");
		auto& io = ImGui::GetIO();
		s_fontmap[size] = io.Fonts->AddFontFromFileTTF(fontpath.c_str(), size);
	}
	return s_fontmap[size];
}