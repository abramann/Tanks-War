#pragma once

#include "Subsystem.h"
#include "Types.h"
#include <vector>

class IObject;
class ISystemComponent;
struct Attack;

class CLogicSystem : public ISubsystem
{
public:
	std::string getName() const override { return "LogicSystem"; }
	
	CLogicSystem();
	~CLogicSystem();
	
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pObject) override;
	void unregisterComponent(ISystemComponent* pObject) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

	/*bool requestAttack(IObject* pObject);
	bool requestMove(IObject* pObject);
	bool requestHeal(IObject* pObject);
	bool requestDestroy(IObject* pObject);
	bool requestReset(IObject* pObject);
	bool requestFree(IObject* pObject);*/

	uint getObjecstCount() const;

private:
	void registerAttack(Attack* pAttack); // Every object attack should register his attack to be updated by this class
	void unregisterAttack(Attack* pAttack); // Used when player attack his allies or attack canceled due long range

	std::vector<IObject*> m_pObjects;
	std::vector<Attack*> m_pAttacks;
};

extern CLogicSystem* g_pLogicSystem;
