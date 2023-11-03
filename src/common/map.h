// map.h
// Author: abramann

#pragma once

#include "types.h"
#include "node.h"
#include <vector>
#include <wrl\client.h>
#include <set>
#include <deque>

class Game;
class Graphics;
class TextureManger;
class Object;
class Image;
class Texture;
class Dx11Wrapper;

const std::vector<const Object*> EXCEPT_ALL_OBJECTS = { nullptr };

class Map
{
	template <typename T>
	class CustomSet : public std::set<T>
	{
	public:

		T operator[](const size_type& pos) const
		{
			std::set<T>::iterator it = begin();
			std::advance(it, pos);
			return *it;
		}
	};

public:

	Map();
	~Map();
	void initialize(const Game* pGame);
	bool load(const std::string& map);
	void draw() const;
	void clear();
	V2 getMapSize() const { return V2(m_width*m_tiledSize.x, m_height*m_tiledSize.y); }
	float passX(const Object* object, float x) const;
	float passY(const Object* object, float y) const;
	bool isCollided(const Image* image) const;
	bool isCollided(const Object* object) const; // specific function for Object type for objects collision
	bool isCollided(const Space& space, const std::vector<const Object*>& pObject = {}) const;
	Space getCollidedSpace(const Space& targetSpace, const std::vector<const Object*>& pExceptObject = {}) const;
	bool isOutOfRange(const Space& space) const;
	bool isOutOfRange(const V3& v) const;
	Object* getObject(const Space& space) const;
	bool isMapExist(const std::string name, Crc32 crc32) const { return true; }
	Crc32 getCrc32() const;
	Space getRandomEmptySpace(const Image* pImage) const;
	void addObject(Object* pObject);
	void removeObject(Object* pObject);
	float getVelocityFactor(Space space) const { return getVelocityFactor(space.getCenter()); }
	float getVelocityFactor(V3 vertex) const;
	Object* findClosestObject(V3 point, std::vector<Object*> exceptObject) const;
	std::vector<V3> pathfind(const V3& start, const V3& end);
	Space findSpaceByVertex(const V3& v3) const;
	Space getRightFreeSpace(const Space& space) const;
	Space getLeftFreeSpace(const Space& space) const;
	Space getTopFreeSpace(const Space& space) const;
	Space getDownFreeSpace(const Space& space) const;
	std::set<Node> getAmbientFreeSpace(const Node& node, const std::vector<V3>& sortByDistance = {}) const;
	bool isVectorUnderFreespace(const Vector3D& vector3d, const std::initializer_list<const Object*>& pExceptObject = {}) const;
	//static std::vector<Space> sortByFCost(const std::vector<V3>& vertexList, const Map::CustomSet<Space>& spaceList);
	bool isValidObject(const Object* const pObject);
	V3 getCollidedV3(const Object* pObject) const;
	void unload();

private:

	bool read();
	void clearUnnecessaryNospace();
	Space getRightSpace(const Space& space) const;
	Space getLeftSpace(const Space& space) const;
	Space getTopSpace(const Space& space) const;
	Space getDownSpace(const Space& space) const;
	bool isNospaceUseless(const Space& space) const;
	bool isFreeSpace(const Space& space) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[textureNS::TEXTURE_TILEDS];
	Graphics* m_pGraphics;
	Dx11Wrapper* m_pDx11Wrapper;
	Microsoft::WRL::ComPtr<DxBuffer> m_pVertexBuf, m_pIndexBuf, m_pNoSpaceBuf, m_pResultBuf, m_pResultStagingBuf, m_pSpaceBuf,
		m_pNoSpaceCountBuf, m_pMapRangeBuf;
	Microsoft::WRL::ComPtr<DxShaderResourceView> m_pNoSpaceSRV, m_pSpaceSRV, m_pNoSpaceCountSRV, m_pMapRangeSRV;
	Microsoft::WRL::ComPtr<DxUnorderedAccessView> m_pResultUAV;
	Microsoft::WRL::ComPtr<DxComputeShader> m_pCollisionCS;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	float m_maxDistance;
	std::vector<std::vector<char>> m_map;
	std::vector<int8> m_noSpaceBitmap;
	std::vector<Space> m_freeSpace, m_noSpace;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObject;
	std::vector<BitmapAttribute> m_bitmapAttribute;
	V2 m_tiledSize;
	std::string m_loadedMap;
	uint32 m_threadGroups;
};