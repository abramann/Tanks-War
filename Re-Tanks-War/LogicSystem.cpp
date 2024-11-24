#include "LogicSystem.h"
#include "Renderer.h"

static CLogicSystem logicSystem;
CLogicSystem* g_pLogicSystem = &logicSystem;

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
	//subsystemsDo(m_pEvents, update);
}

void CLogicSystem::reset()
{
	//subsystemsDo(m_pEvents, reset);
}

void CLogicSystem::perform()
{
}

void CLogicSystem::shutdown()
{
}

void CLogicSystem::onStartGame()
{
}

void CLogicSystem::onQuitGame()
{
}

void CLogicSystem::onPauseGame()
{
}

void CLogicSystem::onResumGame()
{
}

void CLogicSystem::registerComponent(ISystemComponent* pObject)
{
	//subsystemRegister(m_pObjects, dynamic_cast<IObject*>(pObject));
}


void CLogicSystem::unregisterComponent(ISystemComponent* pObject)
{
	//subsystemUnregister(m_pObjects, dynamic_cast<IObject*>(pObject));
}

void CLogicSystem::handleEvent(ISystemComponent* pComponent, int eventCode, void* event)
{
}

/*void CLogicSystem::registerAttack(Attack* pAttack)
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
	//unregisterComponent(pObject);
	return false;
}*/
