// types.h
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
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <d3d11.h>

#pragma warning(disable : 4200)

typedef int8_t PlayerID;
typedef unsigned short Port;
typedef uint8_t Protocol;
typedef uint32_t Crc32;
typedef ImVec2 Vec2;
typedef ImVec4 Vec4;
typedef HRESULT Result;
typedef int8 PlayerAct;
typedef DirectX::XMMATRIX Matrix;
typedef DirectX::XMFLOAT2 V2;

typedef ID3D11ShaderResourceView* LPTextureD3D;
typedef ID3D11Buffer DxBuffer;
typedef ID3D11ShaderResourceView DxShaderResourceView;
typedef ID3D11UnorderedAccessView DxUnorderedAccessView;
typedef ID3D11ComputeShader DxComputeShader;

#define IN_RANGE(n, a, b) (bool) ( max(a, b) > n && n > min(a, b) )
#define IN_RANGE_OR_EQUAL(n, a, b) (bool) ( max(a, b) >= n && n >= min(a, b) )

constexpr const char* strSERVER_STATE[] = { "Not started" , "Waiting for players...", "Preparibg game..." , "Handlubg" };
constexpr Vec4 colSERVER_STATE[] = { Vec4(0.7f,0.7f,0.7f,0.5f), Vec4(0.87f,0.77f,0,1), Vec4(0,1,0,1) };

struct Vertex
{
	Vertex() : x(0), y(0), z(0), u(0), v(0) {};
	Vertex(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {};
	float x, y, z;
	float u, v;
};

struct GameSettings
{
	uint32 width, height;
	uint32 aiLevel, aiCount;
	bool windowed, vsync, audio, computeShader, debugMode;
};

struct BitmapAttribute
{
	bool obstructed; // barrier
	float velocityFactor, damageFactor;
};

struct MapInfo
{
	int16 width, height;
	int8 bitmaps;
	std::vector<BitmapAttribute> bitmapAttribute;
};

//	inlined functions used by Space struct
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

	Space() { clear(); }
	Space(const Space& space) { *this = space; }
	Space(const V3& _v1, const V3& _v2, const V3& _v3, const V3& _v4) : v1(_v1), v2(_v2), v3(_v3), v4(_v4) {}
	void clear() { v1.clear(), v2.clear(), v3.clear(), v4.clear(); }
	bool isSame(const Space& s) const { return ((s.v1 == v1 && s.v2 == v2) || (s.v1 == v2 && s.v2 == v1)) && ((s.v3 == v3 && s.v4 == v4) || (s.v3 == v4 && s.v4 == v3)) ? true : false; }
	bool isValid() const { return (v1.x == mapNS::UNDEFINED_POSITION) ? false : true; }
	bool isIncluded(const V3& v3) { return (IN_RANGE_OR_EQUAL(v3.x, getMinX(), getMaxX()) && IN_RANGE_OR_EQUAL(v3.y, getMinY(), getMaxY())); }
	void setInvalid() { v1.x = mapNS::UNDEFINED_POSITION; }
	//bool operator<(const Space& s) const { return (v1 < s.v1&& v2 < s.v2 && v3 < s.v3 && v4 < s.v4); } // for using with std::set
	float getMaxX() const { return getMax<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMinX() const { return getMin<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMaxY() const { return getMax<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getMinY() const { return getMin<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getWidth() const { return getMaxX() - getMinX(); }
	float getHeight() const { return getMaxY() - getMinY(); }
	float getSize() const { return getWidth()*getHeight(); }
	V3 getCenter() const { return V3(getMinX() + getWidth() / 2, getMinY() + getHeight() / 2, 0); }
	float getDistanceFromCenter(const V3& v3) const { return getCenter().distance(v3); }
	float getFCostForCenter(std::vector<V3> v3) const
	{
		float fcost = 0;
		for (const auto& v : v3)
			fcost += getDistanceFromCenter(v);

		return fcost;
	}

	void addX(float val) { add4(val, v1.x, v2.x, v3.x, v4.x); }
	void addY(float val) { add4(val, v1.y, v2.y, v3.y, v4.y); }
	Space getRightSpace() const
	{
		return Space(
			V3(getMaxX(), getMinY(), 0),
			V3(getMaxX() + getWidth(), getMinY(), 0),
			V3(getMaxX() + getWidth(), getMaxY(), 0),
			V3(getMaxX(), getMaxY(), 0));
	}

	Space getLeftSpace() const
	{
		return Space(
			V3(getMinX() - getWidth(), getMinY(), 0),
			V3(getMinX(), getMinY(), 0),
			V3(getMinX(), getMaxY(), 0),
			V3(getMinX() - getWidth(), getMaxY(), 0));
	}

	Space getUpperSpace() const
	{
		return Space(
			V3(getMinX(), getMaxY(), 0),
			V3(getMaxX(), getMaxY(), 0),
			V3(getMaxX(), getMaxY() + getHeight(), 0),
			V3(getMinX(), getMaxY() + getHeight(), 0));
	}

	Space getDownSpace() const
	{
		return Space(
			V3(getMinX(), getMinY() - getHeight(), 0),
			V3(getMaxX(), getMinY() - getHeight(), 0),
			V3(getMaxX(), getMinY(), 0),
			V3(getMinX(), getMinY(), 0));
	}
};

struct Vector3D
{
	V3 begin, end;
	float size;

	Vector3D() : size(1) {}
	Vector3D(const V3& _begin, const V3& _end) : begin(_begin), end(_end) {}
	Vector3D(const V3& _begin, const V3& _end, float _size) : begin(_begin), end(_end), size(_size) { ; }
	float getMagnitude() const { return begin.distance(end); }
};

struct TextureVertices
{
	Vertex v1, v2, v3, v4;
	bool isValid() const { return (v1.x != mapNS::UNDEFINED_POSITION); }
	float getMaxX() const { return getMax<float>({ v1.x, v2.x, v3.x, v4.x }); }
	float getMinX() const { return getMin<float>({ v1.x, v2.x, v3.x, v4.x }); }
	float getMaxY() const { return getMax<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getMinY() const { return getMin<float>({ v1.y, v2.y, v3.y, v4.y }); }
	Space getSpace() const
	{
		Space space;
		space.v1 = V3(getMinX(), getMinY());
		space.v2 = V3(getMaxX(), getMinY());
		space.v3 = V3(getMaxX(), getMaxY());
		space.v4 = V3(getMinX(), getMaxY());
		return space;
	}
};

inline bool areSpacesCollided(const Space& spaceA, const Space& spaceB)
{
	float size1 = spaceA.getSize();
	float size2 = spaceB.getSize();
	Space s1, s2;
	if (size1 > size2)
		s1 = spaceA, s2 = spaceB;
	else
		s1 = spaceB, s2 = spaceA;

	float maxX1 = s1.getMaxX(),
		minX1 = s1.getMinX(),
		maxY1 = s1.getMaxY(),
		minY1 = s1.getMinY();
	if (IN_RANGE(s2.v1.x, minX1, maxX1) ||
		IN_RANGE(s2.v2.x, minX1, maxX1) ||
		IN_RANGE(s2.v3.x, minX1, maxX1) ||
		IN_RANGE(s2.v4.x, minX1, maxX1)
		)
		if (IN_RANGE(s2.v1.y, minY1, maxY1) ||
			IN_RANGE(s2.v2.y, minY1, maxY1) ||
			IN_RANGE(s2.v3.y, minY1, maxY1) ||
			IN_RANGE(s2.v4.y, minY1, maxY1)
			)
			return true;

	V3 s2cen = s2.getCenter();
	if (IN_RANGE(s2cen.x, minX1, maxX1))
		if (IN_RANGE(s2cen.y, minY1, maxY1))
			return true;

	return false;
}

struct ServerInfo
{
	Port port;
	int32 maxClients;
};

struct ClientInfo
{
	char name[gameNS::MAX_NAME_LEN], serverIP[netNS::IP_SIZE];
	Port serverPort;
};

struct ClientGameAttribute
{
	PlayerID id;
	float health, inflictedDamage, velocity, bulletSpeed, bulletDamage;
};

struct SpsClientGameAttribute
{
	PacketType packetType = PACKET_CLIENT_GAME_ATTRIBUTE;
	int8 clients;
	ClientGameAttribute clientGameAttribute[];
};

struct CpsPlayerAct
{
	PacketType packetType = PACKET_CLIENT_UPDATE;
	PlayerAct act;
	PlayerID id;
};

struct CpsJoin
{
	PacketType packetType = PACKET_CLIENT_JOIN;
	char name[gameNS::MAX_NAME_LEN];
};

struct SpsJoin
{
	PacketType packetType = PACKET_CLIENT_JOIN;
	PlayerID id;
	char name[gameNS::MAX_NAME_LEN];
	char map[gameNS::MAX_NAME_LEN];
	uint32 clients;
};

struct SpsPlayerAct
{
	PacketType packetType = PACKET_CLIENT_UPDATE;
	PlayerAct act;
	PlayerID id;
};

struct CpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
	PlayerID id;
};

struct CpsHeartbeat
{
	PacketType packet = PACKET_CLIENT_HEARTBEAT;
	PlayerID id;
};

struct SpsDisconnect
{
	PacketType packetType = PACKET_DISCONNECT;
	PlayerID id;
};

struct ClientGameStatus
{
	PlayerID id;
	V3 position, rotate;
};

struct SpsClientGameStatus
{
	PacketType packetType = PACKET_CLIENT_GAME_STATE;
	ClientGameStatus clientGameStatus;
};

struct SpsClientInitialData
{
	PacketType packetType = PACKET_CLIENT_INITIAL_DATA;
	char name[gameNS::MAX_NAME_LEN];
	ClientGameStatus clientGameStatus;
};

struct SpsServerShutdown
{
	PacketType packetType = PACKET_SERVER_SHUTDOWN;
};

extern uint64_t g_frameCounter;
extern GameSettings* g_pGameSettings;
