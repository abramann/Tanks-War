#ifndef _TANK_H
#define _TANK_H
#include "unit.h"
#include "fire.h"

class Tank : public Unit
{

public:

	Tank();
	~Tank();
	virtual void initialize(
		int width, int height, float health, float speed, Map* map, 
		Texture* death, Texture* texture, Audio* audio, Graphics* graphics);
	virtual void initialize(Map* map, TextureManger* textureManger, Texture* texture, Audio* audio, Graphics* graphics);
	virtual void inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, 
		Key attack_key);
	virtual void audioInitialize(Sound forward, Sound back, Sound right, Sound left, Sound death,
		Sound attack, Sound attacked);
	virtual void inputUpdate(float frameTime);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release();
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeAttack(float frameTime);
	virtual void executeAttacked(float damage);
	virtual void setTexture(Texture* texture, Texture* fire);
	virtual void endFrame();
	void setClientMode() { m_client = true; };
	void setRemoteClientMode() { m_remoteClient = true; };
	void applyPlayerUpdate(const PlayerUpdate playerUpdate);
	PlayerUpdate getPlayerUpdate() const;
	PlayerID id;
	PlayerAct m_playerAct;
	LPVertexBuffer m_lpVertexBuffer;
private:

	Texture* m_pFire;
	bool m_client;
	bool m_remoteClient;
	Fire m_fire;
	TankInfo m_tankInfo;
	FireInfo* m_fireInfo;
};


#endif