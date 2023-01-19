#include "tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{
	release();
}

void Tank::initialize(int width, int height, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics)
{
	/*
	if (IsBadWritePtr(rect, TANK_RECTS))
		rect = new RECT[TANK_RECTS];

	for (int i = 0; i < TANK_RECTS; i++)
	{
		rect[i].right = width * (i + 1);
		rect[i].left = rect[i].right - width;
		rect[i].top = 0;
		rect[i].bottom = height;
	}
	*/
	//Unit::initialize(width, height, 1, 1, false, 0, _health, _speed, _textureManger, _graphics);
}

void Tank::inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, TextureManger* _fireTex)
{
	fireTex = _fireTex;
	fire.initialize(fireTex, graphics);
	if (IsBadWritePtr(key, TANK_KEYS))
		key = new Key[TANK_KEYS];

	key[TANK_KEYATTACK] = attack_key;
	Unit::inputInitialize(_input, forward_key, back_key, right_key, left_key);
}

void Tank::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff, Effect shot_eff, Effect hit_eff)
{
	if (IsBadWritePtr(effect, TANK_EFFECTS))
		effect = new Effect[TANK_EFFECTS];

	effect[TANK_EFFECTSHOT] = shot_eff;
	effect[TANK_EFFECTHIT] = hit_eff;
	Unit::audioInitialize(forward_eff, back_eff, right_eff, left_eff, death_eff);
}

void Tank::inputUpdate(float frameTime)
{
	Unit::inputUpdate(frameTime);
	if (input->isKeyIn(key[TANK_EFFECTSHOT]))
	{
		executeAttack(frameTime);
	}
}

void Tank::release()
{
	SAFE_DELETE(rect);
}

void Tank::executeForward(float frameTime)
{
	spriteData.x += cosA * speed * frameTime;
	spriteData.y += sinA * speed * frameTime;
}

void Tank::executeBack(float frameTime)
{
	spriteData.x -= frameTime * cosA * speed;
	spriteData.y -= frameTime * sinA * speed;
}

void Tank::executeRight(float frameTime)
{
	spriteData.angle += frameTime * PI / 128;
}

void Tank::executeLeft(float frameTime)
{
	spriteData.angle -= frameTime * PI / 128;
}

void Tank::executeAttack(float frameTime)
{
	//setFrameRect(frame[TANK_FRAMEATTACK], TANK_ATTACKDELAY);
	audio->playCue(effect[TANK_EFFECTSHOT]);
}

void Tank::setTexture(TextureManger* _textureManger, TextureManger* _fireTex)
{
	Unit::setTexture(_textureManger);
	fireTex = _fireTex;
}
