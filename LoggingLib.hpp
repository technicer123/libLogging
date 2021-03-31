/*
 * logginglib.h
 *
 *  Created on: 27 Mar 2021
 *      Author: Simon Neuenhausen
 */
#define LIBLOGGING_API_PTR
#ifdef _SYSTEM_WINDOWS
	#ifdef LIBLOGGING_EXPORTS
	#define LIBLOGGING_API_CALL __declspec(dllexport)
	#define LIBLOGGING_API_TYPE __declspec(dllexport)
	#elif !defined LIBLOGGING_EXPORTS && defined _SYSTEM_WINDOWS
	#define LIBLOGGING_API_CALL __declspec(dllimport)
	#define LIBLOGGING_API_TYPE __declspec(dllimport)
	#endif

#elif defined _SYSTEM_LINUX 

#define LIBLOGGING_API_CALL extern
#define LIBLOGGING_API_TYPE

#elif defined _SYSTEM_MACOSX

#define LIBLOGGING_API_CALL
#define LIBLOGGING_API_TYPE

#elif defined _SYSTEM_OTHER

#define LIBLOGGING_API_CALL
#define LIBLOGGING_API_TYPE

#else

#error("System not defined")

#define LIBLOGGING_API_CALL
#define LIBLOGGING_API_TYPE

#endif

#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <regex>
#ifndef INCLUDE_LOGGINGLIB_H_
#define INCLUDE_LOGGINGLIB_H_

enum LIBLOGGING_API_TYPE SeverityLevels{
	LOGGING_SEVERITY_LEVEL_INFO = 0,
	LOGGING_SEVERITY_LEVEL_WARN = 1,
	LOGGING_SEVERITY_LEVEL_ERROR = 2,
	LOGGING_SEVERITY_LEVEL_CRITICAL = 3
};

class LIBLOGGING_API_TYPE LoggerInstanceProperty{
public:
	std::string m_key;
	std::string m_value;
};
class LIBLOGGING_API_TYPE LoggerInstanceCreateInfo{
public:
	std::vector<LoggerInstanceProperty> m_loggerInstanceProperties;
};
class LIBLOGGING_API_TYPE LogFile{
public:
	std::string   m_logFilePath;
	std::ofstream m_logFile;
	LogFile(const LogFile&){

	}
	LogFile(){

	}
};
typedef LIBLOGGING_API_TYPE uint32_t LoggerInstance;


LIBLOGGING_API_CALL void createLoggerInstance(LoggerInstanceCreateInfo* t_loggerInstanceCreateInfo,LoggerInstance* t_loggerInstance);

LIBLOGGING_API_CALL void logMessage(LoggerInstance t_loggerInstance,std::string t_message,uint32_t t_severityLevel);

LIBLOGGING_API_CALL void destroyLoggerInstance(LoggerInstance* t_loggerInstance);
#endif /* INCLUDE_LOGGINGLIB_H_ */
