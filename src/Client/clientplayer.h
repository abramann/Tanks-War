// clientplayer.h
// Author: abramann

#pragma once
#include "..\common\player.h"

class Input;
class Camera;

class ClientPlayer : public Player
{
public:

	ClientPlayer();
	~ClientPlayer();
	virtual void initialize(PlayerID id, Game* game);
	virtual void update(int64 frameTime);
	virtual void executeForward();
	virtual void executeBack();
	virtual void executeRight();
	virtual void executeLeft();
	virtual void executeAttack();
	virtual void implementAttack();
	PlayerAct getAct() const { return m_act; }

protected:

	void handleInput();

	Camera* m_pCamera;
	Input* m_pInput;
	PlayerAct m_act;
	Key m_forward, m_back, m_right, m_left, m_attack;
	bool m_handleInput;
};