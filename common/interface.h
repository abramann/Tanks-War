#ifndef _INTERFACE_H
#define _INTERFACE_H

class Game;
class Graphics;
class Audio;
class Map;

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

#ifdef _CLIENT_BUILD
	void initialize(Client* client, Game* game);
#else ifdef _SERVER_BUILD
	void initialize(Server* server, Game* game);
#endif

	void mainMenu();
	void multiplayerMenu();
	void settingMenu();
	void show();
	void showFPS(uint16_t fps);

	Menu m_menu;

private:

	void title(const char* text, Vec4 colText = TITLE_BUTTON_TEXT_COLOR, Vec4 colButton = TITLE_BUTTON_COLOR);
	void inputText(const char* desc, const ImVec4& descColor, const char* label,
		char* buf, const uint16_t& len, const ImVec4& color, ImGuiInputTextFlags flags);
	void inputText(const char* label, char* buf, const uint16_t& len, const ImVec4& textColor,
		ImGuiInputTextFlags flags = 0);
	void inputInt(const char* label, int* buf, const ImVec4& textColor,
		int32_t min = 0, int32_t max = 0, ImGuiInputTextFlags flags = 0);
	void inputInt(const char* desc, const Vec4& descColor, const char* label, int* buf,
		const Vec4& color, const int32_t elements, const int32_t min = 0, const int32_t max = 0,
		ImGuiInputTextFlags flags = 0);
	bool button(const char* text, ImVec2 size = Vec2(0, 0), ImVec4 colText = Vec4(0, 0, 0, 0), ImVec4 colButton = Vec4(0, 0, 0, 0),
		ImVec4 colActive = Vec4(0, 0, 0, 0), ImVec4 colHorvored = Vec4(0, 0, 0, 0), ImGuiButtonFlags flags = 0);
	void text(const char* text, const ImVec4 color = Vec4(0, 0, 0, 0));
	void fillWindow();
	void pushSubMenu(const char* label);
	void popSubMenu();

	Graphics* m_pGraphics;
	Audio* m_pAudio;
	Map* m_pMap;
	ImFont* m_font[3];

#ifdef _CLIENT_BUILD
	Client* m_pClient;
#else ifdef _SERVER_BUILD
	Server* m_pServer;
#endif
};

#endif
