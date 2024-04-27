#pragma once

#include <stdint.h>



typedef uint32_t Color;

inline Color makeColor(int r, int g, int b, int alpha = 1)
{
	return (r << 24) + (g << 16) + (b << 8) +(alpha);
}