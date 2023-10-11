#pragma once

using namespace std;

typedef float f;

class V3
{
public:

	f x, y, z;
	V3()  //constructor
	{
		x = 0;
		y = 0;
		z = 0;
	}

	V3(f x1, f y1, f z1 = 0)     //initializing object with values.
	{
		x = x1;
		y = y1;
		z = z1;
	}
	V3(const V3 &vec);    //copy constructor
	V3 operator+(const V3 &vec) const;    //addition
	V3 &operator+=(const V3 &vec);  ////assigning new result to the vector
	V3 operator-(const V3 &vec) const;    //substraction
	V3 &operator-=(const V3 &vec);  //assigning new result to the vector
	V3 operator*(f value) const;    //multiplication
	V3 &operator*=(f value);  //assigning new result to the vector.
	V3 operator/(f value) const;    //division
	V3 &operator/=(f value);  //assigning new result to the vector
	V3 &operator=(const V3 &vec);
	bool operator==(const V3 &v3) const;
	bool operator<(const V3& v3) const;
	f dot_product(const V3 &vec) const; //scalar dot_product
	V3 cross_product(const V3 &vec) const;    //cross_product
	f magnitude() const;  //magnitude of the vector
	V3 normalization();   //nor,malized vector
	f square() const; //gives square of the vector
	f distance(const V3 &vec) const;    //gives distance between two vectors
	void clear() { x = 0, y = 0, z = 0; }
};