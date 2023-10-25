// thisclient.h
// Author: abramann

#pragma once
#include "thisplayer.h"

class Input;
class Camera;
class TanksWar;

class ThisClient : public ThisPlayer
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
	virtual void implementAttack();
	PlayerAct getAct() const { return m_act; }

protected:

	PlayerAct m_act;
	TanksWar* m_pTW;
};