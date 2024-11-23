#pragma once

#include "SystemComponent.h"
#include "VecMath.h"


enum SubstanceType
{
	MINERAL,
	ROCK,
	GLASS,
};

struct Strike;
class IDynamicEntity;

class IEntity : public ISystemComponent
{
public:
	virtual void onCreated() = 0;
	virtual void onBeginBumped(IDynamicEntity* pBumper) = 0;
	virtual void onEndBumped() = 0; 
	virtual void onStriked(IEntity* pAttacker ,Strike* pStrike) = 0;
	virtual bool isMoveable() = 0;
	virtual SubstanceType getSubstanceType() const = 0;
	virtual const Bound& getBound() const = 0;

	const Vertex& getLocation() const { return m_location; }

protected:
	Vertex m_location;
};

class IDynamicEntity : public IEntity
{
public:
	bool isMoveable() override { return true; }

	virtual void onBeginMove() = 0;// do animation | audio | paritcles
	virtual void onEndMove() = 0;
	
	// virtual .. onDirect(angle/point)

	Vertex getDirection() const { return m_direction; }
	float getEnergy() const { return m_energy; }
	float getWeight() const { return m_weight; }

protected:
	Vertex m_direction;
	float m_energy;
	float m_weight;
};

class IStaticEntity : public IEntity
{
public:
	bool isMoveable() override { return false; }
};