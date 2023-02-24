#include<iostream>
#include<vector>
#include<Windows.h>
#include <d3dx9.h>

#define ZERO_V2(v2) (v2 == V2(0,0))
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
typedef BYTE Key;

constexpr uint32_t UNDEFINED_POSITION = 0xFFFF;
constexpr uint8_t MAX_FILE_NAME = 255;
constexpr auto INVALID_DATA = 0xFFFF;
constexpr auto  VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
constexpr auto COLOR_BLACK = COLOR_XRGB(0, 0, 0);
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);
constexpr auto COLOR_ALPHA = COLOR_ARGB(0, 0, 0, 255);
constexpr auto GAME_INFO_PATH = "Assets\\ini\\game-info.txt";
constexpr auto OBJECT_INFO_PATH = "Assets\\ini\\object-info.txt";
constexpr auto TANK_INFO_PATH = "Assets\\ini\\tank-info.txt";
constexpr auto IMAGE_INFO_PATH = "Assets\\ini\\image-info.txt";
constexpr auto FIRE_DATA_PATH = "Assets\\ini\\fire-info.txt";
constexpr auto TEXTURE_DIR = "Assets\\texture\\";

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
//	Color transparency;
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




