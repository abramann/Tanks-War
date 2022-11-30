#include "unit.h"

Unit::Unit()
{
	audio = new Audio;
	health = 0;
	speed = 0;
	sinA = 0; cosA = 0;
	updateInput = false;
}

Unit::~Unit()
{
	release();
}

void Unit::initialize(int _width, int _height, int extraWidth, int extraHeight, int _currentFrame, int _startFrame, int _endFrame, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics)
{
	Image::initialize(_width, _height, extraWidth, extraHeight, false, _currentFrame, _startFrame, _endFrame, NULL, _textureManger, _graphics);
	audio->initialize();
	setHealth(_health);
	setSpeed(_speed);
}

void Unit::inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	key = new Key[UNIT_KEYS];
	updateInput = true;
	input = _input;
	key[KEYFORWARD] = forward_key;
	key[KEYBACK] = back_key;
	key[KEYRIGHT] = right_key;
	key[KEYLEFT] = left_key;
}

void Unit::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff)
{
	effect = new Effect[UNIT_EFFECTS];
	effect[EFFECTFORWARD] = forward_eff;
	effect[EFFECTBACK] = back_eff;
	effect[EFFECTRIGHT] = right_eff;
	effect[EFFECTLEFT] = left_eff;
	effect[EFFECTDEATH] = death_eff;
}

void Unit::update(float frameTime)
{
	audio->run();
	if (health <= 0)
	{
		if (updateInput)
			executeDeath();
		return;
	}

	if (updateInput)
		inputUpdate(frameTime);

	mathUpdate();
	Image::update(frameTime);
}

void Unit::inputUpdate(float frameTime)
{
	if (input->isKeyIn(key[KEYFORWARD]))
		executeForward(frameTime);
	if (input->isKeyIn(key[KEYBACK]))
		executeBack(frameTime);
	if (input->isKeyIn(key[KEYRIGHT]))
		executeRight(frameTime);
	if (input->isKeyIn(key[KEYLEFT]))
		executeLeft(frameTime);
}

void Unit::executeForward(float frameTime)
{
	yAdd(-(speed * frameTime));
	audio->playCue(effect[EFFECTFORWARD]);
}

void Unit::executeBack(float frameTime)
{
	yAdd(speed * frameTime);
	audio->playCue(effect[EFFECTBACK]);
}

void Unit::executeRight(float frameTime)
{
	xAdd(speed * frameTime);
	audio->playCue(effect[EFFECTRIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	xAdd(-(speed * frameTime));
	audio->playCue(effect[EFFECTLEFT]);
}

void Unit::executeDeath()
{
	updateInput = false;
	setFrameRect(frame[FRAMEDEATH], 0);
	audio->playCue(effect[EFFECTDEATH]);
}

void Unit::release()
{
	SAFE_DELETE(audio);
	SAFE_DELETE_ARRAY(key);
	SAFE_DELETE_ARRAY(effect);
}

void Unit::mathUpdate()
{
	sinA = sin(getAngle());
	if (sinA > 1)
		sinA = 1;
	else if (sinA < -1)
		sinA = -1;
	else if (abs(sinA) < 0.0001)
		sinA = 0;
	cosA = cos(getAngle());
	if (cosA > 1)
		cosA = 1;
	else if (cosA < -1)
		cosA = -1;
	else if (abs(cosA) < 0.0001)
		cosA = 0;
}

