// map.h
// Author: abramann

#pragma once

#include "types.h"
#include <d3d11.h>
#include <vector>
#include <wrl\client.h>

class Game;
class Graphics;
class TextureManger;
class Object;
class Image;
class Texture;
class Bullet;
class Dx11Wrapper;

class Map
{
public:

	Map();
	~Map();
	void initialize(const Game* pGame);
	bool load(const char* map);
	void draw() const;
	void clear();
	V2 getMapSize() const { return V2(m_width*m_tiledSize.x, m_height*m_tiledSize.y); }
	float passX(const Object* object, float x) const;
	float passY(const Object* object, float y) const;
	bool isCollided(const Image* image) const;
	bool isCollided(const Object* object) const;
	bool isCollided(const Space space, const Object* object = 0) const;
	bool isOutOfRange(const Space space) const;
	Object* getObject(const Space space) const;
	//static Space getImageSpace(const Image* image, float x0 = 0, float y0 = 0);
	bool isMapExist(const char* name, Crc32 crc32) const { return true; }
	const char* getMap() const { return m_loadedMap; }
	Crc32 getCrc32() const;
	Space getRandomEmptySpace() const;
	void addObject(Object* pObject);
	void removeObject(Object* pObject);

private:

	bool read();
	bool areSpacesCollided(const Space space1, const Space space2) const;
	void clearUnnecessaryNospace();
	Space getRightSpace(Space s) const;
	Space getLeftSpace(Space s) const;
	Space getUpSpace(Space s) const;
	Space getDownSpace(Space s) const;
	bool isNospaceUseless(Space s) const;
	bool isFreeSpace(Space s) const;

	TextureManger* m_pTextureManger;
	Texture* m_pTexture[textureNS::TEXTURE_TILEDS];
	Microsoft::WRL::ComPtr<DxBuffer> m_pVertexBuf, m_pNoSpaceBuf, m_pResultBuf, m_pResultStagingBuf, m_pSpaceBuf,
		m_pNoSpaceCountBuf, m_pMapRangeBuf;
	Microsoft::WRL::ComPtr<DxShaderResourceView> m_pNoSpaceSRV, m_pSpaceSRV, m_pNoSpaceCountSRV, m_pMapRangeSRV;
	Microsoft::WRL::ComPtr<DxUnorderedAccessView> m_pResultUAV;
	Microsoft::WRL::ComPtr<DxComputeShader> m_pCollisionCS;
	Graphics* m_pGraphics;
	std::vector< std::vector<char>> m_map;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	std::vector<int8> m_preventedBitmap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObject;
	V2 m_tiledSize;
	char m_loadedMap[gameNS::MAX_NAME_LEN];
	Dx11Wrapper* m_pDx11Wrapper;
	uint32 m_threadGroups;
};