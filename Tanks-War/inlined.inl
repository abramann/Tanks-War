// Safely call onResetDevice
#include <iostream>
#include <Windows.h>

extern uint64_t g_frameCounter;


//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}

#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

inline void waitTime(float time)
{
	Sleep(time);
}

inline void waitFrame(int framesWait)
{
	framesWait += ::g_frameCounter;
	while (true)
		if (framesWait < ::g_frameCounter)
			break;
}

inline std::string getTargetEqualStringValue(std::string str)
{
	str.erase(0, str.find('=') + 1);
	return str;
}

inline int8_t sign(float value)
{
	return (value >= 0) ? 1 : -1;
}

inline float _round(float value)
{
	float rValue = std::ceil(value * 10000) / 10000;
	if (abs(rValue) > 1)
		return sign(rValue) * 1;
	return rValue;
}

inline int strComp(std::string s1, std::string s2)
{
	return strcmp(s1.c_str(), s2.c_str());
}

inline std::string getFileNameFromPath(std::string path)
{
	std::string sName = path.substr(path.find_last_of('\\') + 1,
		path.find_last_of('.') - path.find_last_of('\\') - 1);
	return sName;
}

inline uint32_t _rand(uint32_t max)
{
	return ::GetTickCount() % max;
}