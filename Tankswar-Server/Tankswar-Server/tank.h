#ifndef _TANK_H
#define _TANK_H
#include "unit.h"
#include "fire.h"

class Tank : public Unit
{

public:

	Tank();
	~Tank();
	virtual void initialize(int width, int height, float health, float speed, Map* map, Texture* death, Texture* texture, Graphics* graphics);
	virtual void initialize(Map* map, TextureManger* textureManger, Texture* texture, Graphics* graphics);
	virtual void inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, Texture* fireTex);
	virtual void inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key);
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
	virtual void setTexture(Texture* texture, Texture* fire);
	virtual void endFrame();
	PlayerState getPlayerState() const;
	
private:

	Texture* m_pFire;
	
	Fire m_fire;
	TankInfo m_tankInfo;
	FireInfo* m_fireInfo;

};


#endif