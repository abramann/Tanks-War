// interface.cpp
// Author: abramann

#include "interface.h"
#include "fileio.h"
#ifdef _CLIENT_BUILD
#include "..\..\Client\tankswar.h"
#else
#include "..\..\Server\tankswarserver.h"
#endif
#include "imgui\imgui.h"
#include "texturemanger.h"
#include "sprite.h"

extern void HelpMarker(const char* desc);

#ifdef _SERVER_BUILD
#include "map.h"
#endif

#pragma warning(disable : 4244)

using namespace colorNS;
using namespace ImGui;
using namespace interfaceNS;

static std::map<int, std::pair<const char*, ImVec4>>  ClientConnectionStatus = {
	{ CLIENT_UNCONNECTED,{ "Not Connected", GREY}},
	{CLIENT_UNCONNECTED_DISCONNECT, {"Disconnected", RED}},
	//{CLIENT_UNCONNECTED_MAP_NOT_FOUND, {"Server map doesn't exist", YELLOW}},
	//{ CLIENT_UNCONNECTED_MAP_NOT_LOAD,{ "Failed to load server map", YELLOW } },
	{ CLIENT_CONNECTED_WAITING,{ "Connected, Waiting server to start...",GREEN } },
	{ CLIENT_CONNECTED_PLAYING,{ "Connected",GREEN } }
};

Interface::Interface() : m_activity(MAIN_ACTIVITY), m_blankActivity(true)
{
}

Interface::~Interface()
{
}

#ifdef _SERVER_BUILD
void Interface::initialize(Server* server, Game* game)
{
	m_pServer = server;
#else ifdef _CLIENT_BUILD
void Interface::initialize(Client* client, TanksWar* game)
{
	m_pClient = client;
#endif
	m_pGraphics = game->getGraphics();;
	m_pAudio = game->getAudio();
	m_pMap = game->getMap();
	m_pGame = game;
	ImGuiIO& io = GetIO();
	m_pFont[FONTSIZE_TINY] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 10);
	m_pFont[FONTSIZE_SMALL] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 15);
	m_pFont[FONTSIZE_SMALL2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 18);
	m_pFont[FONTSIZE_MED] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 22);
	m_pFont[FONTSIZE_MED2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 24);
	m_pFont[FONTSIZE_LARGE] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 26);
	m_pFont[FONTSIZE_LARGE2] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 32);
}

void Interface::executeMainActivity()
{
	beginActivity(true, FONTSIZE_LARGE2);
	Vec2 butSize = Vec2(g_gameInfo.width / 3, g_gameInfo.height / 10),
		butPos = Vec2((g_gameInfo.width / 2) - butSize.x / 2, g_gameInfo.height / 10);
	SetCursorPos(butPos);
	if (button("Multiplayer", butSize))
		m_activity = MULTIPLAYER_ACTIVITY;

	butPos.y += butSize.y + butSize.y*MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Settings", butSize))
		m_activity = SETTINGS_ACTIVITY;

	butPos.y += butSize.y + butSize.y*MAINACTIVITY_BUTTON_PADDING_Y;
	SetCursorPos(butPos);
	if (button("Quit", butSize))
		m_activity = QUIT_ACTIVITY;

	endActivity();
}

void Interface::executeSettingsActivity()
{
	beginActivity(false, FONTSIZE_SMALL2);
	///////////////////////////////////
	//	Graphics section
	separatorText("Graphcis", FONTSIZE_LARGE, RED);
	static GameInfo gameInfo = g_gameInfo;
	bool cWin = Checkbox("Windowed", &gameInfo.windowed);
	bool cVsync = Checkbox("VSync", &gameInfo.vsync);
	bool cComputeShader = Checkbox("Compute Shader", &gameInfo.computeShader); SameLine();  HelpMarker("Use GPU for collision detection.");
	ShowDemoWindow();
	if (cWin || cVsync)
		FileIO::createGameInfo(&gameInfo);

	static char** suppModes = m_pGraphics->getSupportedAdapterModesAsCArray();
	static size_t modes = m_pGraphics->getAdapterModes();
	static int currMode = m_pGraphics->getCurrentAdapterMode();
	if (ListBox("##resolution", &currMode, (char**)suppModes, modes))
	{
		std::string resol = suppModes[currMode];
		auto x = resol.find('x');
		gameInfo.width = std::stoi(resol.substr(0, x)),
			gameInfo.height = std::stoi(resol.substr(x + 1, resol.length()));
		FileIO::createGameInfo(&gameInfo);
	}

	/////////////////////////////////////////////////
	//	Audio section
	separatorText("Audio", FONTSIZE_LARGE, RED);
	if (Checkbox("Play audio", &gameInfo.audio))
		FileIO::createGameInfo(&gameInfo);

#ifdef _CLIENT_BUILD
	/////////////////////////////////////////////////
	// Multiplayer section
	separatorText("Multiplayer", FONTSIZE_LARGE, RED);
	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	bool connected = m_pClient->isConnected();
	if (connected)
		configFlags = ImGuiInputTextFlags_ReadOnly;

	static char* pPlayerName = m_pGame->getPlayerName();
	bool input = inputText("Player Name", pPlayerName, gameNS::MAX_NAME_LEN, configFlags);
	if (input)
		m_pGame->updateClientInfo();

#endif
	endActivity(true, MAIN_ACTIVITY);
}

void Interface::executePlayingActivity()
{
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
	case QUIT_ACTIVITY:
		PostQuitMessage(0);
	default:
		break;
	}
}

void Interface::showFPS(uint16_t fps)
{
	SetCursorPos(Vec2(0, 0));
	Begin("fps", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	SetCursorPos(Vec2(0, 0));
	Text("FPS %d", fps);
	End();
}

void Interface::beginActivity(bool blankActivity, interfaceNS::FontSize fontSize)
{
	SetNextWindowPos(Vec2(0, 0));
	SetNextWindowSize(Vec2(g_gameInfo.width, g_gameInfo.height));
	Begin("Activity", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	PushFont(m_pFont[fontSize]);
	m_blankActivity = blankActivity;
	if (!m_blankActivity)
	{
		PushStyleVar(ImGuiStyleVar_ChildBorderSize, 5);
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);
		BeginChild("Window", Vec2(g_gameInfo.width / 2, g_gameInfo.height*1.0f / 1.5f), true, ImGuiWindowFlags_NoTitleBar);
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
		Vec2 m_backButPos = Vec2(g_gameInfo.width / 2.0f, g_gameInfo.height * 0.7f);
		Vec2 m_backButSize = Vec2(g_gameInfo.width / 10, g_gameInfo.height / 12);
		PushFont(m_pFont[FONTSIZE_LARGE]);
		SetCursorPos(m_backButPos);
		if (button("Back", m_backButSize))
			m_activity = backActivity;

		PopFont();
	}

	End();
}

#ifdef _CLIENT_BUILD
void Interface::executeMultiplayerActivity()
{
	beginActivity(false, FONTSIZE_SMALL2);
	separatorText("Multiplayer Config", FONTSIZE_LARGE, RED);
	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	bool connected = m_pClient->isConnected();
	if (connected)
		configFlags = ImGuiInputTextFlags_ReadOnly;

	Vec4 colorChangeable = (m_pClient->isConnected()) ? SILVER : WHITE;
	static char* serverIP = m_pGame->getServerIP();
	bool input = inputText("Server IP", serverIP, gameNS::MAX_NAME_LEN, configFlags, LIST_MAIN);
	if (input)
		m_pGame->updateClientInfo();

	static int32 port = m_pGame->getServerPort();
	input = inputInt("Port", &port, configFlags, LIST_VERTICAL);
	if (input)
		m_pGame->setServerPort(port);

	Text("status");
	SameLine();
	SetCursorPosX(m_inputFieldListPos.x);
	auto status = m_pClient->getStatus();
	PushStyleColor(ImGuiCol_Text, ClientConnectionStatus[status].second);
	Text(ClientConnectionStatus[status].first);
	PopStyleColor();
	SetCursorPosX(m_inputFieldListPos.x * 1.5f);
	PushStyleVar(ImGuiStyleVar_FrameRounding, 20);
	if (!connected)
	{
		PushStyleColor(ImGuiCol_Button, GREEN);
		input = button("Connect");
		if (input)
			m_pClient->connect();
	}
	else
	{
		PushStyleColor(ImGuiCol_Button, ORANGE);
		input = button("Disconnect");
		if (input)
			m_pClient->disconnect();
	}

	PopStyleColor();
	PopStyleVar();
	/*
	unsigned short* usPort = m_pClient->getServerPort();
	int iPort = (int)*usPort;
	inputInt("Port", colorChangeable, "##Port", &iPort, colorChangeable, 5);
	*usPort = iPort;
	PopFont();

	SeparatorText("Server Status");
	PushFont(m_pFont[FONTSIZE_MED]);
	m_pClient->getstatus();
	text(stringCLIENT_status[status].c_str(), colorCLIENT_status[status]);
	inputText("Game Map", NOOPTIONS_COLOR, "##GamerMap", m_pClient->getGameMap(),
		gameNS::MAX_NAME_LEN, OPTIONS_COLOR, ImGuiInputTextFlags_ReadOnly);
	int gamePlayers = 0, connectedPlayers = 0;
	connectedPlayers = (int)m_pClient->getConnectedPlayers(); SetNextItemWidth(30);
	gamePlayers = m_pClient->getGamePlayers();
	inputInt("Game Players", NOOPTIONS_COLOR, "##GamePlayers", &gamePlayers, NOOPTIONS_COLOR, 3);
	inputInt("Connected", NOOPTIONS_COLOR, "##Players", &connectedPlayers, NOOPTIONS_COLOR, 3);

	if (!m_pClient->isConnected())
	{
		if (button("Connect", Vec2(0, 0), Vec4(0, 1, 1, 1)))
			m_pClient->connect();
	}
	else if (m_pClient->isConnected())
	{
		if (button("Disconnect", Vec2(0, 0), Vec4(0, 1, 1, 1)))
			m_pClient->disconnect();

		SameLine();

		//if (button("Join game", Vec2(0, 0), Vec4(0.1f, 1, 0.6f, 1)));
	}
	*/
	endActivity(true, MAIN_ACTIVITY);
}

#else ifdef _SERVER_BUILD
void Interface::multiplayerMenu()
{
	pushSubMenu("Server Info");
	BeginChild("Server Config", ImVec2(g_gameInfo.width / 2, g_gameInfo.height*1.0f / 2.4f), true);
	title("Server Config");
	static char ip[netNS::IP_SIZE] = { 0 };
	inputText("IP", NOOPTIONS_COLOR, "##ServerIP", ip,
		netNS::IP_SIZE, NOOPTIONS_COLOR, ImGuiInputTextFlags_ReadOnly);
	Port* port = m_pServer->getPort();
	int iPort = 0;
	iPort = *port;
	ImGuiInputTextFlags configFlag = 0;
	Serverstatus status = m_pServer->getstatus();
	Vec4 colConfig = CHOOSE_COLOR;
	if (m_pServer->isStarted())
	{
		colConfig = NOOPTIONS_COLOR;
		configFlag = ImGuiInputTextFlags_ReadOnly;
	}

	inputInt("Port", NOOPTIONS_COLOR, "##ServerPort", &iPort,
		colConfig, 6, 0, 0, ImGuiInputTextFlags_CharsDecimal | configFlag);
	*port = iPort;
	text("Map", NOOPTIONS_COLOR); SameLine();
	BeginChild("Maps", ImVec2(g_gameInfo.width / 5, g_gameInfo.height / 5), true);
	static std::string selectedMap;
	PushStyleColor(ImGuiCol_Text, colConfig);
	static auto mapList = FileIO::getDirFileList(fileNS::MAP_DIR, 0, ".map", false);
	for (auto m : mapList)
	{
		bool select = (m.compare(selectedMap) == 0) ? true : false;
		if (Selectable(m.c_str(), select) && colConfig.operator==(CHOOSE_COLOR))
		{
			selectedMap = m;
			m_pAudio->play(SOUND_BUTTON_CLICKED);
		}
	}

	PopStyleColor();
	EndChild();
	SameLine();
	int players = 0;
	players = m_pServer->getGamePlayers();
	inputInt("Players", colConfig, "##Players", &players, colConfig, 2, 2, 32, ImGuiInputTextFlags_CharsDecimal | configFlag);
	m_pServer->setGamePlayers(players);

	if (status == SERVER_NOT_RUNNING)
	{
		if (button("Start"))
		{
			if (selectedMap.empty())
				selectedMap = m_pMap->loadRandom();
			else
				m_pMap->load(selectedMap.c_str());

			m_pServer->start();
			configFlag = ImGuiInputTextFlags_ReadOnly;
			m_pServer->getIP(ip);
		}
	}
	else if (m_pServer->isStarted())
		if (button("Stop"))
			m_pServer->stop();

	EndChild();
	BeginChild(" ", ImVec2(g_gameInfo.width / 3, g_gameInfo.height / 5), true);
	title("Server status"); SameLine(); text(strSERVER_status[status], colSERVER_status[status]);
	int connected = 0;
	connected = m_pServer->getConnectedPlayers();
	inputInt("Connected", NOOPTIONS_COLOR, "##ConnectedPlayers", &connected, NOOPTIONS_COLOR,
		0, 0, ImGuiInputTextFlags_ReadOnly);

	EndChild();
	BeginChild("Serverstatus", Vec2(g_gameInfo.width / 4, g_gameInfo.height / 2));
	const auto pClientData = m_pServer->getClientsData();
	for (size_t i = 0; i < pClientData->size(); i++)
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll;
		inputText("Player", NOOPTIONS_COLOR, " ", (char*)pClientData->at(i)->getName(), gameNS::MAX_NAME_LEN, NOOPTIONS_COLOR, flags);
		inputText("IP", NOOPTIONS_COLOR, " ", (char*)(pClientData->at(i)->getIP()), netNS::IP_SIZE, NOOPTIONS_COLOR, flags);
		int port = 0;
		port = pClientData->at(i)->getPort();
		inputInt("Port", NOOPTIONS_COLOR, " ", &port, NOOPTIONS_COLOR, 6, 0, 0, flags);
	}
	EndChild();
	popSubMenu();
}
#endif

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