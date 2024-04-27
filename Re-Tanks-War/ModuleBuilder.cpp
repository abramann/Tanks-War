#include "ModuleBuilder.h"
#include "TankModel.h"
#include "IRenderer.h"
#include "ITexture.h"


namespace nsModuleBuilder
{
	struct Mesh
	{
		Vertex* pVBData;
		int vbElements;
		int* pIBData;
		int ibElements;
	};

	Mesh makeOverlordMesh();

	CTankModel * buildTankModel(int tankModelType)
	{
		CTankModel* pTankModel = new CTankModel();
		
		ITexture* pChainsATex = g_pRenderer->loadTextureFromFile(L"Tracks\\Track_1_A.png");
		ITexture* pChainsBTex = g_pRenderer->loadTextureFromFile(L"Tracks\\Track_1_B.png");
		Rect8 leftChains;
		leftChains.vert1 = Vertex(33, 0);
		leftChains.vert3 = Vertex(leftChains.vert1.x + pChainsATex->getWidth(), 0);
		leftChains.vert5 = Vertex(leftChains.vert3.x, pChainsATex->getHeight());
		leftChains.vert7 = Vertex(leftChains.vert1.x, leftChains.vert5.y);

		leftChains.vert2 = Vertex(0, 0);
		leftChains.vert4 = Vertex(1, 0);
		leftChains.vert6 = Vertex(1, 1);
		leftChains.vert8 = Vertex(0, 1);

		Rect8 rightChains = leftChains;
		rightChains.vert1 = Vertex(181, 0);
		rightChains.vert3 = Vertex(rightChains.vert1.x + pChainsATex->getWidth(), 0);
		rightChains.vert5 = Vertex(rightChains.vert3.x, pChainsATex->getHeight());
		rightChains.vert7 = Vertex(rightChains.vert1.x, rightChains.vert5.y);

		ITexture* pBodyTex = g_pRenderer->loadTextureFromFile(L"Hulls_Color_A\\Hull_01.png");
		Rect8 body = leftChains;
		body.vert1 = Vertex(0, 0);
		body.vert3 = Vertex(pBodyTex->getWidth(), 0);
		body.vert5 = Vertex(pBodyTex->getHeight(), pBodyTex->getWidth());
		body.vert7 = Vertex(0, pBodyTex->getHeight());

		ITexture* pGunTex = g_pRenderer->loadTextureFromFile(L"Weapon_Color_A\\Gun_01.png");
		Rect8 gun = leftChains;
		gun.vert1 = Vertex(80, 10);
		gun.vert3 = Vertex(gun.vert1.x + pGunTex->getWidth(), gun.vert1.y);
		gun.vert5 = Vertex(gun.vert3.x, gun.vert1.y + pGunTex->getHeight());
		gun.vert7 = Vertex(gun.vert1.x, gun.vert5.y);

		return pTankModel;
	}

	Mesh makeOverlordMesh()
	{

	}
}