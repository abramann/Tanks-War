#pragma once
#include "constants.h"
#include <vector>

class Game;
class Graphics;
class TextureManger;
class Object2;
class Image2;
class Texture;

class Map2
{
public:
	Map2();
	~Map2();
	void initialize(const Game* game);
	bool load(const char* map);
	void draw() const;
	void clear();
	float passX(const Image2* object, float x) const;
	float passY(const Image2* object, float y) const;
	bool isCollided(const Image2* image) const;
	Object2* getObject(V3 position) const;
	static Space getImageSpace(const Image2* image, float x0 = 0, float y0 = 0);

	bool isMapExist(const char* name, Crc32 crc32) { return true; }
private:

	bool read();
	bool areSpacesCollided(const Space space1, const Space space2) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[TEXTURE_TILEDS];
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
	std::vector<Object2*> m_object;
};
