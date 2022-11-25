#ifndef _TANK_H
#define _TANK_H
#include "unit.h"

constexpr auto TANK_CURRENTFRAME = 1;
constexpr auto TANK_STARTFRAME = 1;
constexpr auto TANK_ENDFRAME = 3;

class Tank : public Unit
{
public:

	Tank();
	~Tank();
	virtual void initialize(int _width, int _height, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics);

private:

};


#endif