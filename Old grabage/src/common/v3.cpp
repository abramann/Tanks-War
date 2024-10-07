#include "v3.h"
#include <math.h>
#include <assert.h>

V3::V3(const V3 &v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
}

//addition

V3 V3 ::operator+(const V3 &v3) const
{
	return V3(x + v3.x, y + v3.y, z + v3.z);
}

V3 &V3 ::operator+=(const V3 &v3)
{
	x += v3.x;
	y += v3.y;
	z += v3.z;
	return *this;
}
//substraction//
V3 V3 ::operator-(const V3 &v3) const
{
	return V3(x - v3.x, y - v3.y, z - v3.z);
}

V3 &V3::operator-=(const V3 &v3)
{
	x -= v3.x;
	y -= v3.y;
	z -= v3.z;
	return *this;
}

//scalar multiplication

V3 V3 ::operator*(f value) const
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
V3 V3 ::operator/(f value) const
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

V3 &V3::operator=(const V3 &v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	return *this;
}

//Dot product
f V3::dot_product(const V3 &v3) const
{
	return x*v3.x + v3.y*y + v3.z*z;
}

bool V3::operator==(const V3 &v3) const
{
	return (v3.x == x&&v3.y == y&&v3.z == z);
}

bool V3::operator<(const V3& v3) const
{
	return (v3.x > x&&v3.y > y, v3.z > z);
}

//cross product
V3 V3::cross_product(const V3 &v3) const
{
	f ni = y*v3.z - z*v3.y;
	f nj = z*v3.x - x*v3.z;
	f nk = x*v3.y - y*v3.x;
	return V3(ni, nj, nk);
}

f V3::magnitude() const
{
	return static_cast<f>(sqrt(square()));
}

f V3::square() const
{
	return x*x + y*y + z*z;
}

V3 V3::normalization()
{
	assert(magnitude() != 0);
	*this /= magnitude();
	return *this;
}

f V3::distance(const V3 &v3) const
{
	V3 dist = *this - v3;
	return dist.magnitude();
}