#include "LogicSystem.h"
#include "Renderer.h"
#include "IObject.h"

std::shared_ptr<CLogicSystem> g_pLogicSystem;

CLogicSystem::CLogicSystem()
{
}

CLogicSystem::~CLogicSystem()
{
}

void CLogicSystem::startup()
{
}

void CLogicSystem::update()
{
}

void CLogicSystem::reset()
{
}

void CLogicSystem::perform()
{
}

void CLogicSystem::registerObject(IObject* pObject)
{
	m_pObjects.emplace(pObject);
	//g_pRenderer->registerModel(pObject->getModel());
}

void CLogicSystem::unregisterObject(IObject* pObject)
{
	m_pObjects.erase(pObject);
	//g_pRenderer->removeObject(pObject->getModel());
}

void CLogicSystem::registerAttack(Attack* pAttack)
{
	m_pAttacks.emplace(pAttack);
}

void CLogicSystem::unregisterAttack(Attack* pAttack)
{
	m_pAttacks.erase(pAttack);
}

bool CLogicSystem::requestAttack(IObject* pObject)
{
	return false;
}

bool CLogicSystem::requestMove(IObject* pObject)
{
	return false;
}

bool CLogicSystem::requestHeal(IObject* pObject)
{
	return false;
}

bool CLogicSystem::requestDestroy(IObject* pObject)
{
	return false;
}

bool CLogicSystem::requestReset(IObject* pObject)
{
	return false;
}

bool CLogicSystem::requestFree(IObject* pObject)
{
	unregisterObject(pObject);
	return false;
}

uint CLogicSystem::getObjecstCount() const
{
	return m_pObjects.size();
}
