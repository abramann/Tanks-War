#ifndef _MAP_H
#define _MAP_H

#include "constants.h"
#include "object.h"

class Map
{

public:

	Map();
	~Map();
	virtual void initialize(Texture* texture, Graphics* graphics);
	virtual bool load(const char* name);
	virtual bool read(const char* map);
	virtual void draw();
	virtual const char* setRandomMap();
	virtual bool isMapExist(const char* name, Crc32 crc) const;
	virtual bool isSpacesCollided(const Space& space1, const Space& space2) const;
	virtual bool isSpaceEmpty(const Space space) const;

	virtual Object* isCollided(const Image& object) const;
	virtual float passX(float x, float x0, float y, uint16_t width) const;
	virtual float passY(float x, float y, float y0, uint16_t height) const;
	virtual void addObject(Object* object) { m_pObjects.push_back(object); }
	virtual bool outOfRange(const Image& object) const;

	std::vector<std::string> getMapList();
	const char* getName() const { return m_name; }
	Space getEmptySpace() const;
	std::vector<Space> getEmptySpaces(uint16_t spaces) const;
	void getFullPath(char* path) const;

//	bool outOfPlayerRange() const;

protected:

	Graphics* m_pGraphics;
	Texture* m_pTexture;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	MapData m_mapData;
	BitmapData m_bitmapData;
	std::vector< std::vector<char>> m_ppMap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObjects;
	char m_name[MAX_NAME_LEN];
	LPVertexBuffer m_lpVertexBuffer;
};




#endif

