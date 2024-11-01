#pragma once

#include "String.h"
#include <Windows.h>

#define CHECK_ERROR(RESULT, MSG) if(!(RESULT)) REPORT_ERROR(MSG)
#define REPORT_ERROR(MSG) CGameError(MSG, __LINE__, __FUNCTION__, __FILE__)
#define DEBUG_BREAK_IF_PRESETN if(IsDebuggerPresent()) \
									DebugBreak();
class CGameError
{
public:
	CGameError(const char* errMsg, int line, const char* function, const char* file)
	{
		m_msg = strFormat("Error: %s\n Line: %d\n Function: %s\n File: %s", errMsg, line, function, file);
		DEBUG_BREAK_IF_PRESETN;
	}

	std::string getMessage() const { return m_msg; }

private:
	std::string m_msg;
};

template <typename T>
inline void safeDelete(T ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
inline void safeDeleteArray(T ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
inline void setNull(T ptr, size_t size)
{
	memset(ptr, 0, size);
}

template <typename T>
inline void setNull(T& var)
{
	memset(&var, 0, sizeof(T));
}