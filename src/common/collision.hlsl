// collision.hlsl
// Author: abramann

#define IN_RANGE(n, a, b) (bool)( (n > a && n < b) || (n > b && n < a))
#define IN_RANGE_OR_EQUAL(n, a, b) (bool)( (n >= a && n <= b) || (n >= b && n <= a))

struct V3
{
	float x, y, z;
};

struct Space
{
	V3 v1, v2, v3, v4;
	float getMaxX()
	{
		float xMax = max(v1.x, v2.x);
		xMax = max(xMax, v3.x);
		xMax = max(xMax, v4.x);
		return xMax;
	}

	float getMinX()
	{
		float xMin = min(v1.x, v2.x);
		xMin = min(xMin, v3.x);
		xMin = min(xMin, v4.x);
		return xMin;
	}

	float getMaxY()
	{
		float yMax = max(v1.y, v2.y);
		yMax = max(yMax, v3.y);
		yMax = max(yMax, v4.y);
		return yMax;
	}

	float getMinY()
	{
		float yMin = min(v1.y, v2.y);
		yMin = min(yMin, v3.y);
		yMin = min(yMin, v4.y);
		return yMin;
	}
};

int areSpacesCollided(Space s1, Space s2)
{
	float maxX1 = s1.getMaxX(),
		minX1 = s1.getMinX(),
		maxY1 = s1.getMaxY(),
		minY1 = s1.getMinY();
	if (IN_RANGE(s2.v1.x, minX1, maxX1) ||
		IN_RANGE(s2.v2.x, minX1, maxX1) ||
		IN_RANGE(s2.v3.x, minX1, maxX1) ||
		IN_RANGE(s2.v4.x, minX1, maxX1)
		)
		if (IN_RANGE(s2.v1.y, minY1, maxY1) ||
			IN_RANGE(s2.v2.y, minY1, maxY1) ||
			IN_RANGE(s2.v3.y, minY1, maxY1) ||
			IN_RANGE(s2.v4.y, minY1, maxY1)
			)
			return 1;

	return 0;
}

StructuredBuffer<Space> g_noSpace : register(t0);
StructuredBuffer<uint> g_mapCells : register(t1);
StructuredBuffer<Space> g_space : register(t2);
StructuredBuffer<int> g_mapRange : register(t3);
RWStructuredBuffer<int> g_result : register(u0);

bool isOutOfRange(Space space)
{
	int x = g_mapRange[0],
		y = g_mapRange[1];
	if (IN_RANGE_OR_EQUAL(space.v1.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v2.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v3.x, 0, x) &&
		IN_RANGE_OR_EQUAL(space.v4.x, 0, x)
		)
		if (IN_RANGE_OR_EQUAL(space.v1.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v2.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v3.y, 0, y) &&
			IN_RANGE_OR_EQUAL(space.v4.y, 0, y)
			)
			return false;

	return true;
}

[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint id = DTid.x;
	if (id == 0)
		g_result[0] = 0;
	if (g_result[0] == 1)
		return;

	if (isOutOfRange(g_space[0]) || areSpacesCollided(g_noSpace[id], g_space[0]))
		g_result[0] = 1;
}