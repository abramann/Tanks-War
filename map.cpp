#include "Map.h"
#include "texturemanger.h"
#include <fstream>

Map::Map() : m_ppMap(0) , m_pGraphics(0), m_pTextureManger(0)
{
}

Map::~Map()
{
	for (auto i = 0; i < m_mapData.height; i++)
		SAFE_DELETE_ARRAY(m_ppMap[i]);
	
	SAFE_DELETE_ARRAY(m_ppMap);
}

bool Map::initialize(const char* path, TextureManger* textureManger, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pTextureManger = textureManger;
	if (!read(path))
		return false;

	uint16_t totalBitmaps = m_mapData.width*m_mapData.height;
	HRESULT hr = m_pGraphics->createVertexBuffer(totalBitmaps * 6 * sizeof(Vertex));
	if (FAILED(hr))
		return false;

	m_bitmapData.width = textureManger->getWidth();
	m_bitmapData.height = textureManger->getHeight();

	TextureVertices*** vertices = new TextureVertices**[m_mapData.bitmaps];
	for (auto i = 0; i < m_mapData.bitmaps; i++)
	{
		vertices[i] = new TextureVertices*[m_mapData.height];
		for (auto j = 0; j < m_mapData.height; j++)
		{
			vertices[i][j] = new TextureVertices[m_mapData.width];
			for (auto k = 0; k < m_mapData.width; k++)
				vertices[i][j][k].v1.x = UNDEFINED_POSITION;
		}
	}

	for (auto h = 0; h < m_mapData.height; h++)
	{
		for (auto w = 0; w < m_mapData.width; w++)
		{
				vertices[m_ppMap[h][w]][h][w].v1 = { (w)*m_bitmapData.width*1.0f ,(h + 1)*m_bitmapData.height*1.0f,1.0f,1.0f,0.0f,1.0f };
				vertices[m_ppMap[h][w]][h][w].v2 = { (w)*m_bitmapData.width*1.0f ,(h)*m_bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				vertices[m_ppMap[h][w]][h][w].v3 = { (w + 1)*m_bitmapData.width*1.0f ,(h)*m_bitmapData.height*1.0f,1.0f,1.0f,1.0f,0.0f };

				vertices[m_ppMap[h][w]][h][w].v4 = { (w + 1)*m_bitmapData.width*1.0f ,(h)*m_bitmapData.height*1.0f,1.0f,1.0f,0.0f,-1.0f };
				vertices[m_ppMap[h][w]][h][w].v5 = { (w + 1)*m_bitmapData.width*1.0f ,(h + 1)*m_bitmapData.height*1.0f,1.0f,1.0f,0.0f,0.0f };
				vertices[m_ppMap[h][w]][h][w].v6 = { (w)*m_bitmapData.width*1.0f ,(h + 1)*m_bitmapData.height*1.0f,1.0f,1.0f,-1.0f,0.0f };
			
			for (auto preventedBM : m_mapData.preventedBM)
				if (m_ppMap[h][w] == preventedBM)
				{
					Space space;
					space.x1 = w*m_bitmapData.width; space.x2 = space.x1 + m_bitmapData.width;
					space.y1 = h*m_bitmapData.height; space.y2 = space.y1 + m_bitmapData.height;
					m_noSpace.push_back(space);
				}
		}
	}

	std::vector<Vertex> pData;
	for (auto i = 0; i < m_mapData.bitmaps; i++)
	{
		m_startVertex.push_back(pData.size());
		for (auto j = 0; j < m_mapData.height; j++)
		{
			for (auto k = 0; k < m_mapData.width; k++)
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

	m_pGraphics->setVertexBuffer(&pData[0], totalBitmaps * 6 * sizeof(Vertex));
	for (auto i = 0; i < m_mapData.bitmaps; i++)
	{
		for (auto j = 0; j < m_mapData.height; j++)
			SAFE_DELETE_ARRAY(vertices[i][j]);

		SAFE_DELETE_ARRAY(vertices[i]);
	}

	SAFE_DELETE_ARRAY(vertices);
	return true;
}

bool Map::read(const char* path)
{
	std::ifstream fMap(path);
	if (!fMap.is_open())
		return false;

	std::string detail;
	std::getline(fMap, detail);
	m_mapData.name = detail;
	std::getline(fMap, detail);
	m_mapData.width = std::stoi(getTargetEqualStringValue(detail));
	std::getline(fMap, detail);
	m_mapData.height = std::stoi(getTargetEqualStringValue(detail));
	std::getline(fMap, detail);
	m_mapData.bitmaps = std::stoi(getTargetEqualStringValue(detail));
	std::getline(fMap, detail);
	uint8_t preventedBMs = std::stoi(getTargetEqualStringValue(detail));
	for (auto i = 0; i < preventedBMs; i++)
	{
		std::getline(fMap, detail);
		m_mapData.preventedBM.push_back(std::stoi(getTargetEqualStringValue(detail)));
	}

	m_ppMap = new char*[m_mapData.height];
	for (uint16_t i = 0; i < m_mapData.height; i++)
		m_ppMap[i] = new char[m_mapData.width];

	std::string line;
	uint16_t i = 0;
	while (std::getline(fMap, line))
	{
		for (uint16_t j = 0; j < m_mapData.width; j++)
		{
			m_ppMap[i][j] = line[j * 2];
			m_ppMap[i][j] = std::stoi(&m_ppMap[i][j]);
		}

		i++;
	}

	return true;
}

void Map::draw()
{
	for (auto i = 0; i < m_mapData.bitmaps; i++)
	{
		m_pGraphics->setTexture(m_pTextureManger[i].getTexture());
		m_pGraphics->drawPrimitive(m_startVertex[i], m_lenVertex[i] / 3);
	}
}

const float Map::passX(float x, float x0, float y)
{
	for (auto space : m_noSpace)
		if (space.x1 <= x & x <= space.x2)
			if (space.y1 <= y & y <= space.y2)
				return (abs(space.x2 - x0) < abs(space.x1 - x0)) ? space.x2 : space.x1;
	return x;
}

const float Map::passY(float x, float y, float y0)
{
	for (auto space : m_noSpace)
		if (space.x1 <= x & x <= space.x2)
			if (space.y1 <= y & y <= space.y2)
				return (abs(space.y2 - y0) > abs(space.y1 - y0)) ? space.y1 : space.y2;
	return y;
}

