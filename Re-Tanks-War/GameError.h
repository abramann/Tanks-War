// Author: Wefaq
// Brief: CGameError: handling errors.
#pragma once

#include "StrUtility.h"
#include <Windows.h>

// TODO: Make use of.
enum ErrorType
{
	ERROR_LOGIC,
	ERROR_INVALID_EXECUTION
};

#define CHECK_ERROR(RESULT, MSG) if((RESULT)) REPORT_ERROR(MSG)
#define REPORT_ERROR(MSG) throw CGameError(MSG, __LINE__, __FUNCTION__, __FILE__)
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
