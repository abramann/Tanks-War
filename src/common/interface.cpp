// interface.cpp
// Author: abramann

#include "interface.h"
#include "fileio.h"
#include "imgui\imgui.h"
#include "texturemanger.h"
#include "sprite.h"
#include "timer.h"
#include "input.h"
#include "inlined.inl"
#ifdef _CLIENT_BUILD
#include "..\..\Client\tankswar.h"
#include "..\Client\thisclient.h"
#else
#include "..\Server\tankswarServer.h"
#include "map.h"
#include "fileio.h"
#endif

extern void HelpMarker(const char* desc);

#pragma warning(disable : 4244)

using namespace colorNS;
using namespace ImGui;
using namespace interfaceNS;
using namespace fileNS;

bool vectorOfStringGetter(void* data, int n, const char** out_text)
{
	const vector<string>* v = (vector<string>*)data;
	*out_text = v->at(n).c_str();
	return true;
}

Interface::Interface() : m_activity(MAIN_ACTIVITY), m_blankActivity(true)
{
}

Interface::~Interface()
{
}

#ifdef _SERVER_BUILD
void Interface::initialize(TanksWarServer* pTWServer)
{
	m_pTWServer = pTWServer;
	TanksWarServer*& pGame = pTWServer;
#else ifdef _CLIENT_BUILD
void Interface::initialize(TanksWar* pTW)
{
	m_pTW = pTW;
	TanksWar*& pGame = pTW;
	m_pThisClient = pTW->getThisClient();
#endif
	m_pGraphics = pGame->getGraphics();;
	m_pAudio = pGame->getAudio();
	m_pTimer = pGame->getTimer();
	m_pInput = pGame->getInput();
	ImGuiIO& io = GetIO();
	m_pFont[FONTSIZE_TINY] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 10);
	m_pFont[FONTSIZE_SMALL] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 15);
	m_pFont[FONTSIZE_SMALL2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 18);
	m_pFont[FONTSIZE_MED] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 22);
	m_pFont[FONTSIZE_MED2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 24);
	m_pFont[FONTSIZE_LARGE] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 26);
	m_pFont[FONTSIZE_LARGE2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT_PATH, 32);
}

void Interface::executeMainActivity()
{
	beginActivity(true, FONTSIZE_LARGE2);
	Vec2 butSize = Vec2(g_pGameSettings->width / 2, g_pGameSettings->height / 5),
		butPos = Vec2((g_pGameSettings->width / 2) - butSize.x / 2, g_pGameSettings->height / 10);
	SetCursorPos(butPos);
	if (button("Multiplayer", butSize))
		m_activity = MULTIPLAYER_ACTIVITY;

	butPos.y += butSize.y + butSize.y * MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Settings", butSize))
		m_activity = SETTINGS_ACTIVITY;

	butPos.y += butSize.y + butSize.y*MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Quit", butSize))
		m_activity = QUIT_ACTIVITY;

	PushFont(m_pFont[FONTSIZE_MED]);
	SetCursorPos(Vec2(0, g_pGameSettings->height * 0.9));
	if (button("About", Vec2(butSize.x / 4, butSize.y / 4)))
		m_activity = CREDITS_ACTIVITY;

	PopFont();
	endActivity();
}

void Interface::executeSettingsActivity()
{
#ifdef _CLIENT_BUILD
	TanksWar*& pGame = m_pTW;;
#else
	TanksWarServer* pGame = m_pTWServer;
#endif
	beginActivity(false, FONTSIZE_SMALL2);
	PushStyleColor(ImGuiCol_Text, YELLOW);
	///////////////////////////////////
	//	Graphics section
	separatorText("Graphcis", FONTSIZE_LARGE, RED);
	bool cWin = Checkbox("Windowed", &g_pGameSettings->windowed);
	bool cVsync = Checkbox("VSync", &g_pGameSettings->vsync);
	bool cComputeShader = Checkbox("Compute Shader", &g_pGameSettings->computeShader); SameLine();
	ImGui::PopStyleColor();
	HelpMarker("Use GPU for collision detection.");
	PushStyleColor(ImGuiCol_Text, YELLOW);
	if (cWin)
	{
		pGame->onUpdateDisplay();
		PopStyleColor();
		endActivity(1, 1);
		return;
	}
	if (cVsync || cComputeShader)
		pGame->updateGameSettings();

	static auto suppMode = m_pGraphics->getSupportedResolutionAsString();
	static int32 currMode = m_pGraphics->getIndexCurrentResolution(); 
	bool input = ListBox("##resolution", &currMode, vectorOfStringGetter, &suppMode, suppMode.size());
	if (input)
	{
		std::string resol = suppMode[currMode];
		auto x = resol.find('x');
		g_pGameSettings->width = std::stoi(resol.substr(0, x)),
			g_pGameSettings->height = std::stoi(resol.substr(x + 1, resol.length()));
		pGame->onUpdateDisplay();
	}

	/////////////////////////////////////////////////
	//	Audio section
	separatorText("Audio", FONTSIZE_LARGE, RED);
	if (Checkbox("Play audio", &g_pGameSettings->audio))
	{
		pGame->updateGameSettings();
	}
#ifdef _CLIENT_BUILD
	/////////////////////////////////////////////////
	// Multiplayer section
	separatorText("Multiplayer", FONTSIZE_LARGE, RED);
	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	bool connected = m_pTW->isOnline();
	if (connected)
		configFlags = ImGuiInputTextFlags_ReadOnly;

	static char* pPlayerName = m_pTW->getClientName();
	input = inputText("Player Name", pPlayerName, gameNS::MAX_NAME_LEN, configFlags);
	if (input)
		m_pTW->updateClientInfo();

#endif
	PopStyleColor();
	endActivity(true, MAIN_ACTIVITY);
}

void Interface::executePlayingActivity()
{
#ifdef _CLIENT_BUILD
	m_pTW->updateScene();
	m_pTW->renderScene();

	SetNextWindowPos(Vec2(0, 0));
	ImGui::Begin("Status", 0, ImGuiWindowFlags_NoTitleBar);
	PushStyleColor(ImGuiCol_Text, YELLOW); 
	PushFont(m_pFont[FONTSIZE_SMALL2]);
	auto pThisClient = m_pTW->getThisClient();
	text(strFormat("Health %d", static_cast<int32>(pThisClient->getHealth())));
	text(strFormat("Kills %d", 0, 0), YELLOW);
	text(strFormat("Score %d", static_cast<int32>(pThisClient->getInflictedDamage())));
	text(strFormat("Players %d", m_pTW->getExistClients()));
	PopFont();
	PopStyleColor();
	End();
	if (m_pInput->isKeyPressed(inputNS::ESCAPE_KEY))
		m_activity = MULTIPLAYER_ACTIVITY;
#endif
}

void Interface::executeCreditsActivity()
{
	beginActivity(true, FONTSIZE_SMALL2);
	separatorText("Tanks War", FONTSIZE_LARGE, RED);
	text("Simple multiplayer game developed by", YELLOW); SameLine(); text("abramann", CYAN); SameLine(); text("(github.com/abramann/Tanks-War).", YELLOW);
	text("Credits to", WHITE, FONTSIZE_MED2);
	PushStyleColor(ImGuiCol_Text, YELLOW);
	BulletText("Charles Kelly for the net library and the useful information in his books (programming2dgames.net)");
	BulletText("Danial  Robbin for Dirent.");
	PopStyleColor();
	text("Assets sources:", WHITE, FONTSIZE_MED2);
	BulletText("adwad\nawld");
	endActivity(true, MAIN_ACTIVITY);
}

void Interface::render()
{
	switch (m_activity)
	{
	case MAIN_ACTIVITY:
		executeMainActivity();
		break;
	case MULTIPLAYER_ACTIVITY:
		executeMultiplayerActivity();
		break;
	case SETTINGS_ACTIVITY:
		executeSettingsActivity();
		break;
	case PLAYING_ACTIVITY:
		executePlayingActivity();
		break;
	case CREDITS_ACTIVITY:
		executeCreditsActivity();
		break;
	case QUIT_ACTIVITY:
		PostQuitMessage(0);
	default:
		break;
	}

	showFPS();
}

void Interface::showFPS()
{
	ImGui::GetForegroundDrawList()->AddText(ImVec2(0, 0), ImColor(255.0f, 255.0f, 255.0f, 255.0f),
		strFormat("FPS %d", m_pTimer->getFPS()).c_str(), nullptr);
}

void Interface::beginActivity(bool blankActivity, interfaceNS::FontSize fontSize)
{
	SetNextWindowPos(Vec2(0, 0));
	SetNextWindowSize(Vec2(g_pGameSettings->width, g_pGameSettings->height));
	Begin("Activity", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	PushFont(m_pFont[fontSize]);
	m_blankActivity = blankActivity;
	if (!m_blankActivity)
	{
		PushStyleVar(ImGuiStyleVar_ChildBorderSize, 5);
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);
		BeginChild("Window", Vec2(g_pGameSettings->width / 2, g_pGameSettings->height*1.0f / 1.5f), true, ImGuiWindowFlags_NoTitleBar);
		PopStyleVar(2);
	}
}

void Interface::endActivity(bool backButton, Activity backActivity)
{
	if (!m_blankActivity)
		EndChild();

	PopFont();
	if (backButton)
	{
		Vec2 m_backButPos = Vec2(g_pGameSettings->width / 2.0f, g_pGameSettings->height * 0.7f);
		Vec2 m_backButSize = Vec2(g_pGameSettings->width / 10, g_pGameSettings->height / 12);
		PushFont(m_pFont[FONTSIZE_LARGE]);
		SetCursorPos(m_backButPos);
		if (button("Back", m_backButSize) || m_pInput->isKeyPressed(inputNS::ESCAPE_KEY))
			m_activity = backActivity;

		PopFont();
	}

	End();
}

void Interface::executeMultiplayerActivity()
{
	beginActivity(false, FONTSIZE_SMALL2);
#ifdef _CLIENT_BUILD
	separatorText("Multiplayer Config", FONTSIZE_LARGE, RED);
	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	bool connected = m_pTW->isOnline();
	if (connected)
		configFlags = ImGuiInputTextFlags_ReadOnly;

	Vec4 colorChangeable = (connected) ? SILVER : WHITE;
	static char* serverIP = m_pTW->getServerIP();
	bool input = inputText("Server IP", serverIP, gameNS::MAX_NAME_LEN, configFlags, LIST_MAIN);
	if (input)
		m_pTW->updateClientInfo();

	static int32 port = m_pTW->getServerPort();
	input = inputInt("Port", &port, configFlags, LIST_VERTICAL);
	if (input)
		m_pTW->setServerPort(port);

	Text("status");
	SameLine();
	SetCursorPosX(m_inputFieldListPos.x);
	auto status = m_pTW->getStatus();
	PushStyleColor(ImGuiCol_Text, clientNS::CLIENT_STATUS[status].second);
	Text(clientNS::CLIENT_STATUS[status].first);
	PopStyleColor();
	SetCursorPosX(m_inputFieldListPos.x * 1.5f);
	PushStyleVar(ImGuiStyleVar_FrameRounding, 20);
	if (!connected)
	{
		PushStyleColor(ImGuiCol_Button, GREEN);
		input = button("Connect");
		if (input)
			m_pTW->connect();
	}
	else
	{
		PushStyleColor(ImGuiCol_Button, RED);
		input = button("Disconnect");
		if (input)
			m_pTW->disconnect();

		SameLine();
		input = button("Join");
		if (input)
			m_activity = PLAYING_ACTIVITY;
	}

	PopStyleColor();
	PopStyleVar();
#else ifdef _SERVER_BUILD
	separatorText("Server Config", FONTSIZE_LARGE, RED);
	bool srvActive = m_pTWServer->isOnline();
	if (srvActive)
		PushStyleColor(ImGuiCol_Text, colorNS::GREY);
	else
		PushStyleColor(ImGuiCol_Text, colorNS::YELLOW);

	ImGuiInputTextFlags configFlags = (srvActive) ? ImGuiInputTextFlags_ReadOnly : 0;
	static char pServerIP[netNS::IP_SIZE];
	static int32 maxClients = m_pTWServer->getMaxClients();
	bool input = inputInt("Max Clients", &maxClients, configFlags, LIST_MAIN);
	if (input)
		m_pTWServer->setMaxClients(maxClients);

	m_pTWServer->getIP(pServerIP);
	inputText("Server IP", pServerIP, netNS::IP_SIZE, ImGuiInputTextFlags_ReadOnly, LIST_VERTICAL);
	static int32 port = *m_pTWServer->getPort();
	input = inputInt("Port", &port, configFlags, LIST_VERTICAL);
	if (input)
		m_pTWServer->setPort(port);

	static auto map = FileIO::getDirFileList(fileNS::MAP_DIR, 0, ".map", false);
	int32 iCurrMap = std::find(map.begin(), map.end(), m_pTWServer->getMap()) - map.begin();
	input = ListBox("##map", &iCurrMap, vectorOfStringGetter, &map, map.size());
	if (input && !srvActive)
		m_pTWServer->setMap(map[iCurrMap]);

	PopStyleColor();
	Text("status");
	SameLine();
	SetCursorPosX(m_inputFieldListPos.x);
	auto status = m_pTWServer->getStatus();
	PushStyleColor(ImGuiCol_Text, serverNS::SERVER_STATUS[status].second);
	Text(serverNS::SERVER_STATUS[status].first);
	PopStyleColor();
	SetCursorPosX(m_inputFieldListPos.x * 1.5f);
	PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	if (!srvActive)
	{
		PushStyleColor(ImGuiCol_Button, GREEN);
		input = button("Start");
		if (input)
			m_pTWServer->serverStart();
	}
	else
	{
		PushStyleColor(ImGuiCol_Button, RED);
		input = button("Disconnect");
		if (input)
			m_pTWServer->serverShutdown();
	}

	PopStyleColor();
	PopStyleVar();
	int32 clients = m_pTWServer->getConnectedClients();
	Text(strFormat("Connected clients %d", clients).c_str());
#endif
	endActivity(true, MAIN_ACTIVITY);
}

bool Interface::inputText(std::string desc, char * buf, size_t length, ImGuiInputTextFlags flags, ListType listType)
{
	Text(desc.c_str());
	PushItemWidth(GetFontSize()*length / 2);
	std::string label = strFormat("%s%s", "##", desc.c_str());
	SameLine(0.0f, 0.0f);
	switch (listType)
	{
	case LIST_NONE:
		break;
	case LIST_MAIN:
		m_inputFieldListPos = GetCursorPos();
		break;
	case LIST_VERTICAL:
		SetCursorPosX(m_inputFieldListPos.x);
		break;
	case LIST_HORIOZONTIAL:
		SetCursorPosY(m_inputFieldListPos.y);
		break;
	default:
		break;
	}

	bool input = InputText(label.c_str(), buf, length, flags);
	PopItemWidth();
	return input;
}

bool Interface::inputInt(std::string desc, int32 * pValue, ImGuiInputTextFlags flags, ListType listType)
{
	Text(desc.c_str());
	std::string label = strFormat("%s%s", "##", desc.c_str());
	PushItemWidth(GetFontSize() * 4);
	SameLine(0.0f, 0.0f);
	switch (listType)
	{
	case LIST_NONE:
		break;
	case LIST_MAIN:
		m_inputFieldListPos = GetCursorPos();
		break;
	case LIST_VERTICAL:
		SetCursorPosX(m_inputFieldListPos.x);
		break;
	case LIST_HORIOZONTIAL:
		SetCursorPosY(m_inputFieldListPos.y);
		break;
	default:
		break;
	}

	bool input = InputInt(label.c_str(), pValue, 0, 0, flags);
	PopItemWidth();
	return input;
}

bool Interface::button(std::string text, Vec2 size)
{
	bool click = Button(text.c_str(), size); ;
	if (click)
		m_pAudio->play("button-click");

	return click;
}

void Interface::separatorText(std::string text, FontSize fontSize, Vec4 color)
{
	PushFont(m_pFont[fontSize]);
	PushStyleColor(ImGuiCol_Text, color);
	SeparatorText(text.c_str());
	PopStyleColor();
	PopFont();
}

void Interface::text(std::string text, Vec4 color, FontSize fontSize)
{
	if (!color.isEmpty())
		PushStyleColor(ImGuiCol_Text, color);
	if (fontSize)
		PushFont(m_pFont[fontSize]);

	Text(text.c_str());
	if (!color.isEmpty())
		PopStyleColor(); 
	if (fontSize)
		PopFont();
}