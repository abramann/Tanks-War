#include "interface.h"
#include "fileio.h"

using namespace ImGui;
const auto ZERO_V4 = Vec4(0, 0, 0, 0);

const std::string stringCLIENT_STATE[] = { "Unconnected", "Connected", "Disconnected", "Waiting" };
const auto OPTIONS_COLOR =  Vec4(0.83f, 0.83f, 0.70f, 0.875f);
const auto NOOPTIONS_COLOR = Vec4(0.83f, 0.83f, 0.70f, 0.6f);
const auto CHOOSE_COLOR = Vec4(0.98f, 0.99f, 0.0f, 1.0f);
#define SUB_MENU_SIZE Vec2(g_gameInfo.width / 4, g_gameInfo.height / 4)
constexpr Color COLOR_RAMADI = COLOR_XRGB(208, 247, 246);

enum Fonts
{
	PROGGYCLEAN,
	SEGA,
	TAHOMA
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

void Interface::initialize()
{
	ImGuiIO& io = GetIO();
	m_font[PROGGYCLEAN] = io.Fonts->AddFontFromFileTTF(PROGGYCLEAN_FONT, 12);
	m_font[SEGA] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 28);
	m_font[TAHOMA] = io.Fonts->AddFontFromFileTTF(TAHOMA_FONT, 19);
	
}

void Interface::mainMenu()
{
	PushFont(m_font[SEGA]);
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoTitleBar;
	Vec2 pos = Vec2(0, 0);
	Vec2 size = Vec2(g_gameInfo.width, g_gameInfo.height);
	Vec2 buttonSize = Vec2(size.x / 3, size.y / 5);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2((size.x / 2) - buttonSize.x / 2, buttonSize.y / 2));
	int8_t moveMenu = menu(" ", pos,  size, buttonSize, flag, { "Multiplayer","Setting","Quit" });
	ImGui::PopStyleVar();
	switch (moveMenu)
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

	PopFont();
}

void Interface::settingMenu(Graphics& graphics, Audio& audio)
{
	pushSubMenu("Setting");
	button("Graphics", Vec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR);

	BeginChild("Graphics");
	text("Resolution",OPTIONS_COLOR);
	BeginChild("Resolution", ImVec2(g_gameInfo.width / 8, g_gameInfo.height / 7), true);
	Resolution currResol = graphics.getResolution();
	for (auto resol : graphics.getSupportedResolutions())
	{
		PushStyleColor(ImGuiCol_Text, CHOOSE_COLOR);
		std::string sResol;
		sResol = std::to_string(resol.width);
		sResol.operator+=("x");
		sResol.operator+=(std::to_string(resol.height));
		bool select = false;
		if (currResol.height == resol.height & currResol.width == resol.width)
			select = true;

		if (Selectable(sResol.c_str(), select))
			graphics.setResolution(resol);

		PopStyleColor();
	}

	EndChild();
	bool fullScreen = graphics.isFullScreen();
	text("Fullscreen", OPTIONS_COLOR); SameLine(0, 5.0f);
	if (Checkbox("##Fullscreen", &fullScreen))
		graphics.setFullScreen(fullScreen);

	EndChild();

	popSubMenu();

}

#ifdef _CLIENT_BUILD
const ImVec4 colorCLIENT_STATE[] = { ImVec4(0.78f,0.78f,0.78f,0.5f),
ImVec4(0.78f,0.78f,0.78f,0.5f), ImVec4(0.78f,0.78f,0.78f,0.5f),ImVec4(0.78f,0.78f,0.78f,0.5f) };

void Interface::multiplayerMenu(Client& client)
{
	pushSubMenu("Multiplayer");

	ImGuiInputTextFlags configFlags = ImGuiInputTextFlags_None;
	ClientState state = (ClientState)client.getState();
	if (state)
		configFlags = ImGuiInputTextFlags_ReadOnly;
	
	BeginChild("Multiplayer", ImVec2(g_gameInfo.width/3,g_gameInfo.height/4), true);

	button("Multiplayer config", ImVec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR);
	inputText("Player Name", OPTIONS_COLOR, "##PlayerName", client.getPlayerName(),
		MAX_PLAYER_NAME, CHOOSE_COLOR, configFlags); 
	inputText("Server IP", OPTIONS_COLOR, "##ServerIP", (char*)client.getServerIP(),
		netNS::IP_SIZE, CHOOSE_COLOR, configFlags);
	
	unsigned short* usPort = client.getServerPort();
	int iPort = (int)*usPort;
	text("Port", OPTIONS_COLOR); SameLine(0, 2); PushStyleColor(ImGuiCol_Text, CHOOSE_COLOR); InputInt("##ServerPort", (int*)&iPort, 0, 0, ImGuiInputTextFlags_CharsDecimal | configFlags); PopStyleColor();

	*usPort = iPort;
	EndChild();

	BeginChild("Server State", ImVec2(g_gameInfo.width / 2, g_gameInfo.height / 4), true);
	button("Server State", ImVec2(0, 0), TITLE_BUTTON_TEXT_COLOR, TITLE_BUTTON_COLOR,
		TITLE_BUTTON_COLOR, TITLE_BUTTON_COLOR); SameLine();
	text(stringCLIENT_STATE[state].c_str(), colorCLIENT_STATE[state]);
	inputText("Game Map", OPTIONS_COLOR, "##GamerMap", client.getGameMap(),
		MAX_NAME_LEN, OPTIONS_COLOR, ImGuiInputTextFlags_ReadOnly);
	text("Players", OPTIONS_COLOR); SameLine();
	int nPlayer = 0;
	nPlayer = (int)client.getGamePlayers();
	PushStyleColor(ImGuiCol_Text, OPTIONS_COLOR); InputInt("##Server Players", (int*)&nPlayer, 0, 0, ImGuiInputTextFlags_ReadOnly); PopStyleColor();
	const char* connection;
	if (state == CLIENT_UNCONNECTED || state== CLIENT_DISCONNECTED)
	{
		if (button("Connect", Vec2(0, 0), Vec4(0, 1, 1, 1)))
			client.connect();
	}
	else if (state == CLIENT_CONNECTED || CLIENT_WAITING || CLIENT_RUNNING)
	{
		if (button("Disconnect", Vec2(0, 0), Vec4(0, 1, 1, 1)))
				client.disconnect();

		SameLine();

		if (button("Join game", Vec2(0, 0), Vec4(0.1f, 1, 0.6f, 1)));
	}
	EndChild();
	popSubMenu();
}

#else ifdef _SERVER_BUILD

void Interface::multiplayerMenu(Server& server, Map& map)
{
	pushSubMenu("Server Info"); 
	
	BeginChild("Server Config", ImVec2(g_gameInfo.width / 2, g_gameInfo.height*1.0f / 2.4f), true);
	title("Server Config");
	inputText("IP", NOOPTIONS_COLOR, "##ServerIP", (char*)server.getIP(),
		netNS::IP_SIZE, NOOPTIONS_COLOR, ImGuiInputTextFlags_ReadOnly);
	Port* port = server.getPort();
	int iPort = 0;
	iPort = *port;
	ImGuiInputTextFlags configFlag = 0;
	ServerState state = server.getState();
	Vec4 colConfig = CHOOSE_COLOR;
	if (state != SERVER_NOT_RUNNING)
		colConfig = NOOPTIONS_COLOR;
	if (state != SERVER_NOT_RUNNING)
		configFlag = ImGuiInputTextFlags_ReadOnly;

	inputInt("Port", NOOPTIONS_COLOR, "##ServerPort", &iPort,
		colConfig, MIN_PORT, MAX_PORT, ImGuiInputTextFlags_CharsDecimal | configFlag);

	*port = iPort;

	text("Map", NOOPTIONS_COLOR); SameLine();
	BeginChild("Maps", ImVec2(g_gameInfo.width/5, g_gameInfo.height/5), true);
	static std::string selectedMap;
	PushStyleColor(ImGuiCol_Text, colConfig);
	for (auto m : FileIO::getDirFileList(MAP_DIR, 0, ".map", false))
	{
		bool select = (m == selectedMap) ? true : false;
		if (Selectable(m.c_str(), select) && colConfig.operator==(CHOOSE_COLOR))
			selectedMap = m;
	}

	PopStyleColor();
	EndChild();
	SameLine();
	int players = 0;
	players = server.getGamePlayers();
	inputInt("Players", OPTIONS_COLOR, "##Players", &players, colConfig, 2, 32, ImGuiInputTextFlags_CharsDecimal | configFlag);
	server.setGamePlayers(players);
	
	if (state == SERVER_NOT_RUNNING)
	{
		if (button("Start"))
		{
			if (selectedMap.empty())
				selectedMap = map.setRandomMap();

			server.start();
			map.load("Nova");
			configFlag = ImGuiInputTextFlags_ReadOnly;
		}
	}
	else if (state == SERVER_WAITING || state == SERVER_HANDLING)
	{
		if (button("Stop"))
			server.stop();
	}
	EndChild();

	BeginChild(" ", ImVec2(g_gameInfo.width / 3, g_gameInfo.height / 5), true);
	title("Server State"); SameLine(); text(strSERVER_STATE[state], colSERVER_STATE[state]);
	int connected = 0;
	connected = server.getConnectedPlayers();
	inputInt("Connected Players", NOOPTIONS_COLOR, "##ConnectedPlayers", &connected, NOOPTIONS_COLOR,
		0, 0, ImGuiInputTextFlags_ReadOnly);

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
	PushFont(m_font[TAHOMA]);
	fillWindow();
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(g_gameInfo.width / 10, g_gameInfo.height / 8));
	Begin(" ", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	PopStyleVar();
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);
	BeginChild(str_id, Vec2(g_gameInfo.width / 2, g_gameInfo.height*1.0f / 1.5f), true, ImGuiWindowFlags_NoTitleBar);
	PopStyleVar(2);
}

void Interface::popSubMenu()
{
	EndChild();
	PopFont();
	PushFont(m_font[SEGA]);
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(g_gameInfo.width*0.9, 1));
	if (Button("Back", Vec2(g_gameInfo.width / 10, g_gameInfo.height / 10)))
		m_menu = MAIN_MENU;

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
	PopStyleColor(coText + coButton + coActive + coHorvered);
	
	return clicked;
}

void Interface::inputText(const char* desc, const Vec4& descColor, const char* label,
	char* buf, const uint16_t& len, const Vec4& color, ImGuiInputTextFlags flags)
{
	text(desc, descColor); SameLine(0, g_gameInfo.width / 200);
	inputText(label, buf, MAX_PLAYER_NAME, color, flags);
}

void Interface::inputInt(const char* label, int* buf, const ImVec4& textColor,
	int32_t min, int32_t max, ImGuiInputTextFlags flags)
{
	PushStyleColor(ImGuiCol_Text, textColor);
	InputInt(label, buf, 0, 0, flags);
	if (*buf > max)
		*buf = max;
	else if (*buf < min)
		*buf = min;
	
	PopStyleColor();

}

void Interface::inputInt(const char* desc, const Vec4& descColor, const char* label,
	int* buf, const Vec4& color, const int32_t min, const int32_t max, ImGuiInputTextFlags flags)
{
	text(desc, descColor); SameLine(0, g_gameInfo.width / 200);
	inputInt(label, buf, color, min, max, flags);
}

Menus Interface::menu(const char* name, const Vec2 pos, const Vec2 size, const Vec2 buttonSize, ImGuiWindowFlags flag, std::initializer_list<const char*> items)
{
	Menus result = NO_MENU;
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);
	ImGui::Begin(name, 0, flag);
	int8_t counter = 0;
	for (auto item : items)
	{
		if (ImGui::Button(item, buttonSize))
			result = (Menus)counter;

		counter++;;
	}

	ImGui::End();
	return result;
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