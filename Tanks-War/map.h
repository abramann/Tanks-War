#ifndef _MAP_H
#define _MAP_H

#include "image.h"

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

protected:

	Image** bitmap;
	Graphics* graphics;
	TextureManger* textureManger;
	
	MapData mapData;
	char **map;

};


#endif
