#ifndef _MAP_H
#define _MAP_H

#include "image.h"
#include <vector>

struct Space
{
	int32_t x1, y1;
	int32_t x2, y2;
};

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
	virtual const float passX(float x, float x0, float y);
	virtual const float passY(float x, float y, float y0);

protected:

	Graphics* m_pGraphics;
	TextureManger* m_pTextureManger;
	std::vector<Space> m_noSpace;
	MapData m_mapData;
	BitmapData m_bitmapData;
	char **m_ppMap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;

};


#endif
