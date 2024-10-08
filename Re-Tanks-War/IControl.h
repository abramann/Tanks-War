#pragma once

class IObject;

class IControl
{
public:
	virtual void getCommand(IObject* pObject) = 0;
};

class CHumanControl : IControl
{
public:
	void getCommand(IObject* pObject) override;

	void handleInput();
private:
};

class AIControl : IControl
{
public:
	void getCommand(IObject* pObject) override;

	void think(IObject* pObject);
private:
};