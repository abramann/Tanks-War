#ifndef _FIRE_H
#define _FIRE_H
#include "always.h"
#include "unit.h"
#include "map.h"

class Fire : public Unit
{
public:

	Fire();
	~Fire();
	virtual void initialize(Map* map, TextureManger* _textureManger, Graphics* _graphics);
	Fire& setRelease(bool release) { m_release = release; return *this; }

private:

	bool m_release;

};


#endif