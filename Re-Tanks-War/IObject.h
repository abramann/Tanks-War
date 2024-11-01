#pragma once

#include "types.h"
#include "Entity.h"
#include <string>
#include <vector>

enum Substance
{
	SUBSTANCE_MINERAL,
	SUBSTANCE_ROCK,
	SUBSTANCE_GLASS,

};

enum ObjectState
{
	OBJECTSTATE_ALIVE,
	OBJECTSTATE_DESTROYED,

};

class CModel;
class IControl;
struct Attack;
struct Command;

class IObject : IEntity
{
public:
	virtual std::string getName() const { return "Object"; }

	virtual void handleCommand(Command* command) = 0;
	virtual void onStriked(Attack* attack) = 0;
	virtual void onDestroyed(Attack* attack) = 0;
	virtual void executeAttack() = 0;
	virtual void executeMove() = 0;
	virtual void executeJump() = 0;
	virtual void heal() = 0;
	virtual void destroy() = 0;
	virtual void free() = 0;

	// get
	virtual Substance getSubstance() = 0;
	virtual int getState() const { return m_state; }
	virtual float getHealth() const { return m_health; }
	virtual float getWeight() const { return m_weight; }
	virtual float getVelocity() const { return m_velocity; };
	virtual float getEnergy() const { return m_energy; }
	virtual CModel* getModel() const { return m_pModel; };

protected:
	int m_state;
	float m_health;
	float m_weight;
	float m_velocity;
	float m_energy;
	CModel* m_pModel;
};
