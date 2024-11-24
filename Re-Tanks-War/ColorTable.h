#pragma once

#include "Types.h"
#include "ImGui/imgui.h"

struct Color
{
	Color(uchar _r = 255, uchar _g = 255, uchar _b = 255, uchar alpha = 255) : r(_r), g(_g), b(_b), a(alpha) {}
	uchar r, g, b, a;

	uint getAsInteger() const
	{
		return (r << 24) + (g << 16) + (b << 8) + (a);
	}
};

namespace colorTable
{
	const Color BLUE = Color(0, 0, 255);
	const Color RED = Color(255, 0, 0);
	const Color GREEN = Color(0, 255, 0);
	const Color CYAN = Color(255, 100, 0);
	const Color WHITE = Color(255, 255, 255);
	const Color BLACK = Color(0, 0, 0);
	// TODO: Add more colors
}

inline ImVec4 ColorToVec4(Color color)
{
	float r = color.r / 255.0f;
	float g = color.g / 255.0f;
	float b = color.b / 255.0f;
	float alpha = color.a / 255.0f;
	return ImVec4(r, g, b, alpha);
}
