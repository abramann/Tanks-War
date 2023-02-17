#ifndef _TANK_H
#define _TANK_H
#include "unit.h"
#include "fire.h"

#define TANK_KEYS			5
#define TANK_KEYS			5
#define TANK_KEYATTACK		4
#define TANK_EFFECTS		7
#define TANK_EFFECTSHOT		5
#define TANK_EFFECTHIT		6

const auto DEFAULT_FIRE_SPEED = 20;
const auto DEFAULT_FIRE_HEALTHDECREASE = 40;

class Tank : public Unit
{
public:

	Tank();
	~Tank();
	virtual void initialize(int width, int height, float health, float speed, Map* map, TextureManger* death, TextureManger* textureManger, Graphics* graphics);
	virtual void inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, TextureManger* fireTex);
	virtual void audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff, Effect shot_eff, Effect hit_eff);
	virtual void inputUpdate(float frameTime);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release();
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeAttack(float frameTime);
	virtual void setTexture(TextureManger* textureManger, TextureManger* fire);
	virtual void endFrame();

private:

	TextureManger* m_pFire;
	Fire m_fire;
	FireData m_fireData;
};


#endif