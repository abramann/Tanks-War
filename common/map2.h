#pragma once
#include "texturemanger.h"
#include "graphics.h"
#include <vector>

class Game;
class Object2;
const auto MAP_BITMAPS = 2;
const auto TEXTURE_BITMAP_0 = 10;

class Map2
{
public:
	Map2();
	~Map2();
	void initialize(const Game* game);
	bool load(const char* map);
	void draw() const;
	void clear();
	float passX(const Object2* object, float x) const;
	float passY(const Object2* object, float y) const;

private:

	bool read();
	bool areSpacesCollided(const Space space1, const Space space2) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[MAP_BITMAPS];
	Graphics* m_pGraphics;
	char m_loadedMap[MAX_NAME_LEN];
	std::vector< std::vector<char>> m_map;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	LPVertexBuffer m_lpVertexBuffer;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	std::vector<int8> m_preventedBitmap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;

};
