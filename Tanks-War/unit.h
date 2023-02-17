#pragma once
#ifndef _UNIT_H
#define _UNIT_H
#include "object.h"
#include "map.h"

class Unit : public Object
{
public:

	Unit();
	~Unit();
	virtual void initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, TextureManger* death, Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);

protected:
	Map* m_pMap;

};


#endif
