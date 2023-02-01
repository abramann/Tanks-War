#include "Map.h"
#include "texture.h"
#include <fstream>

Map::Map()
{
}


Map::~Map()
{
	for (int i = 0; i < mapData.height; i++)
	{
		SAFE_DELETE_ARRAY(map[i]);
		SAFE_DELETE_ARRAY(bitmap[i]);
	}
	
	SAFE_DELETE_ARRAY(map);
	SAFE_DELETE_ARRAY(bitmap);
}

bool Map::initialize(std::string mapPath, int bitmaps, TextureManger* _textureManger, Graphics* _graphics)
{
	graphics = _graphics;
	textureManger = _textureManger;
	if (!read(mapPath))
		return false;

	// only row bitmaps supported
	mapData.bitmapData.bitmaps = bitmaps;

	mapData.bitmapData.width = textureManger->getWidth() / bitmaps;
	mapData.bitmapData.height = textureManger->getHeight();
		
	// temp code
	mapData.bitmapData.width = 50;
	mapData.bitmapData.height = 50;
	///
	bitmap = new Image*[mapData.height];
	for (int i = 0; i < mapData.height; i++)
		bitmap[i] = new Image[mapData.width];
	
	for (int h = 0; h < mapData.height; h++)
	{
		for (int w = 0; w < mapData.width; w++)
		{
			bitmap[h][w].initialize(mapData.bitmapData.width, mapData.bitmapData.height , 1, 1, false, 0, textureManger, graphics);
			RECT rect = { 0 };
			rect.top = 0; rect.bottom = mapData.bitmapData.height;
			rect.left = map[h][w] * mapData.bitmapData.width; rect.right = (map[h][w] + 1)*mapData.bitmapData.width;
			bitmap[h][w].setRect(rect);
			bitmap[h][w].setX(w*mapData.bitmapData.width)->setY(h*mapData.bitmapData.height);
			if (map[h][w] == 0)
			{
				Space space;
				space.x1 = w*mapData.bitmapData.width; space.x2 = space.x1 + mapData.bitmapData.width;
				space.y1 = h*mapData.bitmapData.height; space.y2 = space.y1 + mapData.bitmapData.height;
				noSpace.push_back(space);
				
			}
		}
	}
	/*
	bitmap = new Image*[columns];
	for (int i = 0; i < columns; i++)
	{
		bitmap[i] = new Image[rows];
	//	ZeroMemory(bitmap[i], sizeof(Image)*rows);
	}

	for (int column = 0; column < columns; column++)
	{
		for (int row = 0; row < rows; row++)
		{
			bitmap[column][row].initialize(width, height, column, row, false, 0, textureManger, graphics);
			RECT rect = { 0 };
			rect.top = map[column][row]*height; rect.bottom = (map[column][row] + 1)*height;
			rect.left = map[column][row]*width; rect.right = (row + 1)*width;
			bitmap[column][row].setRect(rect);
			bitmap[column][row].setX(row*width).setY(column*height);
		}

	}
	*/

	return true;
}

bool Map::read(std::string mapPath)
{
	mapData.path = mapPath;
	std::ifstream fMap(mapData.path);
	char resol[9] = { 0 };
	fMap.getline(resol, 9, '\n');

	for (int i = 0; i < 8; i++)
		if (resol[i] == 'x')
		{
			resol[i] = 0;
			mapData.height = std::stoi(&resol[i + 1]);
		}

	mapData.width = std::stoi(resol);
	if (mapData.width == 0 | mapData.height == 0)
		return false;

	
	map = new char*[mapData.height];
	for (int i = 0; i < mapData.height; i++)
	{
		map[i] = new char[mapData.width];
		ZeroMemory(map[i], sizeof(char)*mapData.width);
	}

	/*for (int i = 0; i < mapData.height; i++)
	{
		std::string line;
		std::getline(fMap, line);
		for (int j = 0; j < mapData.width; j++)
		{
			//fMap.getline(&map[i][j], 2, ',');
			//map[i][j] = std::stoi(&map[i][j]);
			map[i][j] = line[j * 2];
			map[i][j] = std::stoi(&map[i][j]);
		}
		
		fMap.ignore(1, '\n');
	}
	*/

	std::string line;
	int i = 0;
	while (std::getline(fMap, line))
	{
		for (int j = 0; j < mapData.width; j++)
		{
			map[i][j] = line[j * 2];
			map[i][j] = std::stoi(&map[i][j]);
			if (map[i][j] == 0)
			{
			}
		}

		i++;
	}
	
	fMap.close();

	return true;
}

void Map::draw()
{
	for (int h = 0; h < mapData.height; h++)
	{
		for (int w = 0; w < mapData.width; w++)
		{
			bitmap[h][w].draw();
		}
	}
}

int Map::passX(int x, int x0, int y)
{

	for (auto space : noSpace)
	{
		if (space.x1 <= x & x <= space.x2)
			if (space.y1 <= y & y <= space.y2)
			{
				if (abs(space.x2 - x0) < abs(space.x1 - x0))
					return space.x2;
				else
					return space.x1;
			}
	}

	return 0;
}

int Map::passY(int x, int y, int y0)
{
	for (auto space : noSpace)
	{
		
		if (space.y1 <= y & y <= space.y2)
			if (space.x1 <= x & x <= space.x2)
			{
				if (abs(space.y2 - y0) > abs(space.y1 - y0))
					return space.y1;
				else
					return space.y2;
			}
	}

	return 0;
}



