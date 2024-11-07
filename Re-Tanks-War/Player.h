#pragma once

#include "SystemComponent.h"
#include <string>

enum PlayerRelationship
{
	PLAYER_ALLY,
	PLAYER_ENEMY
};

class IObject;

struct Score
{
	int deathCount;
	int killCount;
};

class IPlayer : public ISystemComponent
{
public:
	std::string getName() const override { return "Player"; }

	virtual int getTeam() const
	{
		return m_teamID;
	}

	virtual std::string getPlayerName() const
	{
		return m_name;
	};
	
	//virtual void getRelationship(IPlayer* pPlayer) = 0;
	
	virtual Score getScore() const
	{
		return m_score;
	};
	
	virtual IObject* getObject() const
	{
		return m_pObject;
	};
	
	virtual void assignObject(IObject* pObject)
	{
		m_pObject = pObject;
	}

	virtual void assignID(int id)
	{
		m_id = id;
	}

	virtual void getPing() = 0;

protected:
	int m_id;
	int m_teamID;
	std::string m_name;
	Score m_score;
	IObject* m_pObject;
};

class CHumanPlayer : IPlayer
{
	bool isObserver();
};

class CServerPlayer : CHumanPlayer
{

};

class CThisPlayer : CHumanPlayer
{

};

class CAIPlayer : IPlayer
{

};