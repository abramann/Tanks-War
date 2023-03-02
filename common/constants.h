// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "gameerror.h"
#include<iostream>
#include<vector>
#include<Windows.h>
#include <d3dx9.h>

extern uint64_t g_frameCounter;

#define COLOR_ARGB D3DCOLOR_ARGB
#define COLOR_XRGB D3DCOLOR_XRGB
#define TANK_KEYS			5
#define TANK_KEYS			5
#define TANK_KEYATTACK		4
#define TANK_EFFECTS		7
#define TANK_EFFECTSHOT		5
#define TANK_EFFECTHIT		6

constexpr auto COLOR_BLACK = COLOR_XRGB(0, 0, 0);
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);
constexpr auto COLOR_ALPHA = COLOR_ARGB(0, 0, 0, 255);
constexpr double PI = 3.14159265;
constexpr float MIN_FRAME_TIME = 10.0f;
constexpr float MAX_FRAME_TIME = 80.0f;
constexpr auto MAX_PLAYER_NAME = 20;
constexpr auto MAX_PACKET_SIZE = 255;
constexpr DWORD INVALID_ADDRESS = 0xFFFFFFF;
constexpr uint8_t INVALID_ID = 99;
constexpr auto NET_RESPONSE = 1;
constexpr auto NET_NORESPONSE = -1;
constexpr auto MAX_PLAYERS = 12;
constexpr unsigned short UNSPECIFIED_PORT = 0xCCCC;
constexpr uint32_t UNDEFINED_POSITION = 0xFFFF;
constexpr uint8_t MAX_FILE_NAME = 255;
constexpr auto INVALID_DATA = 0xFFFF;
constexpr auto  VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
constexpr auto OBJECT_INFO_PATH = "Assets\\ini\\object-info.txt";
constexpr auto TANK_INFO_PATH = "Assets\\ini\\tank-info.txt";
constexpr auto IMAGE_INFO_PATH = "Assets\\ini\\image-info.txt";
constexpr auto FIRE_DATA_PATH = "Assets\\ini\\fire-info.txt";
constexpr auto TEXTURE_DIR = "Assets\\texture\\";

typedef D3DCOLOR Color;
typedef D3DXVECTOR2 V2;
typedef BYTE Key;
typedef uint8_t PlayerID;
typedef unsigned short Port;
typedef uint8_t Protocol;

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

inline std::string getFileNameFromPath(std::string path)
{
	std::string sName = path.substr(path.find_last_of('\\') + 1,
		path.find_last_of('.') - path.find_last_of('\\') - 1);
	return sName;
}

inline uint32_t _rand(uint32_t max)
{
	return rand() % max;
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
	float scalling;
	RECT rect;
	Color filterColor;
	V2 center;
};

struct GameInfo
{
	bool fullScreen;
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

struct PlayerInfo
{
	char name[MAX_PLAYER_NAME];
	uint8_t id;
};

struct PlayerState
{
	int x, y;
	float angle, health;
	char id;
	bool  attack;
};

struct ServerToPlayer
{
	PlayerState p0, p1;
};

struct PlayerToServer
{
	bool forward, back, right, left, attack;
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

extern GameInfo g_gameInfo;

#endif