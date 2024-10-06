#include "GameLogic.h"
#include "IRenderer.h"
#include "IObject.h"

std::shared_ptr<CGameLogic> g_pGameLogic;

CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
}

void CGameLogic::registerObject(IObject* pObject)
{
	g_pRenderer->registerModel(pObject->getModel());
	
}

void CGameLogic::unregisterObject(IObject* pObject)
{
	g_pRenderer->removeObject(pObject->getModel());
}

void CGameLogic::registerAttack(Attack* attack)
{
}

void CGameLogic::unregisterAttack(Attack* attack)
{
}

bool CGameLogic::requestAttack(IObject* pObject)
{
	return false;
}

bool CGameLogic::requestMove(IObject* pObject)
{
	return false;
}

bool CGameLogic::requestDie(IObject* pObject)
{
	return false;
}
