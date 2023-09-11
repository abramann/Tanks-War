// data.h
// Author: abramann

#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui.h"
//#include <D3D11.h>
#include <map>

#pragma once

typedef ImGuiKey Key;

constexpr float PI = 3.1415926535897f;

namespace fileNS
{
	constexpr auto CLIENT_INFO_PATH = "Assets\\ini\\client-info.txt";
	constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
	constexpr auto TAHOMA_FONT_PATH = "Assets\\Fonts\\tahoma.ttf";
	constexpr auto ASSETS_DIR = "Assets\\";
	constexpr auto MAP_DIR = "Assets\\maps\\";
	constexpr auto AUDIO_DIR = "Assets\\audio\\";
	constexpr auto SERVER_INFO_PATH = "Assets\\ini\\server-info.txt";
	constexpr auto TEXTURE_DIR = "Assets\\texture\\";
}

namespace graphicsNS
{
	const auto VERTICES_IMAGE = 6;
	//constexpr auto  VERTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	constexpr auto CAMERA_FARPLANE = 1000.0f;
	constexpr auto CAMERA_FOV = 1.5707963f;
	constexpr auto CAMERA_NEARPLANE = 1.0f;
	constexpr auto PROJECT_FAR_PLANE = 1000.0f;
	constexpr auto PROJECT_FOV = PI / 2;
	constexpr auto PROJECT_NEAR_PLANE = 1.0f;
	constexpr auto FADE_INC = 0.01f;
}

namespace colorNS
{
	constexpr auto WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	constexpr auto RED = ImVec4(1.0f, 0, 0, 1.0f);
	constexpr auto GREEN = ImVec4(0.0f, 0.55f, 0.0f, 1.0f);
	constexpr auto BLUE = ImVec4(0.3f, 0.7f, 0.9f, 1.0f);
	constexpr auto BROWN = ImVec4(0.57f, 0.29f, 0.00f, 1.0f);
	constexpr auto ORANGE = ImVec4(0.96f, 0.64f, 0.58f, 1.0f);
	constexpr auto PURPLE = ImVec4(0.7f, 0.5f, 1.0f, 1.0f);
	constexpr auto TAN = ImVec4(0.8f, 0.7f, 0.6f, 1.0f);
	constexpr auto GREY = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	constexpr auto GREY_LIGHT = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	constexpr auto TEAL = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
	constexpr auto SILVER = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
	constexpr auto NAVY = ImVec4(0.0f, 0.0f, 0.5f, 1.0f);
	constexpr auto MAROON = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
	constexpr auto CYAN = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	constexpr auto LIME = ImVec4(0.5f, 1.0f, 0.0f, 1.0f);
	constexpr auto MAGENTA = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	constexpr auto YELLOW = ImVec4(0.95f, 0.95f, 0.00f, 1.0f);
}

namespace textureNS
{
	constexpr auto TEXTURE_BULLET_ROWS_COLUMNS = 3;
	constexpr auto TEXTURE_TANK_DESTROY_ROWS_COLUMNS = 8;
	constexpr auto 	TEXTURE_TILEDS = 9;
}

namespace mapNS
{
	const auto SPACE_VERTICES = 4;
	constexpr float UNDEFINED_POSITION = 0xFFFF;
}

namespace dxNS
{
	constexpr auto THREADS_PER_GROUP = 32;
}

namespace gameNS
{
	constexpr auto MAX_PLAYERS = 12;
	constexpr auto MAX_NAME_LEN = 20;
	constexpr auto FRAME_RATE = 60;
	constexpr auto  MIN_RESOLUTION_HEIGHT = 600;
	constexpr auto  MIN_RESOLUTION_WIDTH = 800;
	constexpr auto VSYNC_DEFAULT = 1;
	constexpr auto WINDOWED_DEFAULT = 1;
	constexpr auto UPDATE_DELAY_FPS = 800;
	constexpr auto AUDIO_DEFAULT = 1;
	constexpr auto COMPUTESHADER_DEFAULT = 0;
	constexpr auto LOGO_VIEW_TIME = 4000;
#ifdef _SRVER_BUILD
	constexpr auto WINDOW_TITLE = "Tanks War Server";
#else
	constexpr auto WINDOW_TITLE = "Tanks War";
#endif
	const std::map<std::string, uint32_t> gameFiles =
	{ { "Assets\\cursor.cur", 0x02d949be },
	  { "Assets\\Fonts\\tahoma.ttf", 0x44845b79 },
	  { "Assets\\Audio\\button-click.wav", 0x8dbea76c },
	  { "Assets\\Audio\\tank-attack.wav", 0x77ce040a },
	  { "Assets\\Texture\\bullet.png", 0x2e300ec7 },
	  { "Assets\\Texture\\bullet-destroy.png", 0x701cc174 },
	  { "Assets\\Texture\\concrete.bmp", 0x88764328 },
	  { "Assets\\Texture\\enemy-tank.png", 0x4d3c3ba4 },
	  { "Assets\\Texture\\logo.png", 0x50dbf423 },
	  { "Assets\\Texture\\player-tank.png", 0xd06d54ad },
	  { "Assets\\Texture\\tank-destroy.png", 0x35f50b9e },
	  { "Assets\\Texture\\tiled-0.png", 0xed106bdf },
	  { "Assets\\Texture\\tiled-1.png", 0xe8853b23 } };
}

namespace logicNS
{
	constexpr auto CAMERA_HEIGHT = -300.0f;
	constexpr auto BULLET_DAMAGE = 40.0f;
	constexpr auto BULLET_SPEED = 8.0f;
	constexpr auto HEALTH_TANK = 100.0f;
	constexpr auto TANK_MAX_ANGLE = PI - 0.01f;
	constexpr auto TANK_ROTATE_AMOUNT = PI / 64.0f;
	constexpr auto UPDATE_DELAY_BULLET = 80;
	constexpr auto UPDATE_DELAY_IMAGE = 50;
	constexpr auto UPDATE_DELAY_TANK_DESTORY = 100;
	constexpr auto UPDATE_DELAY_TANK_DESTROY = 100;
	constexpr auto VELOCITY_TANK = 3.0f;
	constexpr auto DEFAULT_BITMAP_DAMAGEFACTOR = 1.0f;
	constexpr auto DEFAULT_BITMAP_VELOCITYFACTOR = 1.0f;

}

namespace networkNS
{
	constexpr auto HEARTBEAT_DELAY = 40000;
	constexpr auto MAX_PACKET_SIZE = 256;
	constexpr auto SERVER_TIME_OUT = 60000;
	constexpr unsigned short START_CLIENT_PORT = 10;
	constexpr unsigned short END_CLIENT_PORT = 3000;
	constexpr int INVALID_ID = -1;
	constexpr unsigned short UNSPECIFIED_PORT = 0xCCCC;
}

namespace inputNS
{
	constexpr Key A_KEY = ImGuiKey_A;
	constexpr Key B_KEY = ImGuiKey_B;
	constexpr Key C_KEY = ImGuiKey_C;
	constexpr Key D_KEY = ImGuiKey_D;
	constexpr Key E_KEY = ImGuiKey_E;
	constexpr Key F_KEY = ImGuiKey_F;
	constexpr Key G_KEY = ImGuiKey_G;
	constexpr Key H_KEY = ImGuiKey_H;
	constexpr Key I_KEY = ImGuiKey_I;
	constexpr Key J_KEY = ImGuiKey_J;
	constexpr Key K_KEY = ImGuiKey_K;
	constexpr Key L_KEY = ImGuiKey_L;
	constexpr Key M_KEY = ImGuiKey_M;
	constexpr Key N_KEY = ImGuiKey_N;
	constexpr Key O_KEY = ImGuiKey_O;
	constexpr Key P_KEY = ImGuiKey_P;
	constexpr Key Q_KEY = ImGuiKey_Q;
	constexpr Key R_KEY = ImGuiKey_R;
	constexpr Key S_KEY = ImGuiKey_S;
	constexpr Key T_KEY = ImGuiKey_T;
	constexpr Key U_KEY = ImGuiKey_U;
	constexpr Key V_KEY = ImGuiKey_V;
	constexpr Key W_KEY = ImGuiKey_W;
	constexpr Key X_KEY = ImGuiKey_X;
	constexpr Key Y_KEY = ImGuiKey_Y;
	constexpr Key Z_KEY = ImGuiKey_Z;
	constexpr Key ENTER_KEY = ImGuiKey_Enter;
	constexpr Key ESCAPE_KEY = ImGuiKey_Escape;
	constexpr Key TAB_KEY = ImGuiKey_Tab;
	constexpr Key COMMA_KEY = ImGuiKey_Comma;
	constexpr Key SPACE_KEY = ImGuiKey_Space;
	constexpr Key BACKSPACE_KEY = ImGuiKey_Backspace;
	constexpr Key RSHIFT_KEY = ImGuiKey_RightShift;
	constexpr Key LSHIFT_KEY = ImGuiKey_LeftShift;
	constexpr Key F1_KEY = ImGuiKey_F1;
	constexpr Key F2_KEY = ImGuiKey_F2;
	constexpr Key F3_KEY = ImGuiKey_F3;

};

namespace interfaceNS
{
	constexpr float MAINACTIVITY_BUTTON_PADDING_Y = 0.05f;
	enum _FontSize
	{
		FONTSIZE_TINY,
		FONTSIZE_SMALL,
		FONTSIZE_SMALL2,
		FONTSIZE_MED,
		FONTSIZE_MED2,
		FONTSIZE_LARGE,
		FONTSIZE_LARGE2,
		FONTSIZES,
		FONTSIZE_DEFAULT
	};
	typedef int8_t FontSize;

	enum _Activity
	{
		MAIN_ACTIVITY,
		MULTIPLAYER_ACTIVITY,
		SETTINGS_ACTIVITY,
		PLAYING_ACTIVITY,
		CREDITS_ACTIVITY,
		QUIT_ACTIVITY,
		ACTIVITIES,
		NO_ACITVITY
	};
	typedef int8_t Activity;

	enum _InputFieldListType
	{
		LIST_VERTICAL,
		LIST_HORIOZONTIAL,
		LIST_MAIN,
		LIST_NONE
	};
	typedef int8_t ListType;
}

namespace serverNS
{
	enum _ServerStatus
	{
		SERVER_NOT_RUNNING,
		SERVER_RUNNING_HANDLING,
		SERVER_DISCONNECTED
	};
	typedef int8_t ServerStatus;

	static std::map<ServerStatus, std::pair<const char*, ImVec4>>  SERVER_STATUS = {
		{ SERVER_NOT_RUNNING,{ "Not Started", colorNS::BROWN } },
		{ SERVER_RUNNING_HANDLING,{ "Handling Requests...", colorNS::ORANGE } },
		{ SERVER_DISCONNECTED,{ "Disconnected", colorNS::RED } }
	};
}

namespace clientNS
{
	enum _ClientStatus
	{
		CLIENT_UNCONNECTED,
		CLIENT_DISCONNECTED,
		CLIENT_CONNECTED
	};
	typedef int8_t ClientStatus;

	static std::map<int, std::pair<const char*, ImVec4>>  CLIENT_STATUS = {
		{ CLIENT_UNCONNECTED,{ "Not Connected", colorNS::BROWN } },
		{ CLIENT_DISCONNECTED,{ "Disconnected", colorNS::RED } },
		{ CLIENT_CONNECTED,{ "Connected",colorNS::ORANGE } }
	};
}

enum MatrixType
{
	MATRIX_TYPE_TRANSLATE,
	MATRIX_TYPE_SCALL,
	MATRIX_TYPE_ROTATE,
};

enum Textures
{
	BLACK,
	PLAYER_TANK,
	TANK_DESTROY,
	FIRE_SIMPLE,
	FIRE_BULLET6,
	FIRE2,
	FIRE_HIT,
	ENEMY_TANK
};

enum KeyControl
{
	KEY_OBJECT_FORWARD,
	KEY_OBJECT_BACK,
	KEY_OBJECT_RIGHT,
	KEY_OBJECT_LEFT,
	KEY_TANK_ATTACK,
};

enum PacketType_
{
	PACKET_START_SEASSON = 100,
	PACKET_DISCONNECT,
	PACKET_SERVER_SHUTDOWN,
	PACKET_CLIENT_DISCONNECTED,
	PACKET_CLIENT_HEARTBEAT,
	PACKET_CLIENT_JOIN,
	PACKET_CLIENT_INITIAL_DATA,
	PACKET_CLIENTS_UPDATE,
	PACKET_CLIENT_GAME_STATE,
	PACKET_CLIENT_UPDATE,
	PACKET_CLIENT_ACT,
	PACKET_CLIENT_GAME_ATTRIBUTE
};
typedef char PacketType;

enum gameTexure
{
	/*TEXTURE_LOGO,
	TEXTURE_PLAYER_TANK,
	TEXTURE_ENEMY_TANK,
	TEXTURE_BULLET,
	TEXTURE_BULLET_DESTROY,
	TEXTURE_TILED0,
	TEXTURE_TILED1,
	TEXTURE_TILED2,
	TEXTURES*/
};

enum PlayerAct_
{
	PLAYER_ACT_NONE,
	PLAYER_ACT_FORWRAD,
	PLAYER_ACT_BACK,
	PLAYER_ACT_RIGHT,
	PLAYER_ACT_FORWARD_RIGHT,
	PLAYER_ACT_BACK_RIGHT,
	PLAYER_ACT_LEFT,
	PLAYER_ACT_FORWARD_LEFT,
	PLAYER_ACT_BACK_LEFT,
	PLAYER_ACT_DIE,
	PLAYER_ACT_ATTACK = 100
};