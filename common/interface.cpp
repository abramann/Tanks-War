#include "interface.h"
#include "fileio.h"

using namespace ImGui;

const std::string stringCLIENT_STATE[] = { "Unconnected", "Disconnected", "Server map not found", "Error map could not load",
"Waiting server...", "Running" };
const auto OPTIONS_COLOR =  Vec4(0.83f, 0.83f, 0.70f, 0.875f);
const auto NOOPTIONS_COLOR = Vec4(0.83f, 0.83f, 0.70f, 0.6f);
const auto CHOOSE_COLOR = Vec4(0.98f, 0.99f, 0.0f, 1.0f);
#define SUB_MENU_SIZE Vec2(g_gameInfo.width / 4, g_gameInfo.height / 4)
constexpr Color COLOR_RAMADI = COLOR_XRGB(208, 247, 246);

enum FontSize
{
	SMALL,
	MED,
	LARGE
};

constexpr auto SEGA_FONT = "Assets\\Fonts\\NiseSegaSonic.ttf";
constexpr auto PROGGYCLEAN_FONT = "Assets\\Fonts\\proggyclean.ttf";
constexpr auto TAHOMA_FONT = "Assets\\Fonts\\tahoma.ttf";

Interface::Interface() : m_menu(MAIN_MENU)
{
}


Interface::~Interface()
{
}

#ifdef _SERVER_BUILD
void Interface::initialize(Server* server, Map* map, Audio* audio, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pAudio = audio;
	m_pServer = server;
	m_pMap = map;
	ImGuiIO& io = GetIO();
	m_font[SMALL] = io.Fonts->AddFontFromFileTTF(PROGGYCLEAN_FONT, 12);
	m_font[MED] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 19);
	m_font[LARGE] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 28);
	
}

#else ifdef _CLIENT_BUILD

void Interface::initialize(Client* client, Map* map, Audio* audio, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pAudio = audio;
	m_pClient = client;
	m_pMap = map;
	ImGuiIO& io = GetIO();
	m_font[SMALL] = io.Fonts->AddFontFromFileTTF(PROGGYCLEAN_FONT, 12);
	m_font[MED] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 19);
	m_font[LARGE] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 28);
}
#endif

void Interface::mainMenu()
{
	m_pAudio->playMusic(MUSIC_MAIN_MENU);
	PushFont(m_font[LARGE]);
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoTitleBar;
	Vec2 pos = Vec2(0, 0);
	Vec2 size = Vec2(g_gameInfo.width, g_gameInfo.height);
	Vec2 buttonSize = Vec2(size.x / 3, size.y / 5);
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2((size.x / 2) - buttonSize.x / 2, buttonSize.y / 2));
	ImGui::Begin("Main Menu", 0, flag);
	int8_t counter = 0;
	Menu menu = NO_MENU;
	for (auto item : { "Multiplayer","Setting","Quit" })
	{
		if (ImGui::Button(item, buttonSize))
		{
			menu = (Menu)counter;
			m_pAudio->play(SOUND_BUTTON_CLICKED);
		}

		counter++;;
	}

	ImGui::End();
	PopStyleVar();
	PopFont();

	switch (menu)
	{
	case 0:
		m_menu = MULTIPLAYER_MENU;
		break;
	case 1:
		m_menu = SETTING_MENU;
		break;
	case 2:
		m_menu = QUIT_MENU;
		break;
	default:
		break;
	}
}

void Interface::settingMenu()
{
	pushSubMenu("Setting");
	button("Graphics", Vec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR);

	BeginChild("Graphics");
	text("Resolution",OPTIONS_COLOR);
	BeginChild("Resolution", ImVec2(g_gameInfo.width / 8, g_gameInfo.height / 7), true);
	Resolution currResol = m_pGraphics->getResolution();
	for (auto resol : m_pGraphics->getSupportedResolutions())
	{
		PushStyleColor(ImGuiCol_Text, CHOOSE_COLOR);
		std::string sResol;
		sResol = std::to_string(resol.width);
		sResol.operator+=("x");
		sResol.operator+=(std::to_string(resol.height));
		bool select = false;
		if (currResol.height == resol.height && currResol.width == resol.width)
			select = true;

		if (Selectable(sResol.c_str(), select))
		{
			m_pGraphics->setResolution(resol);
			m_pAudio->play(SOUND_BUTTON_CLICKED);
		}

		PopStyleColor();
	}

	EndChild();
	bool fullScreen = m_pGraphics->isFullScreen();
	text("Fullscreen", OPTIONS_COLOR); SameLine(0, 5.0f);
	if (Checkbox("##Fullscreen", &fullScreen))
	{
		m_pGraphics->setFullScreen(fullScreen);
		m_pAudio->play(SOUND_BUTTON_CLICKED);
	}

	EndChild();
	popSubMenu();

}

void Interface::show()
{
	switch (m_menu)
	{
	case MAIN_MENU:
		mainMenu();
		break;
	case MULTIPLAYER_MENU:
		multiplayerMenu();
		break;
	case SETTING_MENU:
		settingMenu();
		break;
	case PLAYING_MENU:
	//	playerInterface();
		break;
	case QUIT_MENU:
		PostQuitMessage(0);
	default:
		break;
	}

}

void Interface::showFPS(uint16_t fps)
{
	SetNextWindowPos(Vec2(200, 0));
	if (GetAsyncKeyState(VK_RETURN))
		SetNextWindowFocus();
	Begin("fps", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	Text("FPS %d",fps);
	End();
}

#ifdef _CLIENT_BUILD
const ImVec4 colorCLIENT_STATE[] = { ImVec4(0.78f,0.78f,0.78f,0.5f),
ImVec4(0.78f,0.78f,0.78f,0.5f), ImVec4(0.78f,0.78f,0.78f,0.5f),ImVec4(0.78f,0.78f,0.78f,0.5f)
,ImVec4(0.78f,0.78f,0.78f,0.5f) ,ImVec4(0.78f,0.78f,0.78f,0.5f) ,ImVec4(0.78f,0.78f,0.78f,0.5f) };

void Interface::multiplayerMenu()
{
	pushSubMenu("Multiplayer");

	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	ClientState state = (ClientState)m_pClient->getState();
	if (state)
		configFlags = ImGuiInputTextFlags_ReadOnly;

	Vec4 colConfig = (m_pClient->isConnected()) ? NOOPTIONS_COLOR : CHOOSE_COLOR;

	BeginChild("Multiplayer", ImVec2(g_gameInfo.width / 3, g_gameInfo.height / 4), true);

	button("Multiplayer config", ImVec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR);
	inputText("Player Name", colConfig, "##PlayerName", m_pClient->getPlayerName(),
		MAX_NAME_LEN, colConfig, configFlags);
	inputText("Server IP", colConfig, "##ServerIP", (char*)m_pClient->getServerIP(),
		netNS::IP_SIZE, colConfig, configFlags);

	unsigned short* usPort = m_pClient->getServerPort();
	int iPort = (int)*usPort;
	inputInt("Port", colConfig, "##Port", &iPort, colConfig, 5);
	*usPort = iPort;
	EndChild();
	BeginChild("Server State", ImVec2(g_gameInfo.width / 2, g_gameInfo.height / 4), true);
	button("Server State", ImVec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR); SameLine();
	text(stringCLIENT_STATE[state].c_str(), colorCLIENT_STATE[state]);
	inputText("Game Map", NOOPTIONS_COLOR, "##GamerMap", m_pClient->getGameMap(),
		MAX_NAME_LEN, OPTIONS_COLOR, ImGuiInputTextFlags_ReadOnly);
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

		if (button("Join game", Vec2(0, 0), Vec4(0.1f, 1, 0.6f, 1)));
	}
	EndChild();
	popSubMenu();
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
	ServerState state = m_pServer->getState();
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
	BeginChild("Maps", ImVec2(g_gameInfo.width/5, g_gameInfo.height/5), true);
	static std::string selectedMap;
	PushStyleColor(ImGuiCol_Text, colConfig);
	static auto mapList = FileIO::getDirFileList(MAP_DIR, 0, ".map", false);
	for (auto m : mapList)
	{
		bool select =( m.compare(selectedMap) == 0) ? true : false;
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
	inputInt("Players", colConfig, "##Players", &players, colConfig, 2,2, 32, ImGuiInputTextFlags_CharsDecimal | configFlag);
	m_pServer->setGamePlayers(players);
	
	if (state == SERVER_NOT_RUNNING)
	{
		if (button("Start"))
		{
			if (selectedMap.empty())
				selectedMap = m_pMap->setRandomMap();

			m_pServer->start();
			m_pMap->load(selectedMap.c_str());
			configFlag = ImGuiInputTextFlags_ReadOnly;
			m_pServer->getIP(ip);
		}
	}
	else if (m_pServer->isStarted())
		if (button("Stop"))
			m_pServer->stop();

	EndChild();
	BeginChild(" ", ImVec2(g_gameInfo.width / 3, g_gameInfo.height / 5), true);
	title("Server State"); SameLine(); text(strSERVER_STATE[state], colSERVER_STATE[state]);
	int connected = 0;
	connected = m_pServer->getConnectedPlayers();
	inputInt("Connected", NOOPTIONS_COLOR, "##ConnectedPlayers", &connected, NOOPTIONS_COLOR,
		0, 0, ImGuiInputTextFlags_ReadOnly);

	EndChild();
//	SameLine();
	BeginChild("ServerState", Vec2(g_gameInfo.width / 4, g_gameInfo.height / 2));
	for (auto playerData : m_pServer->getClientsData())
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoHorizontalScroll;
		inputText("Player", NOOPTIONS_COLOR, " ", (char*)playerData.name, MAX_PLAYER_NAME, NOOPTIONS_COLOR, flags);
		inputText("IP", NOOPTIONS_COLOR," ",playerData.ip, netNS::IP_SIZE, NOOPTIONS_COLOR, flags);
		int port = 0;
		port = playerData.port;
		inputInt("Port", NOOPTIONS_COLOR, " ", &port, NOOPTIONS_COLOR, 6, 0, 0, flags);
	}
	EndChild();
	popSubMenu();
}

#endif

void Interface::title(const char* text, Vec4 colText, Vec4 colButton)
{
	button(text, ImVec2(0, 0), colText, colButton,
		colButton, colButton);
}

void Interface::fillWindow()
{
	SetNextWindowPos(Vec2(0, 0));
	SetNextWindowSize(Vec2(g_gameInfo.width, g_gameInfo.height));
}

void Interface::pushSubMenu(const char* str_id)
{
	PushStyleVar(ImGuiStyleVar_ChildBorderSize, 5);
	PushFont(m_font[MED]);
	fillWindow();
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(g_gameInfo.width / 10, g_gameInfo.height / 8));
	Begin("Sub Menu", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	PopStyleVar();
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);
	BeginChild(str_id, Vec2(g_gameInfo.width / 2, g_gameInfo.height*1.0f / 1.5f), true, ImGuiWindowFlags_NoTitleBar);
	PopStyleVar(2);
}

void Interface::popSubMenu()
{
	EndChild();
	PopFont();
	PushFont(m_font[LARGE]);
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(g_gameInfo.width*0.9, 1));
	if (Button("Back", Vec2(g_gameInfo.width / 10, g_gameInfo.height / 10)))
	{
		m_menu = MAIN_MENU;
		m_pAudio->play(SOUND_BUTTON_CLICKED);
	}

	PopStyleVar();
	PopFont();
	End();
}

bool Interface::button(const char* text, Vec2 size, Vec4 colText, Vec4 colButton,
	Vec4 colActive, Vec4 colHorvored, ImGuiButtonFlags flags)
{
	bool clicked = false;

	bool coText = (!colText.isEmpty()), coButton = (!colButton.isEmpty()),
		coActive = (!colActive.isEmpty()), coHorvered = (!colHorvored.isEmpty());
	
	if (coText)
		PushStyleColor(ImGuiCol_Text, colText);
	if (coButton)
		PushStyleColor(ImGuiCol_Button, colButton);
	if(coActive)
		PushStyleColor(ImGuiCol_ButtonActive, colActive);
	if(coHorvered)
		PushStyleColor(ImGuiCol_ButtonHovered, colHorvored);

	clicked = Button(text, size);
	if(clicked)
		m_pAudio->play(SOUND_BUTTON_CLICKED);

	PopStyleColor(coText + coButton + coActive + coHorvered);
	
	return clicked;
}

void Interface::inputText(const char* desc, const Vec4& descColor, const char* label,
	char* buf, const uint16_t& len, const Vec4& color, ImGuiInputTextFlags flags)
{
	text(desc, descColor); SameLine(0, g_gameInfo.width / 200);
	SetNextItemWidth(len*8);
	inputText(label, buf, MAX_NAME_LEN, color, flags);
}

void Interface::inputInt(const char* label, int* buf, const ImVec4& textColor,
	int32_t min, int32_t max, ImGuiInputTextFlags flags)
{
	PushStyleColor(ImGuiCol_Text, textColor);
	InputInt(label, buf, 0, 0, flags);	
	PopStyleColor();
	if (min == max)
		return;

	if (*buf > max)
		*buf = max;
	else if (*buf < min)
		*buf = min;
}

void Interface::inputInt(const char* desc, const Vec4& descColor, const char* label,
	int* buf, const Vec4& color, const int32_t elements, const int32_t min, const int32_t max, ImGuiInputTextFlags flags)
{
	text(desc, descColor); SameLine(0, g_gameInfo.width / 200);
	SetNextItemWidth(g_gameInfo.width / 15);
	inputInt(label, buf, color, min, max, flags);
}

void Interface::inputText(const char* label, char* buf, const uint16_t& len, const Vec4& textColor,
	ImGuiInputTextFlags flags)
{
	PushStyleColor(ImGuiCol_Text, textColor);
	InputText(label, buf, len, flags);
	PopStyleColor();
}

void Interface::text(const char* text, const Vec4 color)
{
	bool col = (!color.isEmpty());
	if (col)
		PushStyleColor(ImGuiCol_Text, color);
	Text(text);
	if (col)
		PopStyleColor();
}