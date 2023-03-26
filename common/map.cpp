#include "Map.h"
#include "texture.h"
#include "fileio.h"
#include <fstream>

Map::Map() : m_ppMap(0) , m_pGraphics(0), m_pTexture(0)
{
}

Map::~Map()
{
//	for (auto i = 0; i < m_mapData.height; i++)
//		SAFE_DELETE_ARRAY(m_ppMap[i]);
	
//	SAFE_DELETE_ARRAY(m_ppMap);
	for (auto& e : m_ppMap)
		e.clear();
	m_ppMap.clear();
}

void Map::initialize(Texture* texture, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pTexture = texture;
}

bool Map::load(const char* name)
{
	strcpy(m_name, name);
	char path[MAX_PATH] = { 0 };
	getFullPath(path);
	if (!read(path))
		return false;

	uint16_t totalBitmaps = m_mapData.width*m_mapData.height;
	HRESULT hr = m_pGraphics->createVertexBuffer(totalBitmaps * 6 * sizeof(Vertex));
	if (FAILED(hr))
		return false;

	m_bitmapData.width = m_pTexture->getWidth();
	m_bitmapData.height = m_pTexture->getHeight();

/*
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
	*/
	std::vector< std::vector< std::vector<TextureVertices>>> vertices;
	vertices.resize(m_mapData.bitmaps);
	for (auto& element : vertices)
	{
		element.resize(m_mapData.height);
		for (auto& element2 : element)
		{
			element2.resize(m_mapData.width);
			for (auto& element3 : element2)
				element3.v1.x = UNDEFINED_POSITION;
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

			Space space;

			bool prevented = false;
			for (auto preventedBM : m_mapData.preventedBM)
				if (m_ppMap[h][w] == preventedBM)
				{
					prevented = true;
					space.x1 = w*m_bitmapData.width; space.x2 = space.x1 + m_bitmapData.width;
					space.y1 = h*m_bitmapData.height; space.y2 = space.y1 + m_bitmapData.height;
					m_noSpace.push_back(space);
				}

			if (prevented)
				continue;

			space.x1 = w*m_bitmapData.width; space.x2 = space.x1 + m_bitmapData.width;
			space.y1 = h*m_bitmapData.height; space.y2 = space.y1 + m_bitmapData.height;
			m_freeSpace.push_back(space);
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
	return true;
}

const char* Map::setRandomMap()
{
	auto list = FileIO::getDirFileList(MAP_DIR, 0, ".map", false);
	strcpy(m_name, list[_rand(list.size() - 1)].c_str());
	return m_name;
}

bool Map::isMapExist(const char* name, Crc32 crc32)
{
	std::string map(MAP_DIR);
	map.operator+= (name);
	map.operator+=(".map");
	if (!FileIO::isFileExist(map.c_str()))
		return false;

	if (crc32 != FileIO::getCRC32(map.c_str()))
		return false;

	return true;
}

bool Map::read(const char* path)
{
	std::ifstream fMap(path);
	if (!fMap.is_open())
		return false;

	m_mapData = *FileIO::readMapInfo(fMap);

	m_ppMap.resize(m_mapData.height);
	for (auto& element : m_ppMap)
		element.resize(m_mapData.width);

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
		m_pGraphics->setTexture(m_pTexture[i].getTexture());
		m_pGraphics->drawPrimitive(m_startVertex[i], m_lenVertex[i] / 3);
	}
}

Space Map::getFreeSpace() const
{
	std::vector<Space> freeSpace;
	Space space;
	for (auto space : m_freeSpace)
		if (emptyFreeSpace(space))
			freeSpace.push_back(space);


	return freeSpace[_rand(freeSpace.size())];
}

bool Map::spacesCollided(const Space& space1, const Space& space2) const
{
	if ((space2.x1 <= space1.x1 & space2.x2 >= space1.x1) | (space2.x1 <= space1.x2 & space1.x2 <= space2.x2) |
		(space2.x1 >= space1.x1 & space2.x2 >= space1.x1 & space1.x2 >= space2.x1 & space1.x2 >= space2.x2))
		
		if ((space2.y1 <= space1.y1 & space1.y1 <= space2.y2) | (space2.y1 <= space1.y2 & space1.y2 <= space2.y2) |
			(space1.y1 <= space2.y1 & space1.y1 <= space2.y2 & space1.y2 >= space2.y1 & space1.y2 >= space2.y2) )
			return true;

	return false;
}

float Map::passX(float x, float x0, float y, uint16_t width) const
{
	Space os;
	os.x1 = x; os.x2 = os.x1 + 60; os.y1 = y; os.y2 = os.y1 +60;
	for (auto space : m_noSpace)
		if (spacesCollided(space, os))
			return x0;

	return x;
}

float Map::passY(float x, float y, float y0, uint16_t height) const
{
	Space os;
	os.x1 = x; os.x2 = os.x1 + 60; os.y1 = y; os.y2 = os.y1 + 60;
	for (auto space : m_noSpace)
		if (spacesCollided(space, os))
			return y0;

	return y;
}

Object* Map::collided(const Image& object) const // Object has  pure functions so should return a pointer
{
	Object* rObject = 0;
	for (auto& mapObject : m_pObjects)
		if (spacesCollided(object.getAllocatedSpace(), mapObject->getAllocatedSpace()))
		{
			if(mapObject->alive())
				rObject = mapObject;
				break;
		}

	return rObject;
}

bool Map::outOfRange(const Image& object) const
{
	if ((object.getX() > m_mapData.width*m_bitmapData.width) |
		(object.getX() < 0) |
		(object.getY() > m_mapData.width*m_bitmapData.height) |
		(object.getY() < 0))
		return true;
	return false;
}

bool Map::emptyFreeSpace(const Space space) const
{
	for (auto object : m_pObjects)
		if (spacesCollided(space, object->getAllocatedSpace()))
			return false;

	return true;
}

void Map::getFullPath(char* path) const
{
	sprintf(path,"%s%s%s", MAP_DIR, m_name, ".map");
}
