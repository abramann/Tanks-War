// constant.h
// Author: abramann
// Note this file is influenced by constant.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0
// Note parts of this code are licensed under CC BY 3.0

#pragma once

#define WIN32_LEAN_AND_MEAN
#include "gameerror.h"
#include "net.h"
#include "crc32.h"
#include "v3.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3D10.h>
#include <d3dx9.h>
D3DXMATRIX;
extern uint64_t g_frameCounter;

struct Color
{
	float x, y, z;
	Color() {};
	Color(float x, float y, float z) {};
};

#define COLOR_ARGB(a,b,c) Color(a,b,c)
#define COLOR_XRGB Color(a,b,c)

typedef uint8_t PlayerID;
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
typedef HRESULT Result;
typedef int8 PacketType;
typedef int8 PlayerAct;
typedef DirectX::XMMATRIX Matrix;
typedef DirectX::XMFLOAT2 V2;

typedef ID3D11ShaderResourceView* LPTextureD3D;
typedef ID3D11Buffer* LPVertexBuffer;

#define IN_RANGE(n, a, b) (bool)( (n > a && n < b) || (n > b && n < a))
#define IN_RANGE_OR_EQUAL(n, a, b) (bool)( (n >= a && n <= b) || (n >= b && n <= a))

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

struct Vertex
{
	Vertex() : x(0), y(0), z(0), u(0), v(0) {};
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
	int8 vsync;
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
	bool isSame(Space s) const { return ((s.v1 == v1 && s.v2 == v2) || (s.v1 == v2 && s.v2 == v1)) && ((s.v3 == v3 && s.v4 == v4) || (s.v3 == v4 && s.v4 == v3)) ? true : false; }
	bool isValid() const { return (v1.x == mapNS::UNDEFINED_POSITION) ? false : true; }
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
	char name[gameNS::MAX_NAME_LEN], serverIP[netNS::IP_SIZE];
	Port serverPort;
};

struct PlayerIniData
{
	char name[gameNS::MAX_NAME_LEN];
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
	char name[gameNS::MAX_NAME_LEN];
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
	char  map[gameNS::MAX_NAME_LEN];
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

extern GameInfo g_gameInfo;