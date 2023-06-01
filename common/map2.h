// map.h
// Author: abramann

#ifndef _MAP_H
#define _MAP_H
#include "constants.h"
#include <vector>

class Game;
class Graphics;
class TextureManger;
class Object2;
class Image2;
class Texture;
class Bullet;

class Map2
{
public:

	Map2();
	~Map2();
	void initialize(const Game* game);
	bool load(const char* map);
	void draw() const;
	void clear();
	V2 getMapSize() const { return V2(m_width*m_tiledSize.x, m_height*m_tiledSize.y); }
	float passX(const Object2* object, float x);
	float passY(const Object2* object, float y);
	bool isCollided(const Image2* image);
	bool isCollided(const Object2* object);
	bool isCollided(const Space space, const Object2* object = 0);
	bool isOutOfRange(const Space space) const;
	Object2* getObject(const Space space);
	//static Space getImageSpace(const Image2* image, float x0 = 0, float y0 = 0);
	bool isMapExist(const char* name, Crc32 crc32) { return true; }
	const char* getMap() const { return m_loadedMap; }
	Crc32 getCrc32() const;
	const char* loadRandom();
	Space getRandomEmptySpace() const;
	void addObject(Object2* object);

private:

	bool read();
	bool areSpacesCollided(const Space space1, const Space space2) const;
	void clearUnnecessaryNospace();
	Space getRightSpace(Space s) const;
	Space getLeftSpace(Space s) const;
	Space getUpSpace(Space s) const;
	Space getDownSpace(Space s) const;
	bool isNospaceUseless(Space s) const;
	bool isFreeSpace(Space s) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[TEXTURE_TILEDS];
	Graphics* m_pGraphics;
	std::vector< std::vector<char>> m_map;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	LPVertexBuffer m_lpVertexBuffer;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	std::vector<int8> m_preventedBitmap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object2*> m_pObject;
	V2 m_tiledSize;
	char m_loadedMap[MAX_NAME_LEN];
};


#endif