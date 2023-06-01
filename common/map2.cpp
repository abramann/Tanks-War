// map.cpp
// Author: abramann

#include "map2.h"
#include "game.h"
#include "object2.h"
#include "texturemanger.h"
#include "texture.h"
#include "constants.h"
#include "fileio.h"
#include <fstream>
#include <string>

Map2::Map2() : m_lpVertexBuffer(0)
{
}

Map2::~Map2()
{
	safeRelease(m_lpVertexBuffer);
}

void Map2::initialize(const Game * game)
{
	m_pGraphics = game->getGraphics();
	m_pTextureManger = game->getTextureManger();
}

bool Map2::load(const char * map)
{
	strcpy_s(m_loadedMap, map);
	if (!read())
		return false;

	int32 totalBitmaps = m_width*m_height;
	for (int i = 0; i < TEXTURE_TILEDS; i++)
		m_pTexture[i] = m_pTextureManger->getTexture(i + TEXTURE_TILED0);

	m_tiledSize.x = m_pTexture[0]->getWidth();;
	m_tiledSize.y = m_pTexture[0]->getHeight();
	std::vector< std::vector< std::vector<TextureVertices>>> vertices;
	vertices.resize(m_usedBitmaps);
	for (auto& element : vertices)
	{
		element.resize(m_height);
		for (auto& element2 : element)
		{
			element2.resize(m_width);
			for (auto& element3 : element2)
			{
				memset(&element3, 0, sizeof(Vertex) * 6);
				element3.v1.x = UNDEFINED_POSITION;
			}
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

	m_lpVertexBuffer = m_pGraphics->createVertexBuffer(totalBitmaps * 6, VB_USAGE_CONST, &pData[0]);
	LPDevice device = m_pGraphics->getDevice();
	return true;
}

void Map2::draw() const
{
	m_pGraphics->setDrawProperties();
	m_pGraphics->streamVertexBuffer(m_lpVertexBuffer);
	for (auto i = 0; i < m_usedBitmaps; i++)
	{
		m_pGraphics->setTexture(m_pTexture[i]->getTexture());
		m_pGraphics->drawPrimitive(m_startVertex[i], m_lenVertex[i] / 3);
	}
}

void Map2::clear()
{
	m_freeSpace.clear();
	m_noSpace.clear();
	m_lenVertex.clear();
	safeRelease(m_lpVertexBuffer);
}

float Map2::passX(const Object2 * object, float x)
{
	Space is = object->getSpace(x, 0);
	float x0 = object->getPosition().x;
	if (isCollided(is, object))
		return x0;

	return x;
}

float Map2::passY(const Object2 * object, float y)
{
	Space is = object->getSpace(0, y);
	float y0 = object->getPosition().y;
	if (isCollided(is, object))
		return y0;

	return y;
}

bool Map2::isCollided(const Image2* image)
{
	Space s = image->getSpace();
	bool r = isCollided(s);
	return r;
}

bool Map2::isCollided(const Object2 * object)
{
	Space is = object->getSpace();
	bool r = isCollided(is, object);
	return r;
}

bool Map2::isCollided(const Space is, const Object2* object)
{
	if (isOutOfRange(is))
		return true;
	for (auto space : m_noSpace)
		if (areSpacesCollided(space, is))
			return true;
	for (auto pObj : m_pObject)
	{
		if (object == pObj)
			continue;

		Space space = pObj->getSpace();
		if (areSpacesCollided(space, is))
			return true;
	}

	return false;
}

bool Map2::isOutOfRange(const Space space) const
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

Object2 * Map2::getObject(const Space space)
{
	Object2* pObject = 0;
	for (auto _pObject : m_pObject)
	{
		Space is = _pObject->getSpace();
		if (areSpacesCollided(space, is))
		{
			pObject = _pObject;
			break;
		}
	}

	return pObject;
}

bool Map2::read()
{
	std::string path = MAP_DIR;
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
	int32 i = 0;
	while (std::getline(file, line))
	{
		for (int32 j = 0; j < m_width; j++)
		{
			m_map[i][j] = line[j * 2];
			byte n = m_map[i][j];
			m_map[i][j] = std::stoi((char*)&n);
		}

		i++;
	}

	return true;
}

bool Map2::areSpacesCollided(const Space s1, const Space s2) const
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

void Map2::clearUnnecessaryNospace()
{
	for (size_t i = 0; i < m_noSpace.size(); i++)
		if (isNospaceUseless(m_noSpace[i]))
		{
			m_noSpace.erase(std::next(m_noSpace.begin(), i));
			i--;
		}
}

Space Map2::getRightSpace(Space s) const
{
	s.addX(m_tiledSize.x);
	if (isOutOfRange(s))
		s.v1.x = UNDEFINED_POSITION;

	return s;
}

Space Map2::getLeftSpace(Space s) const
{
	s.addX(-m_tiledSize.x);
	if (isOutOfRange(s))
		s.v1.x = UNDEFINED_POSITION;

	return s;
}

Space Map2::getUpSpace(Space s) const
{
	s.addY(m_tiledSize.y);
	if (isOutOfRange(s))
		s.v1.x = UNDEFINED_POSITION;

	return s;
}

Space Map2::getDownSpace(Space s) const
{
	s.addY(-m_tiledSize.y);
	if (isOutOfRange(s))
		s.v1.x = UNDEFINED_POSITION;

	return s;
}

bool Map2::isNospaceUseless(Space s) const
{
	Space space[] = { getUpSpace(s),getDownSpace(s),getRightSpace(s),getLeftSpace(s) };
	for (int i = 0; i < SPACE_VERTICES; i++)
	{
		if (space[i].isValid())
			if (isFreeSpace(space[i]))
				return false;
	}
	return true;
}

bool Map2::isFreeSpace(Space s) const
{
	for (Space freeSpace : m_freeSpace)
		if (s.isSame(freeSpace))
		{
			return true;
		}
	return false;
}

Crc32 Map2::getCrc32() const
{
	std::string path(MAP_DIR);
	path.operator+=(m_loadedMap);
	path.operator+=(".map");
	const char* cPath = path.c_str();
	Crc32 crc32 = FileIO::getCRC32(cPath);
	return crc32;
}

const char* Map2::loadRandom()
{
	std::vector<std::string> list = FileIO::getDirFileList(MAP_DIR);
	std::string map = list[_rand(list.size() - 1)];
	map = map.substr(0, map.size() - 4);
	load(map.c_str());
	return m_loadedMap;
}

Space Map2::getRandomEmptySpace() const
{
	Space freeSpace = m_freeSpace[_rand(m_freeSpace.size())];
	return freeSpace;
}

void Map2::addObject(Object2* object)
{
	m_pObject.push_back(object);
}
