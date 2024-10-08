#pragma once

#include <iostream>
#include <Windows.h>

#define CHECK_ERROR(RESULT, MSG) if(!(RESULT)) REPORT_ERROR(MSG)
#define REPORT_ERROR(MSG) CGameError(MSG, __LINE__, __FUNCTION__, __FILE__)

class CGameError
{
public:
	CGameError(const char* errMsg, int line, const char* function, const char* file)
	{
		sprintf(m_msg, "Error: %s\n Line: %d\n Function: %s\n File: %s", errMsg, line, function, file);
		DebugBreak();
	}

	const char* getMessage() const { return m_msg; }

private:
	char m_msg[1024];
};

inline void messageBoxOk(std::string msg)
{
	MessageBoxA(NULL, msg.c_str(), "WARNING", MB_OK);
}

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