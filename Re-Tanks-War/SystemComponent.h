#pragma once

#include <string>

class ISystem;

class ISystemComponent
{
	static uint64_t s_nextComponentID;

public:
	ISystemComponent() : m_componenetID(++s_nextComponentID) {}
	
	virtual void update() = 0;
	virtual void reset() = 0;
	virtual std::string getName() const = 0;
	const uint64_t& getComponentID() const { return m_componenetID; }
	void setSystemHandler(ISystem* pSystem) { m_pSystemHandler = pSystem; }

protected:
	ISystem* m_pSystemHandler;
private:
	const uint64_t m_componenetID;
};
