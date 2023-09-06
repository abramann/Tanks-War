// thisclient.h
// Author: abramann

#pragma once
#include "..\common\player.h"

class Input;
class Camera;
class TanksWar;

class ThisClient : public Player
{
public:

	ThisClient();
	~ThisClient();
	virtual void initialize(PlayerID id, TanksWar * pTW);
	virtual void update();
	virtual void executeForward();
	virtual void executeBack();
	virtual void executeRight();
	virtual void executeLeft();
	virtual bool executeAttack();
	virtual void executeAnimateRepeat();
	virtual void implementAttack();
	virtual void executeDie();
	PlayerAct getAct() const { return m_act; }

protected:

	void handleInput();

	Camera* m_pCamera;
	Input* m_pInput;
	PlayerAct m_act;
	Key m_forward, m_back, m_right, m_left, m_attack;
	bool m_handleInput;
	TanksWar* m_pTW;
};