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
	virtual bool load(const char* path);
	virtual bool read(const char* map);
	std::vector<std::string> getMapList();
	bool isMapExist(const char* name, const char* crc);
	virtual void draw();
	void Add_Object(Object* object) { m_pObjects.push_back(object); }
	Space getFreeSpace() const;
	bool spacesCollided(const Space& space1, const Space& space2) const;
	virtual float passX(float x, float x0, float y, uint16_t width) const;
	virtual float passY(float x, float y, float y0, uint16_t height) const;
	Object* collided(const Image& object) const;
	bool outOfRange(const Image& object) const;
//	bool outOfPlayerRange() const;

protected:

	bool emptyFreeSpace(const Space space) const;
	Graphics* m_pGraphics;
	Texture* m_pTexture;
	std::vector<Space> m_freeSpace;
	std::vector<Space> m_noSpace;
	MapData m_mapData;
	BitmapData m_bitmapData;
	char **m_ppMap;
	std::vector<uint32_t> m_startVertex, m_lenVertex;
	std::vector<Object*> m_pObjects;

};




#endif

