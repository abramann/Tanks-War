#pragma once

#include "Math.h"
#include "GameData.h"


#ifdef _WIN32
#include <Windows.h>

#define IN_RANGE(n, a, b) (bool) ( max(a, b) > n && n > min(a, b) )
#define IN_RANGE_OR_EQUAL(n, a, b) (bool) ( max(a, b) >= n && n >= min(a, b) )
#else
#define IN_RANGE(n, a, b) (bool) ( std::max(a, b) > n && n > std::min(a, b) )
#define IN_RANGE_OR_EQUAL(n, a, b) (bool) ( std::max(a, b) >= n && n >= std::min(a, b) )
#endif


struct Cell
{
	Vertex v1, v2, v3, v4;

	Cell() { clear(); }
	Cell(const Cell& cell) { *this = cell; }
	Cell(const Vertex& _v1, const Vertex& _v2, const Vertex& _v3, const Vertex& _v4) : v1(_v1), v2(_v2), v3(_v3), v4(_v4) {}

	void clear() { v1 = v2 = v3 = v4 = Vertex(0, 0, 0); }
	bool isSame(const Cell& s) const { return ((s.v1 == v1 && s.v2 == v2) || (s.v1 == v2 && s.v2 == v1)) && ((s.v3 == v3 && s.v4 == v4) || (s.v3 == v4 && s.v4 == v3)) ? true : false; }
	bool isValid() const { return (v1.x != mapNS::UNDEFINED_POSITION); }
	bool isIncluded(const Vertex& v3) { return (IN_RANGE_OR_EQUAL(v3.x, getMinX(), getMaxX()) && IN_RANGE_OR_EQUAL(v3.y, getMinY(), getMaxY())); }
	void setInvalid() { v1.x = mapNS::UNDEFINED_POSITION; }
	//bool operator<(const Cell& s) const { return (v1 < s.v1&& v2 < s.v2 && v3 < s.v3 && v4 < s.v4); } // for using with std::set
	float getMaxX() const { return gameMathNS::getMax<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMinX() const { return gameMathNS::getMin<float>({ v1.x,v2.x,v3.x,v4.x }); }
	float getMaxY() const { return gameMathNS::getMax<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getMinY() const { return gameMathNS::getMin<float>({ v1.y, v2.y, v3.y, v4.y }); }
	float getWidth() const { return getMaxX() - getMinX(); }
	float getHeight() const { return getMaxY() - getMinY(); }
	float getSize() const { return getWidth() * getHeight(); }
	Vertex getCenter() const { return Vertex(getMinX() + getWidth() / 2, getMinY() + getHeight() / 2, 0); }
	float getDistanceFromCenter(const Vertex& v3) const { return getCenter().distance(v3); }
	
	float getFCostForCenter(std::vector<Vertex> v3) const
	{
		float fcost = 0;
		for (const auto& v : v3)
			fcost += getDistanceFromCenter(v);

		return fcost;
	}

	void addX(float val) { gameMathNS::add4(val, v1.x, v2.x, v3.x, v4.x); }

	void addY(float val) { gameMathNS::add4(val, v1.y, v2.y, v3.y, v4.y); }

	Cell getRightCell() const
	{
		return Cell(
			Vertex(getMaxX(), getMinY(), 0),
			Vertex(getMaxX() + getWidth(), getMinY(), 0),
			Vertex(getMaxX() + getWidth(), getMaxY(), 0),
			Vertex(getMaxX(), getMaxY(), 0));
	}

	Cell getLeftCell() const
	{
		return Cell(
			Vertex(getMinX() - getWidth(), getMinY(), 0),
			Vertex(getMinX(), getMinY(), 0),
			Vertex(getMinX(), getMaxY(), 0),
			Vertex(getMinX() - getWidth(), getMaxY(), 0));
	}

	Cell getUpperCell() const
	{
		return Cell(
			Vertex(getMinX(), getMaxY(), 0),
			Vertex(getMaxX(), getMaxY(), 0),
			Vertex(getMaxX(), getMaxY() + getHeight(), 0),
			Vertex(getMinX(), getMaxY() + getHeight(), 0));
	}

	Cell getDownCell() const
	{
		return Cell(
			Vertex(getMinX(), getMinY() - getHeight(), 0),
			Vertex(getMaxX(), getMinY() - getHeight(), 0),
			Vertex(getMaxX(), getMinY(), 0),
			Vertex(getMinX(), getMinY(), 0));
	}
};