// Author: abramann
// interface.h

#pragma once

#include "types.h"
#ifdef _CLIENT_BUILD
#include "..\Client\client.h"

class TanksWar;
#else
#include "..\Server\server.h"

class TanksWarServer;
#endif
class Graphics;
class Audio;
class Timer;
class Input;
class ThisPlayer;

class Interface
{
public:

	Interface();
	~Interface();

#ifdef _CLIENT_BUILD
	void initialize(TanksWar* pTW);
#else ifdef _SERVER_BUILD
	void initialize(TanksWarServer* pTWServer);
#endif
	void executeMainActivity();
	void executeMultiplayerActivity();
	void executeSettingsActivity();
	void executePlayingActivity();
	void executeAboutActivity();
	void render();
	void beginActivity(bool blankActivity, interfaceNS::FontSize fontSize = interfaceNS::FONTSIZE_MED);
	void endActivity(bool backButton = false, interfaceNS::Activity backActivity = interfaceNS::NO_ACITVITY);
	void setActivity(interfaceNS::Activity activity) { m_activity = activity; }
	
private:

	void showFPS();
	bool inputText(std::string desc, char* buf, size_t length, ImGuiInputTextFlags flags, interfaceNS::ListType listType = interfaceNS::LIST_NONE);
	bool inputInt(std::string desc, int32* pValue, ImGuiInputTextFlags flags, interfaceNS::ListType listType = interfaceNS::LIST_NONE);
	bool button(std::string text, Vec2 size = Vec2(0, 0));
	void separatorText(std::string text, interfaceNS::FontSize fontSize = interfaceNS::FONTSIZE_MED, Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void text(std::string text, Vec4 color = Vec4(0,0,0,0), interfaceNS::FontSize fontSize = 0);
#ifdef _CLIENT_BUILD
	TanksWar* m_pTW;
	ThisClient* m_pThisClient;
#else
	TanksWarServer* m_pTWServer;
#endif
	Graphics* m_pGraphics;
	Audio* m_pAudio;
	Timer* m_pTimer;
	Input* m_pInput;
	ImFont* m_pFont[interfaceNS::FONTSIZES];
	bool m_blankActivity;
	int8 m_activity;
	Vec2 m_inputFieldListPos;
};