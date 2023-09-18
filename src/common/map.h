// map.h
// Author: abramann

#pragma once

#include "types.h"
#include <vector>
#include <wrl\client.h>

class Game;
class Graphics;
class TextureManger;
class Object;
class Image;
class Texture;
class Dx11Wrapper;

class Map
{
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
	bool isCollided(const Space& space, const Object* object = 0) const;
	bool isOutOfRange(const Space& space) const;
	Object* getObject(const Space& space) const;
	bool isMapExist(const std::string name, Crc32 crc32) const { return true; }
	Crc32 getCrc32() const;
	Space getRandomEmptySpace() const;
	void addObject(Object* pObject);
	void removeObject(Object* pObject);
	float getVelocityFactor(Space space) const { return getVelocityFactor(space.getCenter()); }
	float getVelocityFactor(V3 vertex) const;

private:

	bool read();
	void clearUnnecessaryNospace();
	Space getRightSpace(Space s) const;
	Space getLeftSpace(Space s) const;
	Space getUpSpace(Space s) const;
	Space getDownSpace(Space s) const;
	bool isNospaceUseless(const Space& s) const;
	bool isFreeSpace(const Space& s) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[textureNS::TEXTURE_TILEDS];
	Graphics* m_pGraphics;
	Dx11Wrapper* m_pDx11Wrapper;
	Microsoft::WRL::ComPtr<DxBuffer> m_pVertexBuf, m_pNoSpaceBuf, m_pResultBuf, m_pResultStagingBuf, m_pSpaceBuf,
		m_pNoSpaceCountBuf, m_pMapRangeBuf;
	Microsoft::WRL::ComPtr<DxShaderResourceView> m_pNoSpaceSRV, m_pSpaceSRV, m_pNoSpaceCountSRV, m_pMapRangeSRV;
	Microsoft::WRL::ComPtr<DxUnorderedAccessView> m_pResultUAV;
	Microsoft::WRL::ComPtr<DxComputeShader> m_pCollisionCS;
	std::vector< std::vector<char>> m_map;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	std::vector<int8> m_noSpaceBitmap;
	std::vector<Space> m_freeSpace, m_noSpace;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObject;
	std::vector<BitmapAttribute> m_bitmapAttribute;
	V2 m_tiledSize;
	std::string m_loadedMap;
	uint32 m_threadGroups;
};