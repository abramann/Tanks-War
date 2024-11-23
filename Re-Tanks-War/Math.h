#pragma once

#include "VecMath.h"
#include "MatMath.h"
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#else
#define max std::max
#define min std::min
#endif

namespace gameMathNS
{
	//	inlined functions used by Space struct
	template<typename T>
	inline T getMax(std::vector<T> val)
	{
		T maxValue = val[0];
		for (auto v : val)
			maxValue = max(maxValue, v);

		return maxValue;
	}

	template<typename T>
	inline T getMin(std::vector<T> val)
	{
		T minValue = val[0];
		for (auto v : val)
			minValue = min(minValue, v);

		return minValue;
	}

	template <typename T>
	inline void add4(const T amount, T& v1, T& v2, T& v3, T& v4)
	{
		v1 += amount,
			v2 += amount,
			v3 += amount,
			v4 += amount;
	}

	inline float DegToRad(const float& angle) { return angle * 0.0174532925199432957f; }
	inline float RadToDeg(const float& angle) { return angle * 57.295779513082320876f; }
}

extern void makeUnitSquare(Vertex& vert);
extern Vertex rotateVertex(Vertex vertex, float angle, Vertex origin = Vertex(0, 0));
