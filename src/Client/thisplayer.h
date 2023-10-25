// thisplayer.h
// Author: abramann

#pragma once
#include "player.h"


class Input;
class Camera;

class ThisPlayer : public Player
{
public:
	
	ThisPlayer();
	~ThisPlayer();
	virtual void initialize(const Game * pGame);
	virtual void update();
	virtual void executeDie();
	virtual void executeAnimateRepeat();

protected:


	void handleInput();

	Camera* m_pCamera;
	Input* m_pInput;
	Key m_forward, m_back, m_right, m_left, m_attack;
	bool m_handleInput;
};

