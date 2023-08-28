// map.h
// Author: abramann

#pragma once

#include "types.h"
#include <d3d11.h>
#include <vector>

class Game;
class Graphics;
class TextureManger;
class Object;
class Image;
class Texture;
class Bullet;
class Dx11Wrapper;

typedef ID3D11Buffer* LPDxBuffer;
typedef ID3D11ShaderResourceView* LPShaderResourceView;
typedef ID3D11UnorderedAccessView* LPUnorderedAccessView;
typedef ID3D11ComputeShader* LPComputeShader;

class Map
{
public:

	Map();
	~Map();
	void initialize(const Game* game);
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
	void addObject(Object* object);

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
	LPDxBuffer m_lpNoSpaceBuf, m_lpResultBuf, m_lpResultStagingBuf, m_lpSpaceBuf, m_lpNoSpaceCountBuf;
	LPShaderResourceView m_lpNoSpaceSRV, m_lpSpaceSRV, m_lpNoSpaceCountSRV;
	LPUnorderedAccessView m_lpResultUAV;
	LPComputeShader m_lpCollisionCS;
	Graphics* m_pGraphics;
	std::vector< std::vector<char>> m_map;
	int32 m_width, m_height;
	int8 m_usedBitmaps;
	LPVertexBuffer m_lpVertexBuffer;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	std::vector<int8> m_preventedBitmap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObject;
	V2 m_tiledSize;
	char m_loadedMap[gameNS::MAX_NAME_LEN];
	Dx11Wrapper* m_pDx11Wrapper;
};