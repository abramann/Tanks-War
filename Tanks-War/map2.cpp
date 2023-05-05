#include "map2.h"
#include "game.h"
#include "object2.h"
#include "texturemanger.h"
#include "texture.h"
#include "constants.h"
#include "fileio.h"
#include <fstream>
#include <string>

Map2::Map2()
{
}

Map2::~Map2()
{
	SAFE_RELEASE(m_lpVertexBuffer);
}

void Map2::initialize(const Game * game)
{
	m_pGraphics = game->getGraphics();
	m_pTextureManger = game->getTextureManger();
}

bool Map2::load(const char * map)
{
	strcpy(m_loadedMap, map);
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
						space.v2.y = space.v1.y + m_tiledSize.y;
					m_noSpace.push_back(space);
					break;
				}

			if (prevented)
				continue;

			space.v1.x = w*m_tiledSize.x; space.v2.x = space.v1.x + m_tiledSize.x;
			space.v1.y = h*m_tiledSize.y; space.v2.y = space.v1.y + m_tiledSize.y;
			m_freeSpace.push_back(space);
		}
	}

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
	SAFE_RELEASE(m_lpVertexBuffer);
}

float Map2::passX(const Image2 * image, float x) const
{
	Space is = getImageSpace(image, x, 0);
	float x0 = image->getPosition().x;
	if (isOutOfRange(is))
		return x0;

	for (auto space : m_noSpace)
		if (areSpacesCollided(space, is))
			return x0;

	return x;
}

float Map2::passY(const Image2 * image, float y) const
{
	Space is = getImageSpace(image, 0, y);
	float y0 = image->getPosition().y;
	if (isOutOfRange(is))
		return y0;

	for (auto space : m_noSpace)
		if (areSpacesCollided(space, is))
			return y0;

	return y;
}

bool Map2::isCollided(const Image2 * image) const
{
	Space is = getImageSpace(image);
	if (isOutOfRange(is))
		return true;

	for (auto space : m_noSpace)
		if (areSpacesCollided(space, is))
			return true;
	return false;
}

bool Map2::isOutOfRange(const Space space) const
{
	const int32 x = m_width*m_tiledSize.x,
		y = m_height*m_tiledSize.y;
	if (IN_RANGE(space.v1.x, 0, x) &&
		IN_RANGE(space.v2.x, 0, x) &&
		IN_RANGE(space.v3.x, 0, x) &&
		IN_RANGE(space.v4.x, 0, x)
		)
		if (IN_RANGE(space.v1.y, 0, y) &&
			IN_RANGE(space.v2.y, 0, y) &&
			IN_RANGE(space.v3.y, 0, y) &&
			IN_RANGE(space.v4.y, 0, y)
			)
			return false;
	return true;
}

Object2 * Map2::getObject(V3 position) const
{
	Object2* object = nullptr;
	float x = position.x, y = position.y;
	Space s;
	s.v1 = position;
	for (auto obj : m_object)
	{
		Space os = Map2::getImageSpace(obj);
		if (areSpacesCollided(os, s))
		{
			object = obj;
			break;
		}
	}

	return object;
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
	if (IN_RANGE(s2.v1.x, s1.v1.x, s1.v2.x) ||
		IN_RANGE(s2.v2.x, s1.v1.x, s1.v2.x) ||
		IN_RANGE(s2.v3.x, s1.v1.x, s1.v2.x) ||
		IN_RANGE(s2.v4.x, s1.v1.x, s1.v2.x)
		)
		if (IN_RANGE(s2.v1.y, s1.v1.y, s1.v2.y) ||
			IN_RANGE(s2.v2.y, s1.v1.y, s1.v2.y) ||
			IN_RANGE(s2.v3.y, s1.v1.y, s1.v2.y) ||
			IN_RANGE(s2.v4.y, s1.v1.y, s1.v2.y)
			)
			return true;

	return false;
}

Space Map2::getImageSpace(const Image2* image, float x0, float y0) 
{
	int16 width = image->getWidth(),
		height = image->getHeight();
	if (x0 == 0)
		x0 = image->getPosition().x;
	if (y0 == 0)
		y0 = image->getPosition().y;

	float angle = image->getRotate().z;

	int8 s = 1;
	if (angle > 0.0001)
		s = +1;
	else if (angle < -0.0001)
		s = -1;

	float f1 = 1 + (-0.636619772*abs(angle));
	float f2 = -1 * s*(abs(f1) - 1);
	Space is;
	is.v1.x = x0, is.v1.y = y0;
	is.v2.x = is.v1.x + width*(f1),
		is.v2.y = is.v1.y + (height *f2);
	is.v3.x = is.v2.x + width*(-1 * f2);
	is.v3.y = is.v2.y + height*f1;
	is.v4.x = is.v1.x + width*(-1 * f2);
	is.v4.y = is.v1.y + height*f1;
	return is;
}