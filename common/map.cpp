// map.cpp
// Author: abramann

#include "map.h"
#include "game.h"
#include "object.h"
#include "texturemanger.h"
#include "texture.h"
#include "constants.h"
#include "fileio.h"
#include <fstream>
#include <string>

Map::Map() : m_lpVertexBuffer(0)
{
}

Map::~Map()
{
	safeRelease(m_lpVertexBuffer);
}

void Map::initialize(const Game * game)
{
	m_pGraphics = game->getGraphics();
	m_pTextureManger = game->getTextureManger();
}

bool Map::load(const char * map)
{
	strcpy_s(m_loadedMap, map);
	if (!read())
		return false;

	int32 totalBitmaps = m_width*m_height;
	for (int i = 0; i < textureNS::TEXTURE_TILEDS; i++)
		m_pTexture[i] = m_pTextureManger->getTexture(i + TEXTURE_TILED0);

	m_tiledSize.x = m_pTexture[0]->getWidth();;
	m_tiledSize.y = m_pTexture[0]->getHeight();
	std::vector<std::vector< std::vector<TextureVertices>>> vertices(m_usedBitmaps);
	for (auto& element : vertices)
	{
		element.resize(m_height);
		for (auto& element2 : element)
		{
			element2.resize(m_width);
			for (auto& element3 : element2)
				element3.v1.x = mapNS::UNDEFINED_POSITION;
		}
	}

	for (auto h = 0; h < m_height; h++)
	{
		for (auto w = 0; w < m_width; w++)
		{
			vertices[m_map[h][w]][h][w].v1 = { (w + 1)*m_tiledSize.x ,(h)*m_tiledSize.y,0.0f,0.0f,1.0f };
			vertices[m_map[h][w]][h][w].v2 = { (w)*m_tiledSize.x,(h)*m_tiledSize.y,0.0f,1.0f,1.0f };
			vertices[m_map[h][w]][h][w].v3 = { (w)*m_tiledSize.x ,(h + 1)*m_tiledSize.y,0.0f,1.0f,0.0f };
			vertices[m_map[h][w]][h][w].v4 = { (w)*m_tiledSize.x ,(h + 1)*m_tiledSize.y,0.0f,-1.0f,0.0f };
			vertices[m_map[h][w]][h][w].v5 = { (w + 1)*m_tiledSize.x ,(h + 1)*m_tiledSize.y,0.0f,0.0f,0.0f };
			vertices[m_map[h][w]][h][w].v6 = { (w + 1)*m_tiledSize.x,(h)*m_tiledSize.y,0.0f,0.0f,1.0f };
			Space space;
			bool prevented = false;
			for (auto preventedBM : m_preventedBitmap)
				if (m_map[h][w] == preventedBM)
				{
					prevented = true;
					space.v1.x = w*m_tiledSize.x,
						space.v1.y = h*m_tiledSize.y;
					space.v2.x = space.v1.x + m_tiledSize.x,
						space.v2.y = space.v1.y,
						space.v3.x = space.v1.x + m_tiledSize.x, space.v3.y = space.v1.y + m_tiledSize.y,
						space.v4.x = space.v1.x, space.v4.y = space.v1.y + m_tiledSize.y;
					m_noSpace.push_back(space);
					break;
				}

			if (prevented)
				continue;

			space.v1.x = w*m_tiledSize.x;
			space.v1.y = h*m_tiledSize.y;
			space.v2.x = space.v1.x + m_tiledSize.x;
			space.v2.y = space.v1.y;
			space.v3.x = space.v2.x;
			space.v3.y = space.v1.y + m_tiledSize.y;
			space.v4.x = space.v1.x;
			space.v4.y = space.v3.y;
			m_freeSpace.push_back(space);
		}
	}

	clearUnnecessaryNospace();
	std::vector<Vertex> pData;
	for (auto i = 0; i < m_usedBitmaps; i++)
	{
		m_startVertex.push_back(pData.size());
		for (auto j = 0; j < m_height; j++)
		{
			for (auto k = 0; k < m_width; k++)
			{
				if (vertices[i][j][k].v1.x == mapNS::UNDEFINED_POSITION)
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

	m_lpVertexBuffer = m_pGraphics->createVertexBuffer(totalBitmaps * 6, VB_USAGE_CONST, &pData[0]);
	return true;
}

void Map::draw() const
{
	m_pGraphics->setDrawProperties();
	m_pGraphics->streamVertexBuffer(m_lpVertexBuffer);
	for (auto i = 0; i < m_usedBitmaps; i++)
	{
		m_pGraphics->setTexture(m_pTexture[i]->getTexture());
		m_pGraphics->drawPrimitive(m_startVertex[i], m_lenVertex[i] / 3);
	}
}

void Map::clear()
{
	m_freeSpace.clear();
	m_noSpace.clear();
	m_lenVertex.clear();
	safeRelease(m_lpVertexBuffer);
}

float Map::passX(const Object * object, float x) const
{
	Space is = object->getSpace(x, 0);
	float x0 = object->getPosition().x;
	bool collided = isCollided(is, object);
	if (collided)
		return x0;

	return x;
}

float Map::passY(const Object * object, float y) const
{
	Space is = object->getSpace(0, y);
	float y0 = object->getPosition().y;
	bool collided = isCollided(is, object);
	if (collided)
		return y0;

	return y;
}

bool Map::isCollided(const Image* image) const
{
	Space s = image->getSpace();
	bool collided = isCollided(s);
	return collided;
}

bool Map::isCollided(const Object * object) const
{
	Space is = object->getSpace();
	bool collided = isCollided(is, object);
	return collided;
}

bool Map::isCollided(const Space is, const Object* object) const
{
	bool outOfRange = isOutOfRange(is);
	if (outOfRange)
		return true;
	for (auto space : m_noSpace)
	{
		bool spacesCollided = areSpacesCollided(space, is);
		if (spacesCollided)
			return true;
	}
	for (auto pObj : m_pObject)
	{
		if (object == pObj)
			continue;

		Space space = pObj->getSpace();
		bool spacesCollided = areSpacesCollided(space, is);
		if (spacesCollided)
			return true;
	}

	return false;
}

bool Map::isOutOfRange(const Space space) const
{
	const int32 x = m_width*m_tiledSize.x,
		y = m_height*m_tiledSize.y;
	if (IN_RANGE_OR_EQUAL(space.v1.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v2.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v3.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v4.x, 0, x)
		)
		if (IN_RANGE_OR_EQUAL(space.v1.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v2.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v3.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v4.y, 0, y)
			)
			return false;

	return true;
}

Object * Map::getObject(const Space space) const
{
	Object* pObject = 0;
	for (auto _pObject : m_pObject)
	{
		Space is = _pObject->getSpace();
		bool collided = areSpacesCollided(space, is);
		if (collided)
		{
			pObject = _pObject;
			break;
		}
	}

	return pObject;
}

bool Map::read()
{
	std::string path = fileNS::MAP_DIR;
	path.operator+=(m_loadedMap);
	path.operator+=(".map");
	std::ifstream file(path);
	if (!file.is_open())
		return false;

	MapData mapData = FileIO::readMapInfo(file);
	m_width = mapData.width;
	m_height = mapData.height;
	m_usedBitmaps = mapData.bitmaps;
	m_preventedBitmap = mapData.preventedBM;
	m_map.resize(m_height);
	for (auto& element : m_map)
		element.resize(m_width);

	std::string line;
	for (int32 i = 0; std::getline(file, line); i++)
	{
		for (int32 j = 0; j < m_width; j++)
		{
			m_map[i][j] = line[j * 2];
			m_map[i][j] = std::atof(&m_map[i][j]);
			bool exist = false;
			for (int k = 0; k < m_usedBitmaps; k++) // check if the map element exist if used map elements
			{
				if (m_map[i][j] == k)
				{
					exist = true;
					break;
				}
			}
			if (!exist)
				return false;
		}
	}

	return true;
}

bool Map::areSpacesCollided(const Space s1, const Space s2) const
{
	float maxX1 = s1.getMaxX(),
		minX1 = s1.getMinX(),
		maxY1 = s1.getMaxY(),
		minY1 = s1.getMinY();
	if (IN_RANGE(s2.v1.x, minX1, maxX1) ||
		IN_RANGE(s2.v2.x, minX1, maxX1) ||
		IN_RANGE(s2.v3.x, minX1, maxX1) ||
		IN_RANGE(s2.v4.x, minX1, maxX1)
		)
		if (IN_RANGE(s2.v1.y, minY1, maxY1) ||
			IN_RANGE(s2.v2.y, minY1, maxY1) ||
			IN_RANGE(s2.v3.y, minY1, maxY1) ||
			IN_RANGE(s2.v4.y, minY1, maxY1)
			)
			return true;

	return false;
}

void Map::clearUnnecessaryNospace()
{
	for (size_t i = 0; i < m_noSpace.size(); i++)
	{
		bool isUseless = isNospaceUseless(m_noSpace[i]);
		if (isUseless)
		{
			m_noSpace.erase(std::next(m_noSpace.begin(), i));
			i--;
		}
	}
}

Space Map::getRightSpace(Space s) const
{
	s.addX(m_tiledSize.x);
	if (isOutOfRange(s))
		s.v1.x = mapNS::UNDEFINED_POSITION;

	return s;
}

Space Map::getLeftSpace(Space s) const
{
	s.addX(-m_tiledSize.x);
	if (isOutOfRange(s))
		s.v1.x = mapNS::UNDEFINED_POSITION;

	return s;
}

Space Map::getUpSpace(Space s) const
{
	s.addY(m_tiledSize.y);
	if (isOutOfRange(s))
		s.v1.x = mapNS::UNDEFINED_POSITION;

	return s;
}

Space Map::getDownSpace(Space s) const
{
	s.addY(-m_tiledSize.y);
	if (isOutOfRange(s))
		s.v1.x = mapNS::UNDEFINED_POSITION;

	return s;
}

bool Map::isNospaceUseless(Space s) const
{
	Space space[] = { getUpSpace(s),getDownSpace(s),getRightSpace(s),getLeftSpace(s) };
	for (int i = 0; i < mapNS::SPACE_VERTICES; i++)
	{
		if (space[i].isValid())
			if (isFreeSpace(space[i]))
				return false;
	}
	return true;
}

bool Map::isFreeSpace(Space s) const
{
	for (Space freeSpace : m_freeSpace)
		if (s.isSame(freeSpace))
			return true;

	return false;
}

Crc32 Map::getCrc32() const
{
	std::string path(fileNS::MAP_DIR);
	path.operator+=(m_loadedMap);
	path.operator+=(".map");
	const char* cPath = path.c_str();
	Crc32 crc32 = FileIO::getCRC32(cPath);
	return crc32;
}

const char* Map::loadRandom()
{
	std::vector<std::string> list = FileIO::getDirFileList(fileNS::MAP_DIR);
	std::string map = list[_rand(list.size() - 1)];
	map = map.substr(0, map.size() - 4);
	load(map.c_str());
	return m_loadedMap;
}

Space Map::getRandomEmptySpace() const
{
	Space freeSpace = m_freeSpace[_rand(m_freeSpace.size())];
	return freeSpace;
}

void Map::addObject(Object* object)
{
	m_pObject.push_back(object);
}