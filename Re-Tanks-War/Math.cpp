#include "VecMath.h"
#include "IRenderer.h"


void makeUnitSquare(Vertex& vert)
{
	auto bbHeight = g_pRenderer->getBackbufferHeight();
	auto bbWidth = g_pRenderer->getBackbufferWidth();

	vert.x /= bbWidth;
	vert.y /= bbHeight;

	// Make relative to center down left
	vert.x -= 1;
	vert.y -= 1;
}