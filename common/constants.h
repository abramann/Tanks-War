// Note parts of this code are licensed under CC BY 3.0

// Programming 2D Games Copyright (c) 2011 by Charles Kelly 
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "gameerror.h"
#include "imgui\imgui_impl_win32.h"
#include "net.h"
#include "crc32.h"
#include<iostream>
#include<vector>
#include <stdarg.h>
#include<Windows.h>
#include <d3dx9.h>



extern uint64_t g_frameCounter;

#define COLOR_ARGB D3DCOLOR_ARGB
#define COLOR_XRGB D3DCOLOR_XRGB
#define MILLSEC
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
//#include <D3Dcompiler.h>

typedef ID3D11ShaderResourceView* LPTextureD3D;
typedef ID3D11Buffer* LPVertexBuffer;

#endif

#define IN_RANGE(n, a, b) (bool)( (n > a && n < b) || (n > b && n < a))
#define IN_RANGE_OR_EQUAL(n, a, b) (bool)( (n >= a && n <= b) || (n >= b && n <= a))

constexpr double PI = 3.1415926535897932384626433832795;
constexpr auto  MIN_RESOLUTION_WIDTH = 800;
constexpr auto  MIN_RESOLUTION_HEIGHT = 600;
constexpr auto  VERTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;
constexpr auto CLIENT_INFO_PATH = "Assets\\ini\\client-info.txt";
constexpr auto CLIENT_PRESENT_TIME = 5000;
constexpr auto COLOR_ALPHA = COLOR_ARGB(0, 0, 0, 255);
constexpr auto COLOR_BLACK = COLOR_XRGB(0, 0, 0);
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);
constexpr auto FRAME_RATE = 60;
constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
constexpr auto IMAGE_INFO_PATH = "Assets\\ini\\image-info.txt";
constexpr auto INVALID_DATA = 0xFFFF;
constexpr auto MAP_DIR = "Assets\\maps\\";
constexpr auto MAX_NAME_LEN = 20;
constexpr auto MAX_PACKET_SIZE = 256;
constexpr auto MAX_PLAYERS = 12;
constexpr auto PROJECT_FAR_PLANE = 1000.0f;
constexpr auto PROJECT_FOV = PI / 2;
constexpr auto PROJECT_NEAR_PLANE = 1.0f;
constexpr auto SERVER_INFO_PATH = "Assets\\ini\\server-info.txt";
constexpr auto SERVER_RECIEVE_PRESENT_TIME = 10000;
constexpr auto TANK_INFO_PATH = "Assets\\ini\\tank-info.txt";
constexpr auto TANK_ROTATE_AMOUNT = PI / 64;
constexpr auto TEXTURE_DIR = "Assets\\texture\\";
constexpr auto TEXTURE_TANK_DESTROY_ROWS_COLUMNS = 8;
constexpr auto UPDATE_DELAY_FPS = 0.5f;
constexpr auto UPDATE_DELAY_TANK_DESTROY = 100;
constexpr BYTE BYTE_INVALID_DATA = 0xFF;
constexpr DWORD INVALID_ADDRESS = 0xFFFFFFF;
constexpr float UNDEFINED_POSITION = 0xFFFF;
constexpr int MAX_PORT = 4861;
constexpr int MIN_PORT = 10;
constexpr uint8_t INVALID_ID = -1;
constexpr uint8_t MAX_FILE_NAME = 255;
constexpr unsigned short UNSPECIFIED_PORT = 0xCCCC;
constexpr auto TEXTURE_BULLET_ROWS_COLUMNS = 3;
constexpr auto 	TEXTURE_TILEDS = 3;
constexpr auto UPDATE_DELAY_BULLET = 80;
const auto UNSPECIFIED_POS = V3(0.1f, 0.1f, 0.1f);
const auto SPACE_VERTICES = 4;

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

constexpr const char* strSERVER_STATE[] = { "Not started" , "Waiting for players...", "Preparibg game..." , "Handlubg" };
constexpr Vec4 colSERVER_STATE[] = { Vec4(0.7f,0.7f,0.7f,0.5f), Vec4(0.87f,0.77f,0,1), Vec4(0,1,0,1) };

template <typename T>
void safeRelease(T ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = 0;
	}
}

template <typename T>
inline void safeDelete(T ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = 0;
	}
}

template <typename T>
inline void safeDeleteArray(T ptr)
{
	delete[] ptr;
	ptr = NULL;
}

inline uint32_t _rand(uint32_t max)
{
	return ::GetTickCount() % max;
}

typedef int8 PacketType;

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

typedef int8 PlayerAct;

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


struct GameInfo
{
	int8 windowed;
	int16 width, height;
};

struct ImageInfo
{
	uint16_t width, height;
	uint8_t columns, rows;
	bool animate;
	float animateSpeed, scalling;
};


struct BitmapData
{
	uint16_t width, height;
};

struct MapData
{
	int16 width, height;
	int8 bitmaps;
	std::vector<int8> preventedBM;
};

template<typename T>
inline T getMax(std::vector<T> val)
{
	T maxValue = val[0];
	for (auto v : val)
		maxValue = max(maxValue, v);

	return maxValue;
}

template<typename T>
inline T getMin(std::vector<T> val)
{
	T minValue = val[0];
	for (auto v : val)
		minValue = min(minValue, v);

	return minValue;
}

template <typename T>
inline void add4(const T amount, T& v1, T& v2, T& v3, T& v4)
{
	v1 += amount,
		v2 += amount,
		v3 += amount,
		v4 += amount;
}

struct Space
{
	V3 v1, v2, v3, v4;
	//Space(V3 _v1, V3 _v2, V3 _v3, V3 _v4) : v1(_v1), v2(_v2), v3(_v3), v4(_v4) {};
	//bool isEqual(Space s) const { return (s.v1 == v1 &&s.v2 == v2 &&s.v3 == v3 &&s.v4 == v4) ? true : false; }
	bool isSame(Space s) const { return ( ( (s.v1 == v1 && s.v2 == v2) || (s.v1 == v2 && s.v2 == v1) ) && ( ( s.v3 == v3 && s.v4 == v4) || (s.v3 == v4 && s.v4 == v3) ) ) ? true : false; }
	bool isValid() const { return (v1.x == UNDEFINED_POSITION) ? false : true; }
	float getMaxX() const { return getMax<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMinX() const { return getMin<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMaxY() const { return getMax<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getMinY() const { return getMin<float>({ v1.y, v2.y, v3.y, v4.y }); }
	void addX(float val) { add4(val, v1.x, v2.x, v3.x, v4.x); }
	void addY(float val) { add4(val, v1.y, v2.y, v3.y, v4.y); }
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

struct SpsPlayerAct
{
	PacketType packetType = PACKET_PLAYER_UPDATE;
	PlayerAct act;
	PlayerID id;
};

#pragma warning(disable : 4200)

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