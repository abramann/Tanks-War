// line.h
// Author: abramann

#pragma once

#include "image.h"
#include "types.h"

class Line : public Image
{
public:

	Line();
	~Line();
	virtual void initialize(const Game* pGame);
	virtual void updateLocalCoordinate();
	virtual void update();
	virtual void draw(const Vector3D& vector);
	virtual V3 getRotateCenter() const;

private:

	Vector3D m_vector;
};