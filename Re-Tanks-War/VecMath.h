#pragma once

#include <cmath>



struct Vertex2D
{
	Vertex2D(float _x, float _y) : x(_x), y(_y) {}
	Vertex2D() : x(0), y(0) {}
	float x, y;
};

struct Vertex
{
	float x, y, z;
	float u, v;
	Vertex() : x(0), y(0), z(0), u(0), v(0) {}
	Vertex(float _x, float _y) : x(_x), y(_y), z(0), u(0), v(0) {}
	Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z), u(0), v(0) {}
	Vertex(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {}

	Vertex operator+(const Vertex &vert) const
	{
		return Vertex(x + vert.x, y + vert.y, z + vert.z);
	}

	Vertex &operator+=(const Vertex &vert)
	{
		x += vert.x;
		y += vert.y;
		z += vert.z;
		return *this;
	}

	Vertex operator-(const Vertex &vert) const
	{
		return Vertex(x - vert.x, y - vert.y, z - vert.z);
	}

	Vertex operator*(const Vertex& vert) const
	{
		return Vertex(x * vert.x, y * vert.y, z * vert.z);
	}

	Vertex &operator-=(const Vertex &vert)
	{
		x -= vert.x;
		y -= vert.y;
		z -= vert.z;
		return *this;
	}

	Vertex operator*(float value) const
	{
		return Vertex(x * value, y * value, z * value);
	}

	Vertex &operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vertex operator/(float value) const
	{
		return Vertex(x / value, y / value, z / value);
	}

	Vertex &operator/=(float value)  //assigning new result to the vector
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	Vertex &operator=(const Vertex &vert)
	{
		x = vert.x;
		y = vert.y;
		z = vert.z;
		return *this;
	}

	bool operator==(const Vertex &vert) const
	{
		return (x == vert.x && y == vert.y && z == vert.z);
	}

	float distance(const Vertex& vert) const
	{
		Vertex delta = *this - vert;
		return sqrtf(pow(delta.x, static_cast<float>(2))
			+ pow(delta.y, static_cast<float>(2))
			+ pow(delta.z, static_cast<float>(2)));
	}
};

struct Rect
{
	Vertex vert1;
	Vertex vert2;
	Vertex vert3;
	Vertex vert4;
};

struct Rect8
{
	Vertex vert1;
	Vertex vert2;
	Vertex vert3;
	Vertex vert4;
	Vertex vert5;
	Vertex vert6;
	Vertex vert7;
	Vertex vert8;
};
