// map.h
// Author: abramann

#pragma once

#include "Cell.h"
#include "types.h"
#include <vector>
#include <set>
#include <deque>
#include <memory>

class CModel;
class ITexture;
class IBuffer;

class CMap
{
	friend class RENDER_CLASS;
public:
	CMap();
	~CMap();
	void initialize();
	bool load(const std::string& map);
	void reset();
	Vertex2D getMapDimensions() const { return Vertex2D(m_width, m_height); }
	Vertex2D getMapSizeInPixels() const { return Vertex2D(m_width * mapNS::TILED_DIMENSION, m_height * mapNS::TILED_DIMENSION); }
	bool isMapLoaded() const;
	/*float passX(const Object* object, float x) const;
	float passY(const Object* object, float y) const;
	bool isCollided(const Image* image) const;
	bool isCollided(const Object* object) const; // specific function for Object type for objects collision
	bool isCollided(const Cell& Cell, const std::vector<const Object*>& pObject = {}) const;
	Cell getCollidedCell(const Cell& targetCell, const std::vector<const Object*>& pExceptObject = {}) const;
	*/
	bool isOutOfRange(const Cell& Cell) const;
	bool isOutOfRange(const Vertex& v) const;
	/*Object* getObject(const Cell& Cell) const;
	
	//Cell getRandomEmptyCell(const Image* pImage) const;
	//void addObject(IObject* pObject);
	//void removeObject(IObject* pObject);
		//Object* findClosestObject(V3 point, std::vector<Object*> exceptObject) const;
	//std::vector<V3> pathfind(const V3& start, const V3& end);
		//std::set<Node> getAmbientFreeCell(const Node& node, const std::vector<V3>& sortByDistance = {}) const;
	//bool isVectorUnderFreecell(const Vector3D& vector3d, const std::initializer_list<const Object*>& pExceptObject = {}) const;
	//static std::vector<Cell> sortByFCost(const std::vector<V3>& vertexList, const Map::CustomSet<Cell>& cellList);
	//bool isValidObject(const Object* const pObject);
	//V3 getCollidedV3(const Object* pObject) const;
		//void clearObjects() { m_pObject.clear(); }
		*/
	float getVelocityFactor(Cell Cell) const { return getVelocityFactor(Cell.getCenter()); }
	float getVelocityFactor(Vertex vertex) const;
	Cell findCellByVertex(const Vertex& v) const;
	Cell getRightFreeCell(const Cell& Cell) const;
	Cell getLeftFreeCell(const Cell& Cell) const;
	Cell getTopFreeCell(const Cell& Cell) const;
	Cell getDownFreeCell(const Cell& Cell) const;
	void unload();

private:

	void readInitializeSettings();
	bool isBlockedCellID(int id);
	bool read();
	void clearUnnecessaryBlockedCells();
	bool isBlockedCellUseless(const Cell& cell) const;
	Cell getRightCell(const Cell& Cell) const;
	Cell getLeftCell(const Cell& Cell) const;
	Cell getTopCell(const Cell& Cell) const;
	Cell getDownCell(const Cell& Cell) const;
	bool isNocellUseless(const Cell& Cell) const;
	bool isFreeCell(const Cell& Cell) const;

	struct CellFactors
	{
		int id;
		float damage;
		float velocity;
	};

	IBuffer* m_pVB;
	IBuffer* m_pIB;
	std::vector<Cell> m_blockedCells;
	std::vector<Cell> m_freeCells;
	int m_numTileds;
	std::vector<ITexture*> m_pTextures;
	std::vector<int> m_blockedCellsID;
	std::vector<CellFactors> m_factors;
	int m_width, m_height;
	float m_maxDistance;
	std::vector<std::vector<char>> m_map;
	//std::vector<int> m_numNoCellBitmap;
	//std::vector<Cell> m_freeCell, m_noCell;
	std::vector<uint> m_startVertex, m_lenVertex;
	std::vector<CModel*> m_pObjects;
	std::string m_loadedMap;
};

extern std::shared_ptr<CMap> g_pMap;