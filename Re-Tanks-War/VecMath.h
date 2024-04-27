#pragma once

#include <cmath>




struct Vector2D
{
	float x;
	float y;

	Vector2D() : x(0), y(0) {}
	Vector2D(float _x, float _y) : x(_x), y(_y) {}

	Vector2D operator+(const Vector2D &vec) const
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D &operator+=(const Vector2D &vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2D operator-(const Vector2D &vec) const    //substraction
	{
		return Vector2D(x - vec.x, y - vec.y);
	}

	Vector2D &operator-=(const Vector2D &vec)  //assigning new result to the vector
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2D operator*(float value) const    //multiplication
	{
		return Vector2D(x * value, y * value);
	}

	Vector2D &operator*=(float value)  //assigning new result to the vector.
	{
		x *= value;
		y *= value;
		return *this;
	}

	Vector2D operator/(float value) const    //division
	{
		return Vector2D(x / value, y / value);
	}

	Vector2D &operator/=(float value)  //assigning new result to the vector
	{
		x /= value;
		y /= value;
		return *this;
	}

	Vector2D &operator=(const Vector2D &vec)
	{
		x = vec.x;
		y = vec.y;
	}

	bool operator==(const Vector2D &vec) const
	{
		return (x == vec.x && y == vec.y);
	}

	float magnitude() const
	{
		return sqrtf(pow(x, 2) + pow(y, 2));
	}

	Vector2D normalization() const
	{
		float mag = magnitude();
		Vector2D normalize;
		normalize.x = x / mag;
		normalize.y = y / mag;
		return normalize;
	}

	Vector2D perpendicular() const
	{
		return Vector2D(y, -x);
	}

	void clear()
	{
		x = y = 0;
	}
};

struct Vertex
{
	float x;
	float y;

	Vertex() : x(0), y(0) {}
	Vertex(float _x, float _y) : x(_x), y(_y) {}

	Vertex operator+(const Vertex &vert) const
	{
		return Vertex(x + vert.x, y + vert.y);
	}

	Vertex operator+(const Vector2D &vec) const
	{
		return Vertex(x + vec.x, y + vec.y);
	}

	Vertex &operator+=(const Vertex &vert)
	{
		x += vert.x;
		y += vert.y;
		return *this;
	}

	Vertex operator-(const Vertex &vert) const    //substraction
	{
		return Vertex(x - vert.x, y - vert.y);
	}

	Vertex operator*(const Vertex &vert) const    //substraction
	{
		return Vertex(x * vert.x, y * vert.y);
	}

	Vertex operator-(const Vector2D &vec) const    //substraction
	{
		return Vertex(x - vec.x, y - vec.y);
	}

	Vertex &operator-=(const Vertex &vert)  //assigning new result to the vector
	{
		x -= vert.x;
		y -= vert.y;
		return *this;
	}

	Vertex operator*(float value) const    //multiplication
	{
		return Vertex(x * value, y * value);
	}

	Vertex &operator*=(float value)  //assigning new result to the vector.
	{
		x *= value;
		y *= value;
		return *this;
	}

	Vertex operator/(float value) const    //division
	{
		return Vertex(x / value, y / value);
	}

	Vertex &operator/=(float value)  //assigning new result to the vector
	{
		x /= value;
		y /= value;
		return *this;
	}

	Vertex &operator=(const Vertex &vert)
	{
		x = vert.x;
		y = vert.y;
		return *this;
	}

	bool operator==(const Vertex &vert) const
	{
		return (x == vert.x && y == vert.y);
	}

	float distance(const Vertex& vert) const
	{
		Vertex delta = *this - vert;
		return sqrtf(pow(delta.x, 2) + pow(delta.y, 2));
	}
};

inline Vector2D makeVector2D(Vertex vert1, Vertex vert2)
{
	return Vector2D(vert2.x - vert1.x, vert2.y - vert1.y);
}

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

extern void makeUnitSquare(Vertex& vert);