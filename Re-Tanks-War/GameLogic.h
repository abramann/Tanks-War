#pragma once

#include <memory>

class IObject;
struct Attack;

class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();
	
	void registerObject(IObject* pObject);
	void unregisterObject(IObject* pObject);

	void registerAttack(Attack* attack); // Every object attack should register his attack to be updated by this class
	void unregisterAttack(Attack* attack); // Used when player attack his allies or attack canceled due long range

	bool requestAttack(IObject* pObject);
	bool requestMove(IObject* pObject);
	bool requestDie(IObject* pObject);
private:

};

extern std::shared_ptr<CGameLogic> g_pGameLogic;
