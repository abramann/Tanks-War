// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "gameerror.h"
#include "imgui\imgui_impl_win32.h"
#include<iostream>
#include<vector>
#include<Windows.h>
#include <d3dx9.h>
#include <timeapi.h>
#include "net.h"
#include "crc32.h"

extern uint64_t g_frameCounter;

#define COLOR_ARGB D3DCOLOR_ARGB
#define COLOR_XRGB D3DCOLOR_XRGB
#define TANK_KEYS			5
#define TANK_KEYS			5
#define TANK_KEYATTACK		4
#define TANK_EFFECTS		7
#define TANK_EFFECTSHOT		5
#define TANK_EFFECTHIT		6

typedef D3DCOLOR Color;
typedef D3DXVECTOR2 V2;
typedef uint8_t PlayerID;
typedef ImGuiKey Key;
typedef unsigned short Port;
typedef uint8_t Protocol;
typedef uint32_t Crc32;

constexpr auto COLOR_BLACK = COLOR_XRGB(0, 0, 0);
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);
constexpr auto COLOR_ALPHA = COLOR_ARGB(0, 0, 0, 255);
constexpr double PI = 3.14159265;
constexpr float MIN_FRAME_TIME = 10.0f;
constexpr float MAX_FRAME_TIME = 80.0f;
constexpr auto MAX_PLAYER_NAME = 20;
constexpr auto MAX_PACKET_SIZE = 255;
constexpr DWORD INVALID_ADDRESS = 0xFFFFFFF;
constexpr uint8_t INVALID_ID = -1;
constexpr auto NET_RESPONSE = 1;
constexpr auto NET_NORESPONSE = -1;
constexpr auto MAX_PLAYERS = 12;
constexpr int MAX_PORT = 4861;
constexpr int MIN_PORT = 10;
constexpr unsigned short UNSPECIFIED_PORT = 0xCCCC;
constexpr uint32_t UNDEFINED_POSITION = 0xFFFF;
constexpr uint8_t MAX_FILE_NAME = 255;
constexpr auto INVALID_DATA = 0xFFFF;
constexpr BYTE BYTE_INVALID_DATA = 0xFF;
constexpr auto  VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
constexpr auto OBJECT_INFO_PATH = "Assets\\ini\\object-info.txt";
constexpr auto TANK_INFO_PATH = "Assets\\ini\\tank-info.txt";
constexpr auto IMAGE_INFO_PATH = "Assets\\ini\\image-info.txt";
constexpr auto CLIENT_INFO_PATH = "Assets\\ini\\client-info.txt";
constexpr auto SERVER_INFO_PATH = "Assets\\ini\\server-info.txt";
constexpr auto FIRE_DATA_PATH = "Assets\\ini\\fire-info.txt";
constexpr auto TEXTURE_DIR = "Assets\\texture\\";
constexpr auto MAP_DIR = "Assets\\maps\\";
constexpr auto MAX_NAME_LEN = 20;
constexpr auto MD5_LEN = 16;

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

#define Vec2 ImVec2
#define Vec4 ImVec4
constexpr const char* strSERVER_STATE[] = { "Not started" , "Waiting for players...", "Running" };
constexpr Vec4 colSERVER_STATE[] = { Vec4(0.7f,0.7f,0.7f,0.5f), Vec4(0.87f,0.77f,0,1), Vec4(0,1,0,1) };

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

inline void waitTime(float time)
{
	Sleep(time);
}

inline void waitFrame(int framesWait)
{
	framesWait += ::g_frameCounter;
	while (true)
		if (framesWait < ::g_frameCounter)
			break;
}

inline std::string getTargetEqualStringValue(std::string str)
{
	str.erase(0, str.find('=') + 1);
	return str;
}

inline int8_t sign(float value)
{
	return (value >= 0) ? 1 : -1;
}

inline float _round(float value)
{
	float rValue = std::ceil(value * 10000) / 10000;
	if (abs(rValue) > 1)
		return sign(rValue) * 1;
	return rValue;
}

inline int strComp(std::string s1, std::string s2)
{
	return strcmp(s1.c_str(), s2.c_str());
}

inline void getFileNameFromPath(std::string path, char* name)
{
	std::string sName = path.substr(path.find_last_of('\\') + 1,
		path.find_last_of('.') - path.find_last_of('\\') - 1);

	memset(name, 0, sName.length()+1);
	strcpy(name, sName.c_str());
}

inline uint32_t _rand(uint32_t max)
{
	return ::GetTickCount() % max;
}

struct Vertex
{
	float x, y, z, rhw;
	float u, v;
};

struct TextureVertices
{
	Vertex v1, v2, v3;
	Vertex v4, v5, v6;
};

struct SpriteData
{
	LPDIRECT3DTEXTURE9 lpTexture;
	uint16_t width, height;
	uint16_t textureWidth, textureHeight;
	float x, y;
	uint8_t columns, rows;
	float angle;
	V2 scalling;
	RECT rect;
	Color filterColor;
	V2 center;
};

struct GameInfo
{
	uint8_t fullScreen;
	uint16_t width, height;
};

struct ImageInfo
{
	uint16_t width, height;
	uint8_t columns, rows;
	bool animate;
	float animateSpeed, scalling;
};

struct ObjectInfo
{
	float health, speed;
	uint8_t deathTexture;
};

struct TankInfo
{
	uint8_t fireTexture;
};

struct FireInfo
{
	float speed;
	float damage;
	uint8_t endTexture;
};

struct ObjectData
{
	float x, y;
	uint16_t width, height;
	float angle;
};

struct BitmapData
{
	uint16_t width, height;
};

struct TextureInfo
{
	ImageInfo* imageInfo;
	ObjectInfo* objectInfo;
	TankInfo* tankInfo;
	FireInfo* fireInfo;
};

struct MapData
{
	std::string name;
	uint16_t width, height;
	uint8_t bitmaps;
	std::vector<uint8_t> preventedBM;
};

struct Space
{
	int32_t x1, y1;
	int32_t x2, y2;
};

struct ServerInfo
{
	Port port;
	uint8_t players;
	char map[MAX_NAME_LEN];
};

enum TEXTURES
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

struct ClientInfo
{
	char name[MAX_NAME_LEN], serverIP[netNS::IP_SIZE];
	Port serverPort;
};

struct ClientData
{
	char ip[netNS::IP_SIZE], name[MAX_NAME_LEN];
	Port port;
	PlayerID id;
};

enum PacketType
{
	PACKET_START_SEASSON = 100,
	PACKET_END_SEASSON,
	PACKET_INI,
	PACKET_PLAYERS_EXIST,
	PACKET_PLAYERS_INI_DATA,
	PACKET_GAME,
	PACKET_DISCONNECT,
};

struct CpsIni
{
	PacketType  packetType = PACKET_INI;
	char name[MAX_NAME_LEN];
};

struct CpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
};

struct CpsSeasson
{
	PacketType packetType;
};

struct SpsIni
{
	PacketType packetType = PACKET_INI;
	bool accept;
	PlayerID id;
	uint8_t playersInServer;
	uint8_t gamePlayers;
	char  map[MAX_NAME_LEN];
	Crc32 checksum;
};

struct SpsPlayersExist
{
	PacketType packetType = PACKET_PLAYERS_EXIST;
	uint8_t players;
};

struct SpsPlayersIniData
{
	PacketType packetType = PACKET_PLAYERS_INI_DATA;
	char playerName[MAX_PLAYERS][MAX_NAME_LEN];
	PlayerID id[MAX_PLAYERS];
};

struct SpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
};

enum READ_TYPE
{
	GAME_INFO,
	IMAGE_INFO,
	OBJECT_INFO,
	TANK_INFO,
	FIRE_INFO,
	MAP_INFO
};

enum OBJECT_KEYS {
	KEY_FORWARD,
	KEY_BACK,
	KEY_RIGHT,
	KEY_LEFT
};

enum OBJECT_EFFECTS {
	EFFECT_FORWARD,
	EFFECT_BACK,
	EFFECT_RIGHT,
	EFFECT_LEFT,
	EFFECT_DEATH
};

enum FIRE_RELEASE_MODE
{
	RELEASE_NORMAL,
	RELEASE_TRACE
};

enum FIRE_EFFECTS
{
	HIT_EFFECT,
};

enum Menus
{
	NO_MENU = -1,
	MAIN_MENU,
	MULTIPLAYER_MENU,
	SETTING_MENU,
	PLAYING_MENU,
	QUIT_MENU
};

enum ClientState
{
	CLIENT_UNCONNECTED,
	CLIENT_CONNECTED,
	CLIENT_DISCONNECTED,
	CLIENT_WAITING,
	CLIENT_MAP_NOT_FOUND,
	CLIENT_MAP_NOT_LOAD,
	CLIENT_RUNNING
};

enum ServerState
{
	SERVER_NOT_RUNNING,
	SERVER_WAITING,
	SERVER_HANDLING
};

extern GameInfo g_gameInfo;

#endif