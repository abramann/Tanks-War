#pragma once

#include "types.h"
#include <string>
#include <vector>

class CModel;
class IControl;
class IAttack;

enum Substance
{
	SUBSTANCE_MINERAL,
	SUBSTANCE_ROCK,
	SUBSTANCE_GLASS,

};

enum ObjectState
{
	OBJECTSTATE_ACTIVE,
};

class IEntity
{
protected:
	std::string m_name;
	int m_id;
};

class IObject : IEntity
{
public:
	virtual void onStriked(IAttack* attack) = 0;
	virtual void executeAttack() = 0;
	virtual void executeMove() = 0;
	virtual void update() = 0;
	virtual Substance getSubstance() = 0;
	virtual int getState() const;
	virtual float getHealth() const;
	virtual float getWeight() const;
	virtual float getVelocity() const;
	virtual CModel* getModel() const;	

protected:
	int state;
	float m_health;
	float m_weight;
	float m_velocity;
	int state;
	CModel* m_pModel;
	IControl* m_pControl;
};
