#ifndef _FIRE_H
#define _FIRE_H
#include "always.h"
#include "unit.h"
#include "map.h"

struct FireData
{
	int16_t speed;
	uint16_t healthDecrease;
	std::string effect;
};

struct ObjectData
{
	float x, y;
	uint16_t width, height;
	float angle;
};

enum
{
	RELEASE_NORMAL,
	RELEASE_TRACE,	
};

class Fire : public Image
{
public:

	Fire();
	~Fire();
	virtual void initialize(FireData fireData, Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update(float frameTime);
	virtual void draw();
	Fire& release(const Unit& object, uint8_t releaseType);

private:

	virtual void releaseTraceUpdate(float frameTime);
	virtual void releaseNormalUpdate(float frameTime);

	bool m_release;
	uint8_t m_releaseType;
	Unit m_target;
	FireData m_fireData;
	ObjectData m_objectData;


};


#endif

 