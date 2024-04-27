// logger.h
// Author: abramann

#pragma once
#include "types.h"

enum _LogLevel {
	INFO,
	WARNING,
	FATAL_ERROR
};
typedef int8_t LogLevel;

class Logger
{
public:
	
	Logger();
	~Logger();
	void log(std::string logText, LogLevel level);
private:

	std::string Logger::format(const std::string& logText, LogLevel logLevel);
	std::map<std::string, Vec4> m_log;
};