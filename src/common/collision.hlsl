// collision.hlsl
// Author: abramann

#define IN_RANGE(n, a, b) (bool)( (n > a && n < b) || (n > b && n < a))

struct V3
{
	float x, y, z;
};

struct Space
{
	V3 v1, v2, v3, v4;
	float getMaxX() 
	{
		float xMax = v1.x;
		if (v2.x > xMax)
			xMax = v2.x;
		if (v3.x > xMax)
			xMax = v3.x; 
		if (v4.x > xMax)
			xMax = v4.x;

		return xMax;
	}

	float getMinX()
	{

		float xMin = v1.x;
		if (v2.x < xMin)
			xMin = v2.x;
		if (v3.x < xMin)
			xMin = v3.x;
		if (v4.x < xMin)
			xMin = v4.x;

		return xMin;
	}

	float getMaxY()
	{
		float yMax = yMax = v1.y;
		if (v2.y > yMax)
			yMax = v2.y;
		if (v3.y > yMax)
			yMax = v3.y;
		if (v4.y > yMax)
			yMax = v4.y;

		return yMax;
	}

	float getMinY()
	{
		float yMin = yMin = v1.y;
		if (v2.y < yMin)
			yMin = v2.y;
		if (v3.y < yMin)
			yMin = v3.y;
		if (v4.y < yMin)
			yMin = v4.y;

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
RWStructuredBuffer<int> g_result : register(u0);

[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint id = DTid.x;	
	if (id == 0)
		g_result[0] = 0;
	if (g_result[0] == 1)
		return;
	
	if(areSpacesCollided(g_noSpace[id], g_space[0]))
		g_result[0] = 1;
}