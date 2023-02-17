#ifndef _MAP_H
#define _MAP_H

#include "always.h"
#include "object.h"

struct BitmapData
{
	uint16_t width, height;
};

struct MapData
{
	std::string name;
	uint16_t width, height;
	uint8_t bitmaps;
	std::vector<uint8_t> preventedBM;
};

constexpr uint32_t UNDEFINED_POSITION = 0xFFFF;

class Map
{

public:

	Map();
	~Map();
	virtual bool initialize(const char* path, TextureManger* textureManger, Graphics* graphics);
	virtual bool read(const char* map);
	virtual void draw();
	void Add_Object(Object* object) { m_pObjects.push_back(object); }
	bool spacesCollided(const Space& space1, const Space& space2) const;
	virtual const float passX(float x, float x0, float y);
	virtual const float passY(float x, float y, float y0);
	Object* collided(const Image& object);
	bool outOfMapRange(const Image& object) const;
//	bool outOfPlayerRange() const;

protected:

	Graphics* m_pGraphics;
	TextureManger* m_pTextureManger;
	std::vector<Space> m_noSpace;
	MapData m_mapData;
	BitmapData m_bitmapData;
	char **m_ppMap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObjects;

};




#endif

