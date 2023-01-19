#include "unit.h"

Unit::Unit()
{
	audio = new Audio;
	health = 0;
	speed = 0;
	sinA = 0; cosA = 0;
	key = 0;
	effect = 0;
	updateInput = false;
	playAudio = false;
}

Unit::~Unit()
{
	release();
}

void Unit::initialize(int width, int height, int columns, int rows, bool _animate, float _updateDelay, float _health, float _speed, Image* _death, TextureManger* _textureManger, Graphics* _graphics)
{
	health = _health;
	speed = _speed;
	death = _death;
	audio->initialize();
	Image::initialize(width, height, columns, rows, false, _updateDelay, _textureManger, _graphics);
	if (IsBadReadPtr(_death,sizeof(Image)))
		return;
	deathColumns = death->getColumn();
	deathRows = death->getRow();
}

void Unit::inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	input = _input;
	updateInput = true;
	if (IsBadReadPtr(key, UNIT_KEYS))
		key = new Key[UNIT_KEYS];

	key[KEYFORWARD] = forward_key;
	key[KEYBACK] = back_key;
	key[KEYRIGHT] = right_key;
	key[KEYLEFT] = left_key;
}

void Unit::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff)
{
	playAudio = true;
	if (IsBadWritePtr(effect, UNIT_EFFECTS))
		effect = new Effect[UNIT_EFFECTS];

	effect[EFFECTFORWARD] = forward_eff;
	effect[EFFECTBACK] = back_eff;
	effect[EFFECTRIGHT] = right_eff;
	effect[EFFECTLEFT] = left_eff;
	effect[EFFECTDEATH] = death_eff;
}

void Unit::update(float frameTime)
{
	
	if (health <= 0)
	{
		if (column == 1 || row == 1)
			executeDeath();
		else if (column == 8 && row == 8)
			return;
	}
	else if (updateInput)
			inputUpdate(frameTime);

	if (playAudio);
	audio->run();

	this->mathUpdate();
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
	if (playAudio)
		audio->playCue(effect[EFFECTFORWARD]);
}

void Unit::executeBack(float frameTime)
{
	yAdd(speed * frameTime);

	if (playAudio)
		audio->playCue(effect[EFFECTBACK]);
}

void Unit::executeRight(float frameTime)
{
	xAdd(speed * frameTime);
	
	if (playAudio)
		audio->playCue(effect[EFFECTRIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	xAdd(-(speed * frameTime));
	if (playAudio)
		audio->playCue(effect[EFFECTLEFT]);
}

void Unit::executeDeath()
{
	updateInput = false;
	Image::Image(*death);
	if (playAudio)
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

