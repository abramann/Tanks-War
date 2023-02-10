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
	}
	
	SAFE_DELETE_ARRAY(map);
}

bool Map::initialize(const char* path, TextureManger* _textureManger, Graphics* _graphics)
{
	graphics = _graphics;
	textureManger = _textureManger;

	std::vector<std::vector<TextureVertices>> vert;
	std::vector<TextureVertices> vt0, vt1;

	if (!read(path))
		return false;

	m_totalBitmaps = mapData.width*mapData.height;
	UINT verticesSize = m_totalBitmaps * 6 * sizeof(Vertex);
	HRESULT hr = graphics->createVertexBuffer(m_totalBitmaps * 6);
	if(FAILED(hr))
		return false;

	bitmapData.width = textureManger->getWidth();
	bitmapData.height = textureManger->getHeight();
	
	TextureVertices*** vertices = new TextureVertices**[m_bitmaps];

	for (int i = 0; i < m_bitmaps; i++)
	{
		vertices[i] = new TextureVertices*[mapData.height];
		for (int j = 0; j < mapData.height; j++)
		{
			vertices[i][j] = new TextureVertices[mapData.width];
			for (int k = 0; k < mapData.width; k++)
				vertices[i][j][k].v1.x = UNDEFINED_POSITION;
		}
	}

	for (int h = 0; h < mapData.height; h++)
	{
		for (int w = 0; w < mapData.width; w++)
		{
			if (map[h][w] == 0)
			{
				TextureVertices tv0;
				tv0.v1 = { w*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,1.0f };
				tv0.v2 = { w*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				tv0.v3 = { (w + 1)*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,1.0f,0.0f };

				tv0.v4 = { (w + 1)*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,-1.0f };
				tv0.v5 = { (w + 1)*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				tv0.v6 = { (w)*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,-1.0f,0.0f };
				vertices[0][h][w] = tv0;
				Space space;
				space.x1 = w*bitmapData.width; space.x2 = space.x1 + bitmapData.width;
				space.y1 = h*bitmapData.height; space.y2 = space.y1 + bitmapData.height;
				noSpace.push_back(space);
			}
			else
			{
				vertices[map[h][w]][h][w].v1 = { w*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,1.0f };
				vertices[map[h][w]][h][w].v2 = { w*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				vertices[map[h][w]][h][w].v3 = { (w + 1)*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,1.0f,0.0f };

				vertices[map[h][w]][h][w].v4 = { (w + 1)*bitmapData.width*1.0f ,(h)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,-1.0f };
				vertices[map[h][w]][h][w].v5 = { (w + 1)*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				vertices[map[h][w]][h][w].v6 = { (w)*bitmapData.width*1.0f ,(h + 1)*bitmapData.height*1.0f,1.0f,1.0f,-1.0f,0.0f };

			}
		}
	}

	std::vector<Vertex> pData;
	for (int i = 0; i < m_bitmaps; i++)
	{
		m_startVertex.push_back(pData.size());
		for (int j = 0; j < mapData.height; j++)
		{
			for (int k = 0; k < mapData.width; k++)
			{
				if (vertices[i][j][k].v1.x == UNDEFINED_POSITION)
					continue;

				pData.push_back(vertices[i][j][k].v1);
				pData.push_back(vertices[i][j][k].v2);
				pData.push_back(vertices[i][j][k].v3);
				pData.push_back(vertices[i][j][k].v4);
				pData.push_back(vertices[i][j][k].v5);
				pData.push_back(vertices[i][j][k].v6);
			}
		}

		m_lenVertex.push_back(pData.size() - m_startVertex[i]);
	}

	graphics->setVertexBuffer(&pData[0], verticesSize);
	for (int i = 0; i < m_bitmaps; i++)
	{
		for (int j = 0; j < mapData.height; j++)
			SAFE_DELETE_ARRAY(vertices[i][j]);
		SAFE_DELETE_ARRAY(vertices[i]);
	}

		SAFE_DELETE_ARRAY(vertices);

	return true;
}

bool Map::read(const char* path)
{
	for (char * i = (char*)path + strlen(path); i >= path; i--)
		if (*i == '\\' | *i == '/' | i-1 < path)
		{
			mapData.name = i;
			break;
		}

	std::ifstream fMap(path);
	std::string detail;
	std::getline(fMap, detail);
	mapData.width = getValueFromString(detail);
	std::getline(fMap, detail);
	mapData.height = getValueFromString(detail);
	std::getline(fMap, detail);
	m_bitmaps = getValueFromString(detail);
	
	map = new char*[mapData.height];
	for (int i = 0; i < mapData.height; i++)
		map[i] = new char[mapData.width];

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
	for (int i = 0; i < m_bitmaps; i++)
	{
		graphics->setTexture(textureManger[i].getTexture());
		int ll = m_lenVertex[i];
		graphics->drawPrimitive(m_startVertex[i], m_lenVertex[i] / 3);
	}
	
	return;
}

float Map::passX(float x, float x0, float y)
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

	return x;
}

float Map::passY(float x, float y, float y0)
{
	for (auto space : noSpace)
	{
		if (space.x1 <= x & x <= space.x2)
			if (space.y1 <= y & y <= space.y2)
			{
				if (abs(space.y2 - y0) > abs(space.y1 - y0))
					return space.y1;
				else
					return space.y2;
			}
	}

	return y;
}

/*
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

	return y;
}
*/


