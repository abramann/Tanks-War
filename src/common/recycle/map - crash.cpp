// map.cpp
// Author: abramann

#include "map.h"
#include "game.h"
#include "object.h"
#include "texturemanger.h"
#include "texture.h"
#include "types.h"
#include "dx11wrapper.h"
#include "fileio.h"
#include "collisionshader.h"
#include "inlined.inl"
#include "gamemath.h"
#include <fstream>
#include <string>

#pragma warning(disable : 4267) // conversion from size_t to uint32

Map::Map() : m_pVertexBuf(0), m_pNoSpaceBuf(0), m_pNoSpaceSRV(0), m_pNoSpaceCountBuf(0),
m_pNoSpaceCountSRV(0), m_pCollisionCS(0), m_pSpaceBuf(0), m_pSpaceSRV(0),
m_pResultBuf(0), m_pResultUAV(0), m_pResultStagingBuf(0), m_threadGroups(1)
{
}

Map::~Map()
{
}

void Map::initialize(const Game * pGame)
{
	m_pGraphics = pGame->getGraphics();
	m_pTextureManger = pGame->getTextureManger();
	m_pDx11Wrapper = pGame->getDx11Wrapper();
	m_pCollisionCS = m_pDx11Wrapper->createComputeShader(g_pCollisionShader, ARRAYSIZE(g_pCollisionShader));
	m_pResultBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(int), 1, 0, 0);
	m_pResultUAV = m_pDx11Wrapper->createBufferUAV(m_pResultBuf.Get());
	m_pResultStagingBuf = m_pDx11Wrapper->createStagingBuffer(D3D11_CPU_ACCESS_READ, sizeof(int), 1, 0);
	m_pSpaceBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(Space), 1, 0, D3D11_CPU_ACCESS_WRITE);
	m_pSpaceSRV = m_pDx11Wrapper->createBufferSRV(m_pSpaceBuf.Get());
	for (int i = 0; i < textureNS::TEXTURE_TILEDS; i++)
		m_pTexture[i] = m_pTextureManger->getTexture(strFormat("tiled-%d", i));
}

#ifdef _DEBUG
#pragma optimize("",on)
#endif
bool Map::load(const std::string& map)
{
	m_loadedMap = map;
	if (!read())
		return false;

	int32 totalBitmaps = m_width*m_height;
	m_tiledSize.x = static_cast<float>(m_pTexture[0]->getWidth());
	m_tiledSize.y = static_cast<float>(m_pTexture[0]->getHeight());
	m_maxDistance = sqrt(pow(m_width*m_tiledSize.x, 2) + pow(m_height*m_tiledSize.y, 2));
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

	// create the grid
	for (auto h = 0; h < m_height; h++)
	{
		for (auto w = 0; w < m_width; w++)
		{
			vertices[m_map[h][w]][h][w].v1 = { (w + 1)*m_tiledSize.x ,(h+1)*m_tiledSize.y,0.0f,-1.0f,-1.0f };
			vertices[m_map[h][w]][h][w].v2 = { (w+1)*m_tiledSize.x,(h)*m_tiledSize.y,0.0f,-1.0f,0.0f };
			vertices[m_map[h][w]][h][w].v3 = { (w)*m_tiledSize.x ,(h)*m_tiledSize.y,0.0f,0.0f,0.0f };
			vertices[m_map[h][w]][h][w].v4 = { (w)*m_tiledSize.x ,(h + 1)*m_tiledSize.y,0.0f,0.0f,-1.0f };

			Space space;
			bool noSpace = false;
			for (auto noSpaceBitmap : m_noSpaceBitmap)
				if (m_map[h][w] == noSpaceBitmap)
				{
					noSpace = true;
					space = vertices[m_map[h][w]][h][w].getSpace();
					m_noSpace.push_back(space);
					break;
				}

			if (noSpace)
				continue;

			space = vertices[m_map[h][w]][h][w].getSpace();
			m_freeSpace.push_back(space);
		}
	}

	clearUnnecessaryNospace();

	// organize vertices in a 2d vector waith sort each bitmap as series
	std::vector<Vertex> vertexData;
	
	struct TextureIndex
	{
		uint32 i[6];	// each 2 triangles requires 6 index
	};

	std::vector<TextureIndex> index(totalBitmaps, { 0 });
	for (auto n = 0, i = 0; i < m_usedBitmaps; i++)
	{
		m_startVertex.push_back(vertexData.size());
		for (auto j = 0; j < m_height; j++)
		{
			for (auto k = 0; k < m_width; k++)
			{
				if (!vertices[i][j][k].isValid())
					continue;	// vertices are allocated by another bitmap type

				vertexData.push_back(vertices[i][j][k].v1);
				vertexData.push_back(vertices[i][j][k].v2);
				vertexData.push_back(vertices[i][j][k].v3);
				vertexData.push_back(vertices[i][j][k].v4);

				// as vertices in vertex buffer organized, no need to organize index
				if (n != totalBitmaps)
				{
					index[n].i[0] = 0 + n * 4, index[n].i[1] = 1 + n * 4, index[n].i[2] = 2 + n * 4;
					index[n].i[3] = 0 + n * 4, index[n].i[4] = 2 + n * 4, index[n].i[5] = 3 + n * 4;
					n++;
				}
			}
		}
		
		m_lenVertex.push_back(vertexData.size() - m_startVertex[i]);
	}

	m_pVertexBuf = m_pDx11Wrapper->createVertexBuffer(totalBitmaps * 4, 0, &vertexData[0]);
	m_pIndexBuf = m_pDx11Wrapper->createIndexBuffer(totalBitmaps * sizeof(uint32) * 6, 0, &index[0]);
	if (m_noSpace.size() == 0)
		m_noSpace.push_back(Space());

	m_pNoSpaceBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(Space), m_noSpace.size(), &m_noSpace[0], 0);
	m_pNoSpaceSRV = m_pDx11Wrapper->createBufferSRV(m_pNoSpaceBuf.Get());
	uint32 noSpaceCount = m_noSpace.size();
	m_pNoSpaceCountBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(uint32), 1, (void*)&noSpaceCount, 0);
	m_pNoSpaceCountSRV = m_pDx11Wrapper->createBufferSRV(m_pNoSpaceCountBuf.Get());
	int32 mapRange[] = { static_cast<int32>(m_tiledSize.x*m_width), static_cast<int32>(m_tiledSize.y*m_height) };
	m_pMapRangeBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(int32), 2, mapRange, 0);
	m_pMapRangeSRV = m_pDx11Wrapper->createBufferSRV(m_pMapRangeBuf.Get());
	m_threadGroups = (m_noSpace.size() / dxNS::THREADS_PER_GROUP);
	float f = ((m_noSpace.size()*1.0f) / (dxNS::THREADS_PER_GROUP*1.0f)) - m_threadGroups;
	if (f > 0.001f)
		m_threadGroups++;

	return true;
}

void Map::clearUnnecessaryNospace()
{
	for (size_t i = 0; i < m_noSpace.size(); i++)
		if (isNospaceUseless(m_noSpace[i]))
		{
			m_noSpace.erase(std::next(m_noSpace.begin(), i));
			i--;
		}

	// For X axis
	for (size_t i = 0; i < m_noSpace.size(); i++)
	{
		Space& ns = m_noSpace[i];
		for (size_t j = 0; j < m_noSpace.size(); j++)
		{
			if (i == j)
				continue;

			Space& ns2 = m_noSpace[j];
			if (ns.getMinX() == ns2.getMinX() &&
				ns.getMaxX() == ns2.getMaxX())
			{
				if (ns.getMaxY() == ns2.getMinY() || ns.getMaxY() == ns2.getMaxY() ||
					ns.getMinY() == ns2.getMinY() || ns.getMinY() == ns2.getMaxY())
				{
					ns.v1.y = min(ns.getMinY(), ns2.getMinY());
					ns.v2.y = ns.v1.y;
					ns.v3.y = max(ns.getMaxY(), ns2.getMaxY());
					ns.v4.y = ns.v3.y;

					m_noSpace.erase(std::next(m_noSpace.begin(), j));
					i = -1;
					break;
				}
			}
		}
	}

	// For Y axis
	for (size_t i = 0; i < m_noSpace.size(); i++)
	{
		Space& ns = m_noSpace[i];
		for (size_t j = 0; j < m_noSpace.size(); j++)
		{
			if (i == j)
				continue;

			Space& ns2 = m_noSpace[j];
			if (ns.getMinY() == ns2.getMinY() &&
				ns.getMaxY() == ns2.getMaxY())
			{
				if (ns.getMaxX() == ns2.getMinX() || ns.getMaxX() == ns2.getMaxX() ||
					ns.getMinX() == ns2.getMinX() || ns.getMinX() == ns2.getMaxX())
				{
					ns.v1.x = min(ns.getMinX(), ns2.getMinX());
					ns.v2.x = max(ns.getMaxX(), ns2.getMaxX());
					ns.v3.x = ns.v2.x;
					ns.v4.x = ns.v1.x;
					m_noSpace.erase(std::next(m_noSpace.begin(), j));
					i = -1;
					break;
				}
			}
		}
	}
}
#ifdef _DEBUG
#pragma optimize("", off)
#endif

void Map::draw() const
{
	m_pGraphics->setDrawProperties();
	m_pDx11Wrapper->iaSetIndexBuffer(m_pIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pDx11Wrapper->streamVertexBuffer(m_pVertexBuf.Get());
	for (int8 i = 0; i < m_usedBitmaps; i++)
	{
		m_pGraphics->setTexture(m_pTexture[i]->getTexture());
		m_pDx11Wrapper->d3dDrawIndexed(m_lenVertex[i] *1.5f, m_startVertex[i] * 1.5, 0); // index per teture vertex = 6 / 4 =  1.5
	}
}

void Map::clear()
{
	m_freeSpace.clear();
	m_noSpace.clear();
	m_lenVertex.clear();
	m_pObject.clear();
}

float Map::passX(const Object * thisObject, float x) const
{
	Space is = thisObject->getSpace(x, 0);
	float x0 = thisObject->getPosition().x;
	if (isCollided(is, { thisObject }))
		return x0;

	return x;
}

float Map::passY(const Object * thisObject, float y) const
{
	Space is = thisObject->getSpace(0, y);
	float y0 = thisObject->getPosition().y;
	if (isCollided(is, { thisObject }))
		return y0;

	return y;
}

bool Map::isCollided(const Image* pImage) const
{
	return isCollided(pImage->getSpace());
}

bool Map::isCollided(const Object * pObject) const
{
	return isCollided(pObject->getSpace(), { pObject });
}

bool Map::isCollided(const Space& is, const std::vector<const Object*> pExceptObject) const
{
	if (g_pGameSettings->computeShader)
	{
		m_pDx11Wrapper->copyToResource(m_pSpaceBuf.Get(), (void*)&is, sizeof(Space));
		DxShaderResourceView* ppSRV[] = { m_pNoSpaceSRV.Get(),m_pNoSpaceCountSRV.Get(), m_pSpaceSRV.Get(), m_pMapRangeSRV.Get() };
		DxUnorderedAccessView* ppUAV[] = { m_pResultUAV.Get() };
		m_pDx11Wrapper->runComputeShader(m_pCollisionCS.Get(), ARRAYSIZE(ppSRV), ppSRV, ARRAYSIZE(ppUAV), ppUAV,
			m_threadGroups, 1, 1);
		m_pDx11Wrapper->copyResourceToResource(m_pResultStagingBuf.Get(), m_pResultBuf.Get());
		int32 collided = 0;
		m_pDx11Wrapper->copyResource(&collided, m_pResultStagingBuf.Get(), 4);
		if (collided)
			return true;
	}
	else
	{
		if (isOutOfRange(is))
			return true;

		for (auto space : m_noSpace)
			if (areSpacesCollided(space, is))
				return true;
	}
	for (auto pObj : m_pObject)
	{
		if(std::find(pExceptObject.begin(), pExceptObject.end() , pObj) == pExceptObject.end())
			continue;

		if (areSpacesCollided(pObj->getSpace(), is))
			return true;
	}

	return false;
}

bool Map::isOutOfRange(const Space& space) const
{
	const auto x = m_width*m_tiledSize.x,
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

Object * Map::getObject(const Space& space) const
{
	for (auto pObj : m_pObject)
	{
		Space is = pObj->getSpace();
		bool collided = areSpacesCollided(is, space);
		if (collided)
			return pObj;
	}

	return nullptr;
}

bool Map::read()
{
	std::ifstream mapFileHandle(strFormat("%s%s.map", fileNS::MAP_DIR, m_loadedMap.c_str()));
	if (!mapFileHandle.is_open())
		return false;

	MapInfo mapInfo = FileIO::readMapInfo(mapFileHandle);
	m_width = mapInfo.width;
	m_height = mapInfo.height;
	m_usedBitmaps = mapInfo.bitmaps;
	m_bitmapAttribute = mapInfo.bitmapAttribute;
	for (size_t i = 0; i < m_bitmapAttribute.size(); i++)
		if (m_bitmapAttribute[i].obstructed)
			m_noSpaceBitmap.push_back(i);

	m_map.resize(m_height);
	for (auto& element : m_map)
		element.resize(m_width);

	std::string line;
	for (int32 i = m_height - 1; std::getline(mapFileHandle, line); i--)	// x0 and y0 is left down in map file, read first map cells in last array elements
	{
		for (int32 j = m_width - 1; j >= 0; j--)
		{
			char cell[2] = { 0 }; // 2 elements for avoid multi-byte mismatch happens when call std::atof
			cell[0] = line[j * 2];
			m_map[i][j] = static_cast<char>(std::atof(cell));
			bool exist = false;
			for (int8 k = 0; k < m_usedBitmaps; k++) // check if the map element exist if used map elements
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

bool Map::isNospaceUseless(const Space& s) const
{
	Space space[] = { getUpSpace(s), getDownSpace(s), getRightSpace(s), getLeftSpace(s) };
	for (int i = 0; i < mapNS::SPACE_VERTICES; i++)
	{
		if (space[i].isValid())
			if (isFreeSpace(space[i]))
				return false;
	}

	return true;
}

bool Map::isFreeSpace(const Space& space) const
{
	for (Space freeSpace : m_freeSpace)
		if (space.isSame(freeSpace))
			return true;

	return false;
}

Crc32 Map::getCrc32() const
{
	return FileIO::getCRC32(strFormat("%s%s.map", fileNS::MAP_DIR, m_loadedMap).c_str());
}

Space Map::getRandomEmptySpace() const
{
	Space emptySpace;
	do
		emptySpace = m_freeSpace[random(0, m_freeSpace.size())];
	while (getObject(emptySpace) != nullptr);

	return emptySpace;
}

void Map::addObject(Object* object)
{
	m_pObject.push_back(object);
}

void Map::removeObject(Object* pObject)
{
#ifdef _SERVER_BUILD
	if (m_pObject.size() == 0)
		debuggerBreak();
#endif
	auto pObj = std::find(m_pObject.begin(), m_pObject.end(), pObject);
	if (pObj != m_pObject.end())
		m_pObject.erase(pObj);
}

float Map::getVelocityFactor(V3 vertex) const
{
	vertex.x = floor(vertex.x / m_tiledSize.x);
	vertex.y = floor(vertex.y / m_tiledSize.y);
	int8 n = m_map[vertex.y][vertex.x];
	return m_bitmapAttribute[n].velocityFactor;
}

Object * Map::findClosestObject(V3 point, std::vector<Object*> exceptObject) const
{
	Object* pClosestObj = nullptr;
	float dist = m_maxDistance;
	for (const auto& thisObj : m_pObject)
	{
		if (std::find(exceptObject.begin(), exceptObject.end(), thisObj) != exceptObject.end())
			continue;

		float thisObjDist = thisObj->getSpace().getCenter().distance(point);
		if (thisObjDist > m_maxDistance)
			debuggerBreak("Invalid object position!");

		if (min(dist, thisObjDist) == thisObjDist)
			pClosestObj = thisObj;
	}

	return pClosestObj;
}

inline float getSmallestFcost(std::vector<Space> s, Space& sm)
{
	
}

inline std::vector<Space> sortByFCost(const std::vector<V3>& vertexList, const std::vector<Space>& spaceList)
{
	std::vector<Space> spaceListSort = { spaceList[0] };
	for (int i = 1; i < spaceList.size(); i++)
		for (int j = 0; j < spaceListSort.size(); j++)
		{
			float fcost = spaceList[i].getFCostFromCenter(vertexList);
			if (spaceListSort[j].getFCostFromCenter(vertexList)  > fcost)
			{
				spaceListSort.insert(spaceListSort.begin() + j, spaceList[i]);
				break;
			}
			if (j == spaceListSort.size() - 1)
			{
				spaceListSort.push_back(spaceList[i]);
				break;
			}
		}


	// Test code check
	std::vector<float> f;
	for (auto va : spaceListSort)
		f.push_back(va.getFCostFromCenter(vertexList));

	return spaceListSort;

}

/*
bool isOpenNodeWithLowerFExists(Space* successor, std::vector<Space*>& openList) {
	for (auto openNode : openList) {
		if (openNode->position == successor->position && openNode->f < successor->f) {
			return true;
		}
	}
	return false;
}

bool isClosedNodeWithLowerFExists(Space* successor, std::vector<Space*>& closedList) {
	for (auto closedNode : closedList) {
		if (closedNode->position == successor->position && closedNode->f < successor->f) {
			return true;
		}
	}
	return false;
}
*/
std::vector<V3> Map::getVertexPathfind(const V3 & start, const V3 & end)
{
	std::vector<V3> pathfind;
	std::vector<Space> openList, closedList;
	Space startSpace = findSpaceByVertex(start);
	bool done = false;
	
	openList.push_back(startSpace);
	while (!openList.empty())
	{
		auto q = sortByFCost({ start, end }, openList)[0];
		//auto qIterator = std::find(openList.begin(), openList.end(), q);
		//if (qIterator != openList.end())
		//	openList.erase(qIterator);
		for (int i = 0; i < openList.size(); i++)
			if (openList[i].isSame(q))
			{
				openList.erase(std::next(openList.begin(), i));
				break;
			}
		
		auto successors = getAmbientFreeSpace(q, { start,end });
		for (auto& successor : successors)
		{
			successor.parent = &q;
			if(successor.getCenter() == end)
			{
				done = true;
				break;
			}
			else
			{
				bool skip = false;
				for (auto node : openList)
				{
					if (node.getFCostFromCenter({ start,end }) < successor.getFCostFromCenter({ start,end }))
					{
						skip = true;
						break;
					}
				}
				if (skip)
					continue;

				for (auto node : closedList)
				{
					if (node.getFCostFromCenter({ start,end }) < successor.getFCostFromCenter({ start,end }))
					{
						skip = true;
						break;
					}
				}
				if (skip)
					continue;

				openList.push_back(successor);
			}
		}

		closedList.push_back(q);
	}

	for (auto closed : closedList)
		pathfind.push_back(closed.getCenter());

	return pathfind;
	std::vector<float> vecAmbientFcost;
	std::vector<std::pair<Space, float>> vecpaAmbient;
	auto vecAmbient = getAmbientFreeSpace(startSpace, { start, end });
	for (auto ambSpace : vecAmbient)
	{

	}

	return pathfind;
}

Space Map::findSpaceByVertex(const V3& v3) const
{
	Space space;
	space.v1.x = std::floor(v3.x / 100) * 100,
		space.v1.y = std::floor(v3.y / 100) * 100;
	space.v2.x = space.v1.x + m_tiledSize.x,
		space.v2.y = space.v1.y;
	space.v3.x = space.v2.x,
		space.v3.y = space.v2.y + m_tiledSize.y;
	space.v4.x = space.v1.x,
		space.v4.y = space.v3.y;

#ifdef _DEBUG
	if (isCollided(space), { m_pObject })
		debuggerBreak("Invalid V3 location or invalid m_freeSpace list!");
#endif

	return space;
}

Space Map::getRightFreeSpace(const Space & space) const
{
	Space rSpace(space);
	rSpace.addX(m_tiledSize.x);
	if (isCollided(rSpace))
		rSpace.v1.x = mapNS::UNDEFINED_POSITION;

	return rSpace;
}

Space Map::getLeftFreeSpace(const Space & space) const
{
	Space lSpace(space);
	lSpace.addX(-m_tiledSize.x);
	if (isCollided(lSpace))
		lSpace.v1.x = mapNS::UNDEFINED_POSITION;

	return lSpace;
}

Space Map::getTopFreeSpace(const Space & space) const
{
	Space tSpace(space);
	tSpace.addY(m_tiledSize.y);
	if (isCollided(tSpace))
		tSpace.v1.x = mapNS::UNDEFINED_POSITION;

	return tSpace;
}

Space Map::getDownFreeSpace(const Space & space) const
{
	Space dSpace(space);
	dSpace.addY(-m_tiledSize.y);
	if (isCollided(dSpace))
		dSpace.v1.x = mapNS::UNDEFINED_POSITION;

	return dSpace;
}

std::vector<Space> Map::getAmbientFreeSpace(const Space& space, const std::vector<V3>& vertexList) const
{
	Space ambient[] = {
		getRightFreeSpace(space),
		getLeftFreeSpace(space),
		getTopFreeSpace(space),
		getDownFreeSpace(space) };

	std::vector<Space> vecAmbient;
	for (int i = 0; i < 4; i++)
		if (ambient[i].isValid())
		{
			if (vecAmbient.empty())
				vecAmbient.push_back(ambient[i]);
			else
				if (!vertexList.empty())
				{
					for (int j = 0; j < vecAmbient.size(); j++)
					{
						float fcost = ambient[i].getFCostFromCenter(vertexList);
						if (vecAmbient[j].getFCostFromCenter(vertexList)  > fcost)
						{
							vecAmbient.insert(vecAmbient.begin() + j, ambient[i]);
							break;
						}
						if (j == vecAmbient.size() - 1)
						{
							vecAmbient.push_back(ambient[i]);
							break;
						}
					}
				}
		}

	// Test code check
	std::vector<float> f;
	for (auto va : vecAmbient)
		f.push_back(va.getFCostFromCenter(vertexList));
	
	if (vecAmbient.size() == 0)
		debuggerBreak();

	return vecAmbient;
}