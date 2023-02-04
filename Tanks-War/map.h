#ifndef _MAP_H
#define _MAP_H

#include "image.h"
#include <vector>

struct Space {
	int x1, y1;
	int x2, y2;
};

struct BitmapData {
	int bitmaps;
	int width, height;
};

struct MapData
{
	std::string path;
	int width, height;
	BitmapData bitmapData;
};

class Map
{
public:

	Map();
	~Map();
	virtual bool initialize(std::string mapPath, int bitmaps, TextureManger* _textureManger, Graphics* _graphics);
	virtual bool read(std::string map);
	virtual void draw();
	virtual float passX(float x, float x0, float y);
	virtual float passY(float x, float y, float y0);

protected:

	Image** bitmap;
	Graphics* graphics;
	TextureManger* textureManger;
	std::vector<Space> noSpace;

	MapData mapData;
	char **map;

};


#endif
