
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
	V3 operator+(const V3 &vec);    //addition
	V3 &operator+=(const V3 &vec);  ////assigning new result to the vector
	V3 operator-(const V3 &vec);    //substraction
	V3 &operator-=(const V3 &vec);  //assigning new result to the vector
	V3 operator*(f value);    //multiplication
	V3 &operator*=(f value);  //assigning new result to the vector.
	V3 operator/(f value);    //division
	V3 &operator/=(f value);  //assigning new result to the vector
	V3 &operator=(const V3 &vec);
	bool operator==(const V3 &vec);
	f dot_product(const V3 &vec); //scalar dot_product
	V3 cross_product(const V3 &vec);    //cross_product
	f magnitude();  //magnitude of the vector
	V3 normalization();   //nor,malized vector
	f square(); //gives square of the vector

	f distance(const V3 &vec);    //gives distance between two vectors
	f show_X(); //return x
	f show_Y(); //return y
	f show_Z(); //return z
};