#include "types.h"
#include <memory>
#include <string>

#pragma once

template <typename T>
void safeRelease(T ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = 0;
	}
}

template <typename T>
inline void safeDelete(T ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = 0;
	}
}

template <typename T>
inline void safeDeleteArray(T ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

inline int32 random(int32 a, int32 b)
{
	srand(GetTickCount());
	return (a + (rand() % (b - a + 1)));
}

inline void debuggerBreak(const std::string& dbgMsg = "")
{
	if (IsDebuggerPresent())
	{
		OutputDebugStringA(dbgMsg.c_str());
		DebugBreak();
	}
}

//	https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
inline std::string strFormat(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

inline void messageBoxOk(std::string msg, std::string title)
{
	MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK);
}

inline V3 getSpaceCenter(Space space)
{
	V3 center;
	center.x = space.getMaxX() - space.getMinX();
}