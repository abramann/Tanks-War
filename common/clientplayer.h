#pragma once
#include "player.h"

class Input;
class Camera;

class ClientPlayer : public Player
{
public:

	ClientPlayer();
	~ClientPlayer();
	virtual void initialize(PlayerID id, Game* game);
	virtual void update(float frameTime);
	virtual void executeForward();
	virtual void executeBack();
	virtual void executeRight();
	virtual void executeLeft();
	virtual void executeAttack();

protected:

	void handleInput();

	Camera* m_pCamera;
	Input* m_pInput;
	PlayerAct m_act;
	Key m_forward, m_back, m_right, m_left;
	bool m_handleInput;
	
};

