#include "VecMath.h"
#include "IRenderer.h"


Vertex rotateVertex(Vertex vertex, float angle, Vertex origin)
{
	float translatedToOriginX = vertex.x - origin.x;
	float translatedToOriginY = vertex.y - origin.y;

	float rotatedX = translatedToOriginX * cos(angle) - translatedToOriginY * sin(angle);
	float rotatedY = translatedToOriginX * sin(angle) + translatedToOriginY * cos(angle);

	float reverseTranslatedX = rotatedX + origin.x;
	float reverseTranslatedY = rotatedY + origin.y;

	return Vertex(reverseTranslatedX, reverseTranslatedY);
}

void makeUnitSquare(Vertex& vert)
{
	auto bbHeight = g_pRenderer->getBackbufferHeight();
	auto bbWidth = g_pRenderer->getBackbufferWidth();

	vert.x /= bbWidth;
	vert.y /= bbHeight;
	vert *= 2;
	vert = Vertex(vert.x - 1, vert.y - 1);
}