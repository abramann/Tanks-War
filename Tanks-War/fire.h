#ifndef _FIRE_H
#define _FIRE_H
#include "always.h"
#include "object.h"
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
	virtual void initialize(FireData fireData, TextureManger* explostion, Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release(Object* object, uint8_t releaseType);
	virtual void setExplosionMode();
	virtual void endFrame();
	virtual void setDefaultMode();
	const bool active() const { return m_active; }

private:

	virtual void releaseTraceUpdate(float frameTime);
	virtual void releaseNormalUpdate(float frameTime);

	Map* m_pMap;
	Object* m_pTarget;
	TextureManger* m_pFire,* m_pExplostion;
	bool m_release;
	bool m_bomb;
	uint8_t m_releaseType;
	FireData m_fireData;
	ObjectData m_objectData;
	bool m_active;
};


#endif

 