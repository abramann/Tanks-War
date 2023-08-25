// Author: abramann
// interface.h

#pragma once

#include "types.h"
#ifdef _CLIENT_BUILD
#include "..\Client\client.h"

class TanksWar;
#endif
#ifdef _SERVER_BUILD
#include "..\Server\server.h"

class TanksWarServer;
#endif
class Graphics;
class Audio;
class Map;

class Interface
{
public:

	Interface();
	~Interface();

#ifdef _CLIENT_BUILD
	void initialize(Client* client, TanksWar* pTK);
#else ifdef _SERVER_BUILD
	void initialize(TanksWarServer* pTKServer);
#endif
	void executeMainActivity();
	void executeMultiplayerActivity();
	void executeSettingsActivity();
	void executePlayingActivity();
	void render();
	void beginActivity(bool blankActivity, interfaceNS::FontSize fontSize = interfaceNS::FONTSIZE_MED);
	void endActivity(bool backButton = false, interfaceNS::Activity backActivity = interfaceNS::NO_ACITVITY);

private:

	void showFPS(uint16_t fps);
	/*void inputText(const char* desc, const ImVec4& descColor, const char* label,
		char* buf, const uint16_t& len, const ImVec4& color, ImGuiInputTextFlags flags);
	void inputText(const char* label, char* buf, const uint16_t& len, const ImVec4& textColor,
		ImGuiInputTextFlags flags = 0);
	void inputInt(const char* label, int* buf, const ImVec4& textColor,
		int32_t min = 0, int32_t max = 0, ImGuiInputTextFlags flags = 0);
	void inputInt(const char* desc, const Vec4& descColor, const char* label, int* buf,
		const Vec4& color, const int32_t elements, const int32_t min = 0, const int32_t max = 0,
		ImGuiInputTextFlags flags = 0);*/

	bool inputText(std::string desc, char* buf, size_t length, ImGuiInputTextFlags flags, interfaceNS::ListType listType = interfaceNS::LIST_NONE);
	bool inputInt(std::string desc, int32* pValue, ImGuiInputTextFlags flags, interfaceNS::ListType listType = interfaceNS::LIST_NONE);
	bool button(std::string text, Vec2 size = Vec2(0, 0));
	void separatorText(std::string text, interfaceNS::FontSize fontSize = interfaceNS::FONTSIZE_MED, Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f));

#ifdef _CLIENT_BUILD
	TanksWar* m_pTK;
#else
	TanksWarServer* m_pTKServer;
#endif
	Graphics* m_pGraphics;
	Audio* m_pAudio;
	ImFont* m_pFont[interfaceNS::FONTSIZES];
	bool m_blankActivity;
	int8 m_activity;
	Vec2 m_inputFieldListPos;
#ifdef _CLIENT_BUILD
	Client* m_pClient;
#else ifdef _SERVER_BUILD
	//Server* m_pServer;
#endif
};