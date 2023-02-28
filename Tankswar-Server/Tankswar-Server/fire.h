#ifndef _FIRE_H
#define _FIRE_H
#include "always.h"
#include "object.h"
#include "map.h"
#include "texture.h"

class Fire : public Image
{
public:

	Fire();
	~Fire();
	virtual void initialize(Object* object, Map* map, Texture* explosion, Texture* texture, Graphics* graphics);
	virtual void initialize(Object* object, Map* map, TextureManger* textureManger, Texture* texture, Graphics* graphics);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release(uint8_t releaseType);
	virtual void setExplosionMode();
	virtual void endFrame();
	virtual void setDefaultMode();
	const bool active() const { return m_active; }

private:

	virtual void releaseTraceUpdate(float frameTime);
	virtual void releaseNormalUpdate(float frameTime);

	Map* m_pMap;
	Object* m_pObject, * m_pTarget;
	Texture* m_pFire,* m_pExplostion;
	bool m_release;
	bool m_bomb;
	uint8_t m_releaseType;
	FireInfo m_fireInfo;
	ObjectData m_objectData;
	bool m_active;
};


#endif

 