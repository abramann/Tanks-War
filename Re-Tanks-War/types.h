#pragma once

#include <Windows.h>




typedef HWND HWindow;
typedef unsigned int uint;
typedef unsigned char uchar;

struct Color
{
	Color(uchar _r = 255, uchar _g = 255, uchar _b = 255, uchar alpha = 255) : r(_r), g(_g), b(_b), a(alpha) {}
	uchar r, g, b, a;

	uint getAsInteger() const
	{
		return (r << 24) + (g << 16) + (b << 8) + (a);
	}
};
