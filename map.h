#ifndef _MAP_H
#define _MAP_H

#include "image.h"
#include <vector>

struct Space {
	int x1, y1;
	int x2, y2;
};

struct BitmapData {
	UINT width, height;
};

struct MapData
{
	char* name;
	UINT width, height;
};

const int UNDEFINED_POSITION = 0xFFFF;

class Map
{
public:

	Map();
	~Map();
	virtual bool initialize(const char* path, TextureManger* _textureManger, Graphics* _graphics);
	virtual bool read(const char* map);
	virtual void draw();
	virtual float passX(float x, float x0, float y);
	virtual float passY(float x, float y, float y0);

protected:

	Graphics* graphics;
	TextureManger* textureManger;
	std::vector<Space> noSpace;

	MapData mapData;
	BitmapData bitmapData;
	char **map;
	UINT m_bitmaps, m_totalBitmaps;
	std::vector<int> m_startVertex, m_lenVertex;

};


#endif
