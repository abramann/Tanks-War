// line.cpp
// Author: abramann

#include "line.h"
#include "gamemath.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::initialize(const Game * pGame)
{
	Image::initialize("red-line", pGame);
}

void Line::updateLocalCoordinate()
{
	Image::updateLocalCoordinate();
}

void Line::update()
{
}

void Line::draw(const Vector3D& vector)
{
	setPosition(vector.begin);
	m_rotate.z = -1 * (gameMathNS::getAngle(vector) + PI);
	m_width = vector.size;
	m_height = vector.getMagnitude();
	updateLocalCoordinate();
	Image::draw();
}

V3 Line::getRotateCenter() const
{
	return V3(0, 0, 0);
}