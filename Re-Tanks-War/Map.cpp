// map.cpp
// Author: abramann

#include "Map.h"
#include "GameData.h"
#include "File.hpp"
#include "Renderer.h"
#include "String.h"

using namespace evt;
using namespace mapNS;

//#pragma warning(disable : 4267) // conversion from size_t to uint

std::shared_ptr<CMap> g_pMap;

CMap::CMap() : m_pVB(nullptr), m_pIB(nullptr)
{
}

CMap::~CMap()
{
	for (auto& t : m_pTextures)
		g_pRenderer->releaseTexture(t);

	g_pRenderer->releaseBuffer(m_pVB);
	g_pRenderer->releaseBuffer(m_pIB);
}

void CMap::initialize()
{
	readInitializeSettings();
	for (int i = 0; i < m_numTileds; i++)
	{
		std::wstring wName = to_wstring(strFormat("tiled-%d.png", i + 1));
		m_pTextures[i] = g_pRenderer->loadTextureFromFile(wName.c_str());
	}
}

void CMap::readInitializeSettings()
{
	auto hFile = FileIO::plainText(fileNS::MAP_INITIALIZE);
	m_numTileds = hFile.readValueAsInteger("NumTileds");
	m_pTextures.resize(m_numTileds);
	m_factors.resize(m_numTileds);
	m_blockedCellsID = hFile.readValuesAsNumeric("BlockedCells");
	for (int i = 0; i < m_numTileds; i++)
	{
		int id = i + 1;
		if (isBlockedCellID(id))
			continue;

		m_factors[i].id = id;
		m_factors[i].damage = hFile.readValueAsFloat(strFormat("Factor(%d).Damage", id).c_str());
		m_factors[i].velocity = hFile.readValueAsFloat(strFormat("Factor(%d).Velocity", id).c_str());
	}
}

bool CMap::isBlockedCellID(int id)
{
	for (auto blockedID : m_blockedCellsID)
		if (blockedID == id)
			return true;
	return false;
}

bool CMap::read()
{
	std::string mappath = strFormat("%s%s.map", fileNS::MAP_DIR, m_loadedMap.c_str());
	if (!BaseFileIO::exists(mappath))
		return false;

	auto hMapFile = FileIO::plainText(mappath);
	std::string line = hMapFile.getline();
	m_width = strGetRepeatedNum(line, ',') + 1;

	hMapFile.readFromBeginning();

	line = hMapFile.completeRead();

	m_height = strGetRepeatedNum(line, '\n') + 1;

	m_map.resize(m_height);
	for (auto& element : m_map)
	{
		element.resize(m_width);
	}

	hMapFile.readFromBeginning();
	
	for (int i = m_height - 1; i >= 0; line = hMapFile.getline(), i--)	// x0 and y0 is left down in map file, read first map cells in last array elements
	{
		for (int j = m_width - 1; j >= 0; j--)
		{
			char cell[2] = { 0 }; // 2 elements for avoid multi-byte mismatch happens when call std::atof
			cell[0] = line[j * 2];
			m_map[i][j] = static_cast<char>(std::atof(cell));
			bool exist = false;
			for (int k = 0; k < m_numTileds + 1; k++) // check if the map element exist if used map elements
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

struct CellMesh
{
	CellMesh() {};
	Vertex v1, v2, v3, v4;

	bool isValid() const { return (v1.x != mapNS::UNDEFINED_POSITION); }
	float getMaxX() const { return gameMathNS::getMax<float>({ v1.x, v2.x, v3.x, v4.x }); }
	float getMinX() const { return gameMathNS::getMin<float>({ v1.x, v2.x, v3.x, v4.x }); }
	float getMaxY() const { return gameMathNS::getMax<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getMinY() const { return gameMathNS::getMin<float>({ v1.y, v2.y, v3.y, v4.y }); }

	Cell getCell() const
	{
		Cell cell;
		cell.v1 = Vertex(getMinX(), getMinY());
		cell.v2 = Vertex(getMaxX(), getMinY());
		cell.v3 = Vertex(getMaxX(), getMaxY());
		cell.v4 = Vertex(getMinX(), getMaxY());
		return cell;
	}
};

bool CMap::load(const std::string& map)
{
	m_loadedMap = map;
	if (!read())
		return false;

	int numCells = m_width * m_height;
	m_maxDistance = sqrt(pow(m_width * TILED_DIMENSION, 2) + pow(m_height * TILED_DIMENSION, 2));
	std::vector<std::vector< std::vector<CellMesh>>> mesh(m_numTileds);
	for (auto& element : mesh)
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

			mesh[m_map[h][w]][h][w].v1 = { static_cast<float>((w)*TILED_DIMENSION), static_cast<float>((h)*TILED_DIMENSION),0.0f,0.0f,0.0f };
			mesh[m_map[h][w]][h][w].v1.u = 0, mesh[m_map[h][w]][h][w].v1.v = 1;

			mesh[m_map[h][w]][h][w].v2 = { static_cast<float>((w + 1) * TILED_DIMENSION), static_cast<float>((h)*TILED_DIMENSION),0.0f,-1.0f,0.0f };
			mesh[m_map[h][w]][h][w].v2.u = 1, mesh[m_map[h][w]][h][w].v2.v = 1;

			mesh[m_map[h][w]][h][w].v3 = {static_cast<float>((w + 1) * TILED_DIMENSION), static_cast<float>((h + 1) * TILED_DIMENSION),0.0f,-1.0f,-1.0f };
			mesh[m_map[h][w]][h][w].v3.u = 1, mesh[m_map[h][w]][h][w].v3.v = 0;

			mesh[m_map[h][w]][h][w].v4 = { static_cast<float>((w)*TILED_DIMENSION), static_cast<float>((h + 1) * TILED_DIMENSION),0.0f,0.0f,-1.0f };
			mesh[m_map[h][w]][h][w].v4.u = 0, mesh[m_map[h][w]][h][w].v4.v = 0;

			Cell theCell = mesh[m_map[h][w]][h][w].getCell();
			bool blockedCell = isBlockedCellID(m_map[h][w]);
			if(blockedCell)
			{
				blockedCell = true;
				m_blockedCells.push_back(theCell);
				continue;
			}

			m_freeCells.push_back(theCell);
		}
	}

	clearUnnecessaryBlockedCells();
	
	// organize mesh in a 2d vector waith sort each bitmap as series
	std::vector<Vertex> vertexData;

	struct TextureIndex
	{
		uint i[6];	// each 2 triangles requires 6 index
	};

	std::vector<TextureIndex> index(numCells, { 0 });
	for (auto n = 0, i = 0; i < m_numTileds; i++)
	{
		m_startVertex.push_back(vertexData.size());
		for (auto j = 0; j < m_height; j++)
		{
			for (auto k = 0; k < m_width; k++)
			{
				if (!mesh[i][j][k].isValid())
					continue;	// mesh are allocated by another bitmap type

				vertexData.push_back(mesh[i][j][k].v1);
				vertexData.push_back(mesh[i][j][k].v2);
				vertexData.push_back(mesh[i][j][k].v3);
				vertexData.push_back(mesh[i][j][k].v4);

				// as mesh in vertex buffer organized, no need to organize index
				if (n != numCells)
				{
					index[n].i[0] = 2 + n * 4, index[n].i[1] = 1 + n * 4, index[n].i[2] = 0 + n * 4;
					index[n].i[3] = 2 + n * 4, index[n].i[4] = 0 + n * 4, index[n].i[5] = 3 + n * 4;
					n++;
				}
			}
		}

		m_lenVertex.push_back(vertexData.size() - m_startVertex[i]);
	}
	
	m_pVB = g_pRenderer->createVertexBuffer(numCells * 4, &vertexData[0], IRenderer::BUFFER_IMMUTABLE);
	m_pIB = g_pRenderer->createIndexBuffer(numCells * sizeof(uint) * 6, (uint*) & index[0], IRenderer::BUFFER_IMMUTABLE);
	//if (m_noCell.size() == 0)
	//	m_noCell.push_back(Cell());
	/*
	m_pNoCellBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(Cell), m_noCell.size(), &m_noCell[0], 0);
	m_pNoCellSRV = m_pDx11Wrapper->createBufferSRV(m_pNoCellBuf.Get());
	uint32 noCellCount = m_noCell.size();
	m_pNoCellCountBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(uint32), 1, (void*)&noCellCount, 0);
	m_pNoCellCountSRV = m_pDx11Wrapper->createBufferSRV(m_pNoCellCountBuf.Get());
	int32 mapRange[] = { static_cast<int32>(TILED_DIMENSION*m_width), static_cast<int32>(TILED_DIMENSION*m_height) };
	m_pMapRangeBuf = m_pDx11Wrapper->createStructuredBuffer(sizeof(int32), 2, mapRange, 0);
	m_pMapRangeSRV = m_pDx11Wrapper->createBufferSRV(m_pMapRangeBuf.Get());
	m_threadGroups = (m_noCell.size() / dxNS::THREADS_PER_GROUP);
	float f = ((m_noCell.size()*1.0f) / (dxNS::THREADS_PER_GROUP*1.0f)) - m_threadGroups;
	if (f > 0.001f)
		m_threadGroups++;
		*/
	return true;
}

Cell CMap::getRightCell(const Cell& cell) const
{
	Cell rCell = cell;
	rCell.addX(TILED_DIMENSION);
	if (isOutOfRange(rCell))
		rCell.v1.x = mapNS::UNDEFINED_POSITION;

	return rCell;
}

Cell CMap::getLeftCell(const Cell& cell) const
{
	Cell lCell = cell;
	lCell.addX(-TILED_DIMENSION);
	if (isOutOfRange(lCell))
		lCell.v1.x = mapNS::UNDEFINED_POSITION;

	return lCell;
}

Cell CMap::getTopCell(const Cell& cell) const
{
	Cell tCell = cell;
	tCell.addY(TILED_DIMENSION);
	if (isOutOfRange(tCell))
		tCell.v1.x = mapNS::UNDEFINED_POSITION;

	return tCell;
}

Cell CMap::getDownCell(const Cell& cell) const
{
	Cell dCell = cell;
	dCell.addY(-TILED_DIMENSION);
	if (isOutOfRange(dCell))
		dCell.v1.x = mapNS::UNDEFINED_POSITION;

	return dCell;
}

bool CMap::isBlockedCellUseless(const Cell& cell) const
{
	Cell ambient[] = { getTopCell(cell), getDownCell(cell), getRightCell(cell), getLeftCell(cell) };
	for (int i = 0; i < mapNS::CELL_VERTICES_NUM; i++)
		if (ambient[i].isValid())
			if (isFreeCell(ambient[i]))
				return false;

	return true;
}

void CMap::clearUnnecessaryBlockedCells()
{
	for (size_t i = 0; i < m_blockedCells.size(); i++)
		if (isBlockedCellUseless(m_blockedCells[i]))
		{
			m_blockedCells.erase(std::next(m_blockedCells.begin(), i));
			i--;
		}

	// For X axis
	for (size_t i = 0; i < m_blockedCells.size(); i++)
	{
		Cell& ns = m_blockedCells[i];
		for (size_t j = 0; j < m_blockedCells.size(); j++)
		{
			if (i == j)
				continue;

			Cell& ns2 = m_blockedCells[j];
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

					m_blockedCells.erase(std::next(m_blockedCells.begin(), j));
					i = -1;
					break;
				}
			}
		}
	}

	// For Y axis
	for (size_t i = 0; i < m_blockedCells.size(); i++)
	{
		Cell& ns = m_blockedCells[i];
		for (size_t j = 0; j < m_blockedCells.size(); j++)
		{
			if (i == j)
				continue;

			Cell& ns2 = m_blockedCells[j];
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
					m_blockedCells.erase(std::next(m_blockedCells.begin(), j));
					i = -1;
					break;
				}
			}
		}
	}
}


void CMap::reset()
{
	//m_freeCell.clear();
	//m_noCell.clear();
	m_lenVertex.clear();
	//m_pObject.clear();
}

bool CMap::isMapLoaded() const
{
	if (m_loadedMap.empty())
		return false;
	return true;
}

/*
float Map::passX(const Object * pThisObject, float x) const
{
	Cell is = pThisObject->getCell(x, 0);
	float x0 = pThisObject->getPosition().x;
	if (isCollided(is, { pThisObject }))
		return x0;

	return x;
}

float Map::passY(const Object * pThisObject, float y) const
{
	Cell is = pThisObject->getCell(0, y);
	float y0 = pThisObject->getPosition().y;
	if (isCollided(is, { pThisObject }))
	{
		return y0;
	}

	return y;
}

bool Map::isCollided(const Image* pImage) const
{
	return isCollided(pImage->getCell());
}

bool Map::isCollided(const Object * pObject) const
{
	return isCollided(pObject->getCell(), { pObject });
}

bool Map::isCollided(const Cell& targetCell, const std::vector<const Object*>& pExceptObject) const
{
	m_pGraphics->drawBox(targetCell);
	Cell colCell = getCollidedCell(targetCell, pExceptObject);
	return colCell.isValid();
}

Cell Map::getCollidedCell(const Cell & targetCell, const std::vector<const Object*>& pExceptObject) const
{
	Cell collidedCell;
	collidedCell.setInvalid();
	if (g_pGameSettings->computeShader)
	{
		m_pDx11Wrapper->copyToResource(m_pCellBuf.Get(), (void*)&targetCell, sizeof(Cell));
		DxShaderResourceView* ppSRV[] = { m_pNoCellSRV.Get(),m_pNoCellCountSRV.Get(), m_pCellSRV.Get(), m_pMapRangeSRV.Get() };
		DxUnorderedAccessView* ppUAV[] = { m_pResultUAV.Get() };
		m_pDx11Wrapper->runComputeShader(m_pCollisionCS.Get(), ARRAYSIZE(ppSRV), ppSRV, ARRAYSIZE(ppUAV), ppUAV,
			m_threadGroups, 1, 1);
		m_pDx11Wrapper->copyResourceToResource(m_pResultStagingBuf.Get(), m_pResultBuf.Get());
		m_pDx11Wrapper->copyResource(&collidedCell, m_pResultStagingBuf.Get(), 4);
		if (collidedCell.isValid())
			return collidedCell;
	}
	else
	{
		if (isOutOfRange(targetCell))
			return Cell();

		for (auto noCell : m_noCell)
			if (areCellsCollided(noCell, targetCell))
				return noCell;
	}

	//if (pExceptObject[0] != nullptr)
	if (!pExceptObject.empty() && pExceptObject != EXCEPT_ALL_OBJECTS)
		for (const auto& pObj : m_pObject)
		{
			if (std::find(pExceptObject.begin(), pExceptObject.end(), pObj) != pExceptObject.end())
				continue;

			Cell objCell = pObj->getCell();
			if (areCellsCollided(objCell, targetCell))
				return objCell;
		}

	return collidedCell;
}
*/
bool CMap::isOutOfRange(const Cell& cell) const
{
	const auto x = m_width * TILED_DIMENSION,
		y = m_height * TILED_DIMENSION;
	if (IN_RANGE_OR_EQUAL(cell.v1.x, 0, x) &&
		IN_RANGE_OR_EQUAL(cell.v2.x, 0, x) &&
		IN_RANGE_OR_EQUAL(cell.v3.x, 0, x) &&
		IN_RANGE_OR_EQUAL(cell.v4.x, 0, x)
		)
		if (IN_RANGE_OR_EQUAL(cell.v1.y, 0, y) &&
			IN_RANGE_OR_EQUAL(cell.v2.y, 0, y) &&
			IN_RANGE_OR_EQUAL(cell.v3.y, 0, y) &&
			IN_RANGE_OR_EQUAL(cell.v4.y, 0, y)
			)
			return false;

	return true;
}

bool CMap::isOutOfRange(const Vertex & v) const
{
	const auto x = m_width * TILED_DIMENSION,
		y = m_height * TILED_DIMENSION;
	if (IN_RANGE_OR_EQUAL(v.x, 0, x) && IN_RANGE_OR_EQUAL(v.y, 0, y))
		return false;

	return true;
}
/*
Object * Map::getObject(const Cell& cell) const
{
	for (auto pObj : m_pObject)
	{
		Cell is = pObj->getCell();
		bool collided = areCellsCollided(is, cell);
		if (collided)
			return pObj;
	}

	return nullptr;
}
*/

/*
Cell Map::getRightCell(const Cell& cell) const
{
	Cell rCell = cell;
	rCell.addX(m_tiledSize.x);
	if (isOutOfRange(rCell))
		rCell.v1.x = mapNS::UNDEFINED_POSITION;

	return rCell;
}

Cell Map::getLeftCell(const Cell& cell) const
{
	Cell lCell = cell;
	lCell.addX(-m_tiledSize.x);
	if (isOutOfRange(lCell))
		lCell.v1.x = mapNS::UNDEFINED_POSITION;

	return lCell;
}

Cell Map::getTopCell(const Cell& cell) const
{
	Cell tCell = cell;
	tCell.addY(m_tiledSize.y);
	if (isOutOfRange(tCell))
		tCell.v1.x = mapNS::UNDEFINED_POSITION;

	return tCell;
}

Cell Map::getDownCell(const Cell& cell) const
{
	Cell dCell = cell;
	dCell.addY(-m_tiledSize.y);
	if (isOutOfRange(dCell))
		dCell.v1.x = mapNS::UNDEFINED_POSITION;

	return dCell;
}

bool Map::isNocellUseless(const Cell& cell) const
{
	Cell ambient[] = { getTopCell(cell), getDownCell(cell), getRightCell(cell), getLeftCell(cell) };
	for (int i = 0; i < mapNS::CELL_VERTICES_NUM; i++)
		if (ambient[i].isValid())
			if (isFreeCell(ambient[i]))
				return false;

	return true;
}
*/
bool CMap::isFreeCell(const Cell& cell) const
{
	for (const Cell& freeCell : m_freeCells)
		if (cell.isSame(freeCell))
			return true;

	return false;
}
/*
Crc32 Map::getCrc32() const
{
	return FileIO::getCRC32(strFormat("%s%s.map", fileNS::MAP_DIR, m_loadedMap).c_str());
}

Cell Map::getRandomEmptyCell(const Image* pImage) const
{
	Cell emptyCell, imCell;
	do
	{
		emptyCell = m_freeCell[random(0, m_freeCell.size())];
		emptyCell.v2.x = pImage->getWidth() + emptyCell.v1.x;
		emptyCell.v3.x = emptyCell.v2.x;
		emptyCell.v3.y = emptyCell.v1.y + pImage->getHeight();
		emptyCell.v4.x = emptyCell.v1.x;
		emptyCell.v4.y = emptyCell.v3.y;
	} while (getObject(emptyCell) != nullptr && !isCollided(emptyCell));

	return emptyCell;
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
	uint8 n = m_map[static_cast<uint32>(vertex.y)][static_cast<uint32>(vertex.x)];
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

		float thisObjDist = thisObj->getCell().getCenter().distance(point);
		debuggerBreak(thisObjDist > m_maxDistance, "Invalid object position!");

		if (min(dist, thisObjDist) == thisObjDist)
			pClosestObj = thisObj;
	}

	return pClosestObj;
}

std::vector<V3> Map::pathfind(const V3 & start, const V3 & end)
{
	debuggerBreak(isOutOfRange(start) || isOutOfRange(end), "Invalid start / end location!");
	CustomSet<Node> openList;
	CustomSet<std::shared_ptr<Node>> pClosedList;
	Cell startCell = findCellByVertex(start);
	bool done = false;
	openList.insert(Node(startCell, { start, end }));
	Node endSuccessor;
	std::shared_ptr<Node> pQ;
	while (!openList.empty())
	{
		pQ = std::make_shared<Node>(Node(openList[0], { start,end }));
		Node& q = *pQ;
		openList.erase(openList[0]);
		std::shared_ptr<std::set<Node>> pSuccessors(new std::set<Node>(getAmbientFreeCell(q, { start,end })));
		std::set<Node>& successors = *pSuccessors;
		for (auto& successor : successors)
		{
			if (successor.getCenter() == end)
			{
				endSuccessor = successor;
				done = true;
				break;
			}
			else
			{
				bool add = true;
				for (auto pClosed : pClosedList)
					if (successor.isSame(*pClosed))
						add = false;

				if (add)
					openList.insert(successor);
			}
		}

		pClosedList.insert(pQ);
		if (done)
			break;
	}

	std::vector<V3> pathfind;
	if (done)
		while (1)
		{
			if (endSuccessor.getCenter() == start)
				break;

			pathfind.push_back(endSuccessor.getCenter());
			if (endSuccessor.parent == nullptr)
				break;

			endSuccessor = *endSuccessor.parent;
		}

	return pathfind;
}

Cell Map::findCellByVertex(const V3& v3) const
{
	Cell cell;
	cell.v1.x = std::floor(v3.x / 100) * 100,
		cell.v1.y = std::floor(v3.y / 100) * 100;
	cell.v2.x = cell.v1.x + m_tiledSize.x,
		cell.v2.y = cell.v1.y;
	cell.v3.x = cell.v2.x,
		cell.v3.y = cell.v2.y + m_tiledSize.y;
	cell.v4.x = cell.v1.x,
		cell.v4.y = cell.v3.y;

#ifdef _DEBUG
	std::vector< const Object*> cObj;
	for (auto obj : m_pObject)
		cObj.push_back(obj);

	debuggerBreak(isCollided(cell, cObj), "Invalid V3 location or invalid m_freeCell list!");
#endif

	return cell;
}

Cell Map::getRightFreeCell(const Cell & cell) const
{
	Cell rCell(cell);
	rCell.addX(m_tiledSize.x);
	if (isCollided(rCell, EXCEPT_ALL_OBJECTS))
		rCell.v1.x = mapNS::UNDEFINED_POSITION;

	return rCell;
}

Cell Map::getLeftFreeCell(const Cell & cell) const
{
	Cell lCell(getLeftCell(cell));
	if (isCollided(lCell, EXCEPT_ALL_OBJECTS))
		lCell.v1.x = mapNS::UNDEFINED_POSITION;

	return lCell;
}

Cell Map::getTopFreeCell(const Cell & cell) const
{
	Cell tCell(getTopCell(cell));
	if (isCollided(tCell, EXCEPT_ALL_OBJECTS))
		tCell.v1.x = mapNS::UNDEFINED_POSITION;

	return tCell;
}

Cell Map::getDownFreeCell(const Cell & cell) const
{
	Cell dCell(getDownCell(cell));
	if (isCollided(dCell, EXCEPT_ALL_OBJECTS))
		dCell.v1.x = mapNS::UNDEFINED_POSITION;

	return dCell;
}

std::set<Node> Map::getAmbientFreeCell(const Node& node, const std::vector<V3>& vertexList) const
{
	Node ambient[] = {
		Node(getRightFreeCell(node), vertexList),
		Node(getLeftFreeCell(node), vertexList),
		Node(getTopFreeCell(node), vertexList),
		Node(getDownFreeCell(node), vertexList) };

	std::set<Node> setAmbient;

	for (int i = 0; i < 4; i++)
		if (ambient[i].isValid())
		{
			ambient[i].parent = &node;
			setAmbient.insert(ambient[i]);
		}

	return setAmbient;
}

bool Map::isVectorUnderFreecell(const Vector3D& vector, const std::initializer_list<const Object*>& pExceptObject) const
{
	V3 v1, v2, v3, v4;
	V3 center(0, 0, 0);
	v1 = V3(vector.begin.x, vector.begin.y);
	float rotate = gameMathNS::getAngle(vector);
	v2.x = vector.begin.x + vector.size / 2;
	v2.y = vector.begin.y + vector.size / 2;
	if (v1.distance(center) > v2.distance(center))
		swap(v1, v2);

	v3 = V3(vector.end.x - (vector.size / 2), vector.end.y - (vector.size / 2), 0);
	v4 = V3(vector.end.x + vector.size / 2, vector.end.y + vector.size / 2, 0);
	if (v3.distance(center) < v4.distance(center))
		swap(v3, v4);

	auto zz = (isCollided(Cell(v1, v2, v3, v4), pExceptObject));
	zz = !zz;
	return zz;
}

bool Map::isValidObject(const Object * const pObject)
{
	if (!pObject)
		return false;

	return (std::find(m_pObject.begin(), m_pObject.end(), pObject) != m_pObject.end());
}

V3 Map::getCollidedV3(const Object * pObject) const
{
	return V3();
}

void Map::unload()
{
	m_width = m_height = m_usedBitmaps = 0;
	m_maxDistance = 0;
	m_map.clear();
	m_noCellBitmap.clear();
	m_freeCell.clear();
	m_noCell.clear();
	m_startVertex.clear();
	m_lenVertex.clear();;
	m_bitmapAttribute.clear();
	m_loadedMap.clear();
}
*/