#pragma once
#ifndef _UNIT_H
#define _UNIT_H
#include "map.h"

class Unit : public Object
{
public:

	Unit();
	~Unit();
	virtual void initialize(uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate, float updateDelay, float health, float speed, Map* map, Texture* death, Texture* texture, Graphics* graphics);
	virtual void initialize(Map * map, TextureManger* textureManger, Texture * texture, Graphics * graphics);
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);

protected:

	Map* m_pMap;

};


#endif
