#pragma once

#include <vector>
#include <algorithm>
#include "System.h"
#include "SystemComponent.h"

class ISubsystem : public ISystem, public ISystemComponent
{
public:
	virtual std::string getName() const { return "Subsystem"; };
	virtual void __declspec(deprecated) ISystemComponent::reset() = 0;
	virtual void __declspec(deprecated) ISystemComponent::update() = 0;

	// For ambiguous(update, reset)
	__forceinline ISystem* getSystem() { return this; }
};

template<typename Subsystem>
__forceinline constexpr void subsystemRegister(std::vector<Subsystem*>& pSubsystems, Subsystem* pComponent)
{
#ifdef _TEST
	auto it = std::find(pSubsystems.begin(), pSubsystems.end(), pComponent);
	CHECK_ERROR(it == pSubsystems.end(), "Invalid registering operation");
#endif 
	pSubsystems.push_back(pComponent);
}

template<typename Subsystem>
__forceinline constexpr void subsystemUnregister(std::vector<Subsystem*>& pSubsystems, Subsystem* pComponent)
{
	auto it = std::find(pSubsystems.begin(), pSubsystems.end(), pComponent);
#ifdef _TEST 		
	CHECK_ERROR(it == m_pSystems.end(), "Invalid unregistering operation");
#endif 	
	pSubsystems.erase(it);
}

template<typename T>
__forceinline constexpr void subsystemsStartup(ISystem* pThisSystem, std::vector<T*>& pSubsystems)
{
	std::for_each(pSubsystems.begin(), pSubsystems.end(), 
		[pThisSystem](T* pSubsystem)
		{ 
			pSubsystem->startup();
			pSubsystem->setSystemHandler(pThisSystem);
		});
}

#define subsystemsDo(pSubsystems, act) \
	 std::for_each(pSubsystems.begin(), pSubsystems.end(), [](decltype(pSubsystems)::value_type pSubsystem) { pSubsystem->##act(); });

#define subsystemsDo2(pSubsystems, act, param) \
	 std::for_each(pSubsystems.begin(), pSubsystems.end(), [ param ](decltype(pSubsystems)::value_type pSubsystem) { pSubsystem->##act(##param); });

#define SubsystemsDo2static(pSubsystems, act, param) \
	 std::for_each(pSubsystems.begin(), pSubsystems.end(), [](decltype(pSubsystems)::value_type pSubsystem) { pSubsystem->##act(##param); });
