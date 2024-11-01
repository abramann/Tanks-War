#pragma once

#include "System.h"
#include "Types.h"
#include <memory>
#include <set>

class IObject;
class Attack;

class CLogicSystem : ISystem
{
public:
	CLogicSystem();
	~CLogicSystem();
	
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;

	void registerObject(IObject* pObject);
	void unregisterObject(IObject* pObject);

	void registerAttack(Attack* pAttack); // Every object attack should register his attack to be updated by this class
	void unregisterAttack(Attack* pAttack); // Used when player attack his allies or attack canceled due long range

	bool requestAttack(IObject* pObject);
	bool requestMove(IObject* pObject);
	bool requestHeal(IObject* pObject);
	bool requestDestroy(IObject* pObject);
	bool requestReset(IObject* pObject);
	bool requestFree(IObject* pObject);

	uint getObjecstCount() const;

private:
	std::set<IObject*> m_pObjects;
	std::set<Attack*> m_pAttacks;
};

extern std::shared_ptr<CLogicSystem> g_pLogicSystem;
