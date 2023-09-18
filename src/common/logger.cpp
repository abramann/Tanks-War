#include "logger.h"
#include "inlined.inl"
#include <fstream>

const std::map<LogLevel, Vec4> LOG_LEVEL_COLOR =
{
	{INFO, colorNS::WHITE},
	{WARNING, colorNS::ORANGE },
	{FATAL_ERROR,colorNS::RED}
};

const std::map<LogLevel, std::string> LOG_LEVEL_STRING =
{
	{ INFO, "INFO" },
	{ WARNING, "WARNING" },
	{ FATAL_ERROR,"ERROR" }
};

Logger::Logger()
{
	ofstream newLog("log.txt");
	newLog << format("Logger begin.", INFO).c_str() << std::endl;
}

Logger::~Logger()
{
	log("Logger end.", INFO);
}

void Logger::log(std::string logText, LogLevel logLevel)
{
	std::fstream hLogFile("log.txt");
	hLogFile.seekg(0, std::ios::end);
	hLogFile << format(logText, logLevel).c_str() << std::endl;
	m_log[logText] = LOG_LEVEL_COLOR.at(logLevel);
}

std::string Logger::format(const std::string& logText, LogLevel logLevel)
{
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	return strFormat("[%d:%d:%d]: %s : %s", sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
		LOG_LEVEL_STRING.at(logLevel).c_str(), logText.c_str());
}
