#include "v3.h"
#include <math.h>
#include <assert.h>

V3::V3(const V3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

//addition

V3 V3 ::operator+(const V3 &vec)
{
	return V3(x + vec.x, y + vec.y, z + vec.z);
}

V3 &V3 ::operator+=(const V3 &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}
//substraction//
V3 V3 ::operator-(const V3 &vec)
{
	return V3(x - vec.x, y - vec.y, z - vec.z);
}

V3 &V3::operator-=(const V3 &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

//scalar multiplication

V3 V3 ::operator*(f value)
{
	return V3(x*value, y*value, z*value);
}

V3 &V3::operator*=(f value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

//scalar division
V3 V3 ::operator/(f value)
{
	assert(value != 0);
	return V3(x / value, y / value, z / value);
}

V3 &V3 ::operator/=(f value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

V3 &V3::operator=(const V3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

bool V3::operator==(const V3 &vec)
{
	return (vec.x == x && vec.y == y && vec.z == z);
}
//Dot product
f V3::dot_product(const V3 &vec)
{
	return x*vec.x + vec.y*y + vec.z*z;
}

//cross product
V3 V3::cross_product(const V3 &vec)
{
	f ni = y*vec.z - z*vec.y;
	f nj = z*vec.x - x*vec.z;
	f nk = x*vec.y - y*vec.x;
	return V3(ni, nj, nk);
}

f V3::magnitude()
{
	return static_cast<f>(sqrt(square()));
}

f V3::square()
{
	return x*x + y*y + z*z;
}

V3 V3::normalization()
{
	assert(magnitude() != 0);
	*this /= magnitude();
	return *this;
}

f V3::distance(const V3 &vec)
{
	V3 dist = *this - vec;
	return dist.magnitude();
}

f V3::show_X()
{
	return x;
}

f V3::show_Y()
{
	return y;
}

f V3::show_Z()
{
	return z;
}