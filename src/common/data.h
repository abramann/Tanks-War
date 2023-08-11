#include "imgui\imgui_impl_win32.h"
#include <D3D11.h>

#pragma once

typedef ImGuiKey Key;

constexpr float PI = 3.1415926535897f;

namespace fileNS
{
	constexpr auto CLIENT_INFO_PATH = "Assets\\ini\\client-info.txt";
	constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
	constexpr auto MAP_DIR = "Assets\\maps\\";
	constexpr auto SERVER_INFO_PATH = "Assets\\ini\\server-info.txt";
	constexpr auto TEXTURE_DIR = "Assets\\texture\\";
}

namespace graphicsNS
{
	const auto VERTICES_IMAGE = 6;
	//constexpr auto  VERTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	constexpr auto CAMERA_FARPLANE = 1000.0f;
	constexpr auto CAMERA_FOV = 1.5707963f;
	constexpr auto CAMERA_HEIGHT = -300.0f;
	constexpr auto CAMERA_NEARPLANE = 1.0f;
	constexpr auto COLOR_ALPHA = 111;// COLOR_ARGB(0, 0, 0, 255);
	constexpr auto COLOR_BLACK = 1212;// COLOR_XRGB(0, 0, 0);
	constexpr auto COLOR_WHITE = 4444;// COLOR_XRGB(255, 255, 255);
	constexpr auto PROJECT_FAR_PLANE = 1000.0f;
	constexpr auto PROJECT_FOV = PI / 2;
	constexpr auto PROJECT_NEAR_PLANE = 1.0f;
}

namespace textureNS
{
	constexpr auto TEXTURE_BULLET_ROWS_COLUMNS = 3;
	constexpr auto TEXTURE_TANK_DESTROY_ROWS_COLUMNS = 8;
	constexpr auto 	TEXTURE_TILEDS = 3;
}

namespace mapNS
{
	const auto SPACE_VERTICES = 4;
	constexpr float UNDEFINED_POSITION = 0xFFFF;
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
	constexpr auto UPDATE_DELAY_FPS = 5;
}

namespace logicNS
{
	constexpr auto UPDATE_DELAY_IMAGE = 50;
	constexpr auto TANK_ROTATE_AMOUNT = PI / 64.0f;
	constexpr auto UPDATE_DELAY_BULLET = 80;
	constexpr auto UPDATE_DELAY_TANK_DESTROY = 100;
	constexpr auto HEALTH_TANK = 100.0f;
	constexpr auto VELOCITY_TANK = 10.0f;
	constexpr auto UPDATE_DELAY_TANK_DESTORY = 100;
	constexpr auto TANK_MAX_ANGLE = PI - 0.01f;
	constexpr auto BULLET_SPEED = 20.0f;
	constexpr auto BULLET_DAMAGE = 40.0f;
}

namespace networkNS
{
	constexpr auto CLIENT_PRESENT_TIME = 5000;
	constexpr auto MAX_PACKET_SIZE = 256;
	constexpr auto SERVER_RECIEVE_PRESENT_TIME = 10000;
	constexpr int MAX_PORT = 4861;
	constexpr int MIN_PORT = 10; // already in netNS
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
	constexpr Key SPACE_KEY = ImGuiKey_Space;
	constexpr Key BACKSPACE_KEY = ImGuiKey_Backspace;
	constexpr Key RSHIFT_KEY = ImGuiKey_RightShift;
	constexpr Key LSHIFT_KEY = ImGuiKey_LeftShift;
};

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

enum Menu
{
	NO_MENU = -1,
	MAIN_MENU = 100,
	MULTIPLAYER_MENU,
	SETTING_MENU,
	PLAYING_MENU,
	QUIT_MENU
};

enum ClientState
{
	CLIENT_UNCONNECTED,
	CLIENT_UNCONNECTED_DISCONNECT,
	CLIENT_UNCONNECTED_MAP_NOT_FOUND,
	CLIENT_UNCONNECTED_MAP_NOT_LOAD,
	CLIENT_CONNECTED_WAITING,
	CLIENT_CONNECTED_PLAYING
};

enum ServerState
{
	SERVER_NOT_RUNNING,
	SERVER_RUNNING_WAITING,
	SERVER_RUNNING_PREPGAME,
	SERVER_RUNNING_HANDLING,
};

enum VB_USAGE
{
#ifdef _BUILD_WITH_D3D9
	VB_USAGE_CONST = D3DUSAGE_WRITEONLY,
	VB_USAGE_WRITE = D3DUSAGE_DYNAMIC
#else ifdef _BUILD_WITH_D3D11
	VB_USAGE_CONST = D3D11_USAGE_IMMUTABLE,
	VB_USAGE_WRITE = D3D11_USAGE_DYNAMIC
#endif
};

enum PacketType_
{
	PACKET_START_SEASSON = 100,
	PACKET_INI,
	PACKET_PLAYERS_EXIST,
	PACKET_PLAYERS_INI_DATA,
	PACKET_DISCONNECT,
	PACKET_PLAYER_DISCONNECTED,
	PACKET_PRESENT_CLIENT,
	PACKET_PLAYERS_INI,
	PACKET_NEW_PLAYER,
	PACKET_PLAYERS_UPDATE,
	PACKET_PLAYER_UPDATE,
	PACKET_PLAYER_ACT
};

enum gameTexure
{
	TEXTURE_LOGO,
	TEXTURE_PLAYER_TANK,
	TEXTURE_ENEMY_TANK,
	TEXTURE_TANK_DESTROY,
	TEXTURE_BULLET,
	TEXTURE_BULLET_DESTROY,
	TEXTURE_TILED0,
	TEXTURE_TILED1,
	TEXTURE_TILED2,
	TEXTURES
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
	PLAYER_ACT_ATTACK = 100
};