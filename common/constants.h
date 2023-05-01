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
#define MILLSEC
#define MICROSEC
typedef D3DCOLOR Color;
typedef D3DXVECTOR2 V2;
typedef D3DXVECTOR3 V3;
typedef uint8_t PlayerID;
typedef ImGuiKey Key;
typedef unsigned short Port;
typedef uint8_t Protocol;
typedef uint32_t Crc32;
typedef ImVec2 Vec2;
typedef ImVec4 Vec4;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef D3DXMATRIX Matrix;
typedef HRESULT Result;

#ifdef _BUILD_WITH_D3D9

typedef LPDIRECT3DTEXTURE9 LPTextureD3D;
typedef LPDIRECT3DVERTEXBUFFER9 LPVertexBuffer;

#else ifdef _BUILD_WITH_D3D11

#include <D3D11.h>
#include <D3DX11.h>
#include <D3D10.h>
#include <D3Dcompiler.h>

typedef ID3D11ShaderResourceView* LPTextureD3D;
typedef ID3D11Buffer* LPVertexBuffer;

#endif

#define IN_RANGE(n, startRange, endRange) (bool)(n >= startRange && n <= endRange)

constexpr double PI = 3.14159265;
constexpr auto  MIN_RESOLUTION_WIDTH = 800;
constexpr auto  MIN_RESOLUTION_HEIGHT = 600;
constexpr auto  VERTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;
constexpr auto CLIENT_INFO_PATH = "Assets\\ini\\client-info.txt";
constexpr auto CLIENT_PRESENT_TIME = 5000;
constexpr auto COLOR_ALPHA = COLOR_ARGB(0, 0, 0, 255);
constexpr auto COLOR_BLACK = COLOR_XRGB(0, 0, 0);
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);
constexpr auto FIRE_DATA_PATH = "Assets\\ini\\fire-info.txt";
constexpr auto FRAME_RATE = 240;
constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
constexpr auto IMAGE_INFO_PATH = "Assets\\ini\\image-info.txt";
constexpr auto INVALID_DATA = 0xFFFF;
constexpr auto MAP_DIR = "Assets\\maps\\";
constexpr auto MAX_FRAME_TIME = 80;
constexpr auto MAX_NAME_LEN = 20;
constexpr auto MAX_PACKET_SIZE = 128;
constexpr auto MAX_PLAYERS = 12;
constexpr auto MIN_FRAME_RATE = 10;
constexpr auto MIN_FRAME_TIME = 20;
constexpr auto OBJECT_INFO_PATH = "Assets\\ini\\object-info.txt";
constexpr auto PROJECT_FAR_PLANE = 1000.0f;
constexpr auto PROJECT_FOV = PI / 2;
constexpr auto PROJECT_NEAR_PLANE = 1.0f;
constexpr auto SERVER_INFO_PATH = "Assets\\ini\\server-info.txt";
constexpr auto SERVER_RECIEVE_PRESENT_TIME = 10000;
constexpr auto TANK_INFO_PATH = "Assets\\ini\\tank-info.txt";
constexpr auto TANK_ROTATE_AMOUNT = PI / 8;
constexpr auto TEXTURE_DIR = "Assets\\texture\\";
constexpr auto TEXTURE_TANK_DESTROY_ROWS_COLUMNS = 4;
constexpr auto UPDATE_DELAY_FPS = 0.5f;
constexpr auto UPDATE_DELAY_TANK_DESTROY = 100;
constexpr BYTE BYTE_INVALID_DATA = 0xFF;
constexpr DWORD INVALID_ADDRESS = 0xFFFFFFF;
constexpr DWORD INVALID_PTR = 0xcdcdcdcd;
constexpr float UNDEFINED_POSITION = 0xFFFF;
constexpr int MAX_PORT = 4861;
constexpr int MIN_PORT = 10;
constexpr uint8_t INVALID_ID = -1;
constexpr uint8_t MAX_FILE_NAME = 255;
constexpr unsigned short UNSPECIFIED_PORT = 0xCCCC;
constexpr auto LOGO_WIDTH = 800;
constexpr auto LOGO_HEIGHT = 600;
constexpr auto TEXTURE_PLAYER_TANK = 2;
constexpr auto TEXTURE_ENEMY_TANK = 8;

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

#define UNSPECIFIED_POS V3(0.1f,0.1f,0.1f)
constexpr const char* strSERVER_STATE[] = { "Not started" , "Waiting for players...", "Preparibg game..." , "Handlubg" };
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

	memset(name, 0, sName.length() + 1);
	strcpy(name, sName.c_str());
}

inline uint32_t _rand(uint32_t max)
{
	return ::GetTickCount() % max;
}

template <typename T>
inline T* getIdItem(const PlayerID id, std::vector<T>* item)
{
	T* rItem = 0;;
	for (auto& it : *item)
	{
		if (it.id == id)
		{
			rItem = &it;
			break;
		}
	}

	return rItem;
}

enum PacketType
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

enum PlayerAct
{
	PLAYER_ACT_NONE,
	PLAYER_ACT_FORWRAD,
	PLAYER_ACT_BACK,
	PLAYER_ACT_RIGHT,
	PLAYER_ACT_LEFT,
	PLAYER_ACT_ATTACK
};

struct Vertex
{
	Vertex() {};
	Vertex(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {};
	float x, y, z;
	float u, v;
};

struct TextureVertices
{
	Vertex v1, v2, v3;
	Vertex v4, v5, v6;
};

struct SpriteData
{
	LPTextureD3D lpTexture;
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
	int8 windowed;
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
	int16 width, height;
	int8 bitmaps;
	std::vector<int8> preventedBM;
};

struct Space
{
	V3 v1, v2, v3, v4;
};

struct ServerInfo
{
	Port port;
	uint8_t players;
};

struct ClientInfo
{
	char name[MAX_NAME_LEN], serverIP[netNS::IP_SIZE];
	Port serverPort;
};

struct PlayerIniData
{
	char name[MAX_NAME_LEN];
	PlayerID id;
};

struct PlayerUpdate
{
	V3 position;
	V3 rotate;
	float health;
	PlayerID id;
};

struct CpsPlayerAct
{
	PacketType packetType = PACKET_PLAYER_UPDATE;
	PlayerAct act;
	PlayerID id;
};

struct SpsPlayerUpdate
{
	PacketType packetType = PACKET_PLAYER_UPDATE;
	PlayerUpdate playerUpdate[];
};

struct CpsIni
{
	PacketType  packetType = PACKET_INI;
	char name[MAX_NAME_LEN];
};

struct CpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
	PlayerID id;
};

struct CpsPresent
{
	PacketType packet = PACKET_PRESENT_CLIENT;
	PlayerID id;
};

struct CpsSeasson
{
	PacketType packetType;
	PlayerID id;
};

struct SpsIni
{
	PacketType packetType = PACKET_INI;
	bool accept;
	PlayerID id;
	uint8_t gamePlayers;
	char  map[MAX_NAME_LEN];
	Crc32 checksum;
	PlayerIniData playerIniData;
};

struct SpsPlayersExist
{
	PacketType packetType = PACKET_PLAYERS_EXIST;
	uint8_t players;
};

struct SpsPlayersInitData
{
	PacketType packetType = PACKET_PLAYERS_INI_DATA;
	PlayerIniData playerIniData[];
};

struct SpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
	PlayerID id;
};

/*struct SpsNewPlayer
{
	PacketType packet = PACKET_NEW_PLAYER;
	char name[MAX_NAME_LEN];
	PlayerID id;
};*/

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

enum FireReleaseMode
{
	RELEASE_NORMAL,
	RELEASE_TRACE
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
extern GameInfo g_gameInfo;

#endif