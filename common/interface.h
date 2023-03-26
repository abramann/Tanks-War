#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "graphics.h"
#include "audio.h"
#include "map.h"

#ifdef _CLIENT_BUILD
#include "..\Tanks-War\client.h"
#endif
#ifdef _SERVER_BUILD
#include "..\Tanks-War-Server\server.h"
#endif


#define TITLE_BUTTON_COLOR Vec4(0.94f, 0.25f, 0.20f, 1.0f)
#define TITLE_BUTTON_TEXT_COLOR ImVec4(1,1,1,1)

class Interface
{
public:

	Interface();
	~Interface();
	void initialize();
	Menus menu(const char* name, const ImVec2 pos, const ImVec2 size, const ImVec2 buttonSize, ImGuiWindowFlags flag, std::initializer_list<const char*> items);
	void mainMenu();
	void settingMenu(Graphics& graphics, Audio& audio);
#ifdef _CLIENT_BUILD
	void multiplayerMenu(Client& client);
#else ifdef _SERVER_BUILD
	void multiplayerMenu(Server& server, Map& map);
#endif
	
	Menus m_menu;

private:

	void title(const char* text, Vec4 colText = TITLE_BUTTON_TEXT_COLOR, Vec4 colButton = TITLE_BUTTON_COLOR);
	void inputText(const char* desc, const ImVec4& descColor, const char* label,
		char* buf, const uint16_t& len, const ImVec4& color, ImGuiInputTextFlags flags);
	void inputText(const char* label, char* buf, const uint16_t& len, const ImVec4& textColor,
		ImGuiInputTextFlags flags = 0);
	
	void Interface::inputInt(const char* label, int* buf, const ImVec4& textColor,
		int32_t min = 0, int32_t max = 0, ImGuiInputTextFlags flags = 0);
	void Interface::inputInt(const char* desc, const Vec4& descColor, const char* label, int* buf,
		const Vec4& color, const int32_t elements, const int32_t min = 0, const int32_t max = 0, ImGuiInputTextFlags flags = 0);

	bool button(const char* text, ImVec2 size = Vec2(0, 0), ImVec4 colText = Vec4(0, 0, 0, 0), ImVec4 colButton = Vec4(0, 0, 0, 0),
		ImVec4 colActive = Vec4(0, 0, 0, 0), ImVec4 colHorvored = Vec4(0, 0, 0, 0), ImGuiButtonFlags flags = 0);

	void text(const char* text, const ImVec4 color = Vec4(0, 0, 0, 0));
	void fillWindow();
	void pushSubMenu(const char* label);
	void popSubMenu();

	ImFont* m_font[3];
};


#endif
