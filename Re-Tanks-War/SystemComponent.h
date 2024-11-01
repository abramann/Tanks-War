#pragma once

#include <string>

class ISystemComponent
{
	static uint64_t s_nextComponentID;
	
public:
	ISystemComponent() : m_componenetID(++s_nextComponentID) {}
	
	virtual void update() = 0;
	virtual void reset() = 0;
	virtual std::string getName() const = 0;

	const int& getComponentID() const { return m_componenetID; }

private:
	const uint64_t m_componenetID;
};

uint64_t ISystemComponent::s_nextComponentID = 0;