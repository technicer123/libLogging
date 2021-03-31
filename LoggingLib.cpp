#include "LoggingLib.hpp"

namespace {
	class LogFileBuilder{
	private:
		std::string m_timezone = "default";
		std::string m_logfilePath = "";
		void generateLogFilePath(){
			   std::time_t m_time = std::time(0);
#ifndef _SYSTEM_WINDOWS
			   std::string m_timeString = std::ctime(&m_time);
			   if(this->m_timezone.compare("default")){
				   this->m_logfilePath = this->m_logfilePath + std::regex_replace(m_timeString, std::regex("\n"), "") + ".log";
			   }else{
				   std::tm *m_timeStructure = gmtime(&m_time);
				   m_timeString = asctime(m_timeStructure);
				   this->m_logfilePath = this->m_logfilePath + std::regex_replace(m_timeString, std::regex("\n"), "") + ".log";
			   }
#else 
			   char m_timeString[26];
			   if(this->m_timezone.compare("default")) {
				   ctime_s(m_timeString, 26, &m_time);
				   this->m_logfilePath = this->m_logfilePath + std::regex_replace(m_timeString, std::regex("\n"), "") + ".log";
			   }
			   else {
				   std::tm* m_timeStructure = new std::tm;
				   gmtime_s(m_timeStructure, &m_time);
				   asctime_s(m_timeString, 26, m_timeStructure);
				   this->m_logfilePath = this->m_logfilePath + std::regex_replace(m_timeString, std::regex("\n"), "") + ".log";
			   }
#endif
		}
	public:
		void setTimezone(std::string t_timezone){
			this->m_timezone = t_timezone;
		}
		void setLogFilePath(std::string t_logFilePath){
			this->m_logfilePath = t_logFilePath;
		}
		void generateLogFile(LogFile* t_logFile){
			generateLogFilePath();
			t_logFile->m_logFilePath = this->m_logfilePath;
			t_logFile->m_logFile.open(this->m_logfilePath, std::ios::out);
			std::cout<<t_logFile->m_logFile.is_open();
		}
	};
	class MessageBuilder{
	private:
		std::string getSeverityLevelAsString(uint32_t t_severityLevel){
			switch(t_severityLevel){
				case 0:
					return "INFO";
				case 1:
					return "WARN";
				case 2:
					return "ERROR";
				case 3:
					return "CRITICAL";
				default:
					throw std::invalid_argument("Severity Level not recognized");
			}
		}
	public:
		std::string generateMessage(std::string t_message, uint32_t t_severityLevel){
			char m_timeBuffer[10];
			std::time_t m_time = std::time(nullptr);
			std::tm* m_timeStructure = new std::tm;
#ifndef _SYSTEM_WINDOWS
			m_timeStructure = std::localtime(&m_time);
#else
			localtime_s(m_timeStructure, &m_time);
#endif
			std::strftime(m_timeBuffer, 10, "%H:%M:%S", m_timeStructure);
			std::string m_timeString = std::string(m_timeBuffer);
			return "[" + m_timeString + "]: [" + getSeverityLevelAsString(t_severityLevel) + "]: " + t_message;
		}
	};
	class InternalLoggerInstance{
	public:
		LogFile m_logFile;
		MessageBuilder m_messageBuilder;
		InternalLoggerInstance(LogFile t_logFile, MessageBuilder t_messageBuilder):m_logFile(t_logFile),m_messageBuilder(t_messageBuilder){

		}
		InternalLoggerInstance() {

		}
	};
	std::vector<InternalLoggerInstance> m_loggerInstances;
	InternalLoggerInstance getInternalLoggerInstance(LoggerInstance t_loggerInstance){
		return m_loggerInstances.at(t_loggerInstance);
	}

};
void createLoggerInstance(LoggerInstanceCreateInfo* t_loggerInstanceCreateInfo,LoggerInstance* t_loggerInstance){
	LogFileBuilder* m_logFileBuilder = new LogFileBuilder();
	for(LoggerInstanceProperty m_loggerInstaceProperty : t_loggerInstanceCreateInfo->m_loggerInstanceProperties){
		if(m_loggerInstaceProperty.m_key.compare("Timezone")){
			m_logFileBuilder->setTimezone(m_loggerInstaceProperty.m_value);
		}else if(m_loggerInstaceProperty.m_key.compare("LogFilePath")){
			m_logFileBuilder->setLogFilePath(m_loggerInstaceProperty.m_value);
		}
	}

	InternalLoggerInstance* m_internalLoggerInstance = new InternalLoggerInstance();
	m_internalLoggerInstance->m_messageBuilder = MessageBuilder();
	m_logFileBuilder->generateLogFile(&m_internalLoggerInstance->m_logFile);

	m_loggerInstances.push_back(*m_internalLoggerInstance);
	*t_loggerInstance = static_cast<LoggerInstance>(m_loggerInstances.size()-1);
}
void log(LoggerInstance t_loggerInstance,std::string t_message,uint32_t t_severityLevel){
	InternalLoggerInstance m_internalLoggerInstance = getInternalLoggerInstance(t_loggerInstance);
	std::string m_message = m_internalLoggerInstance.m_messageBuilder.generateMessage(t_message, t_severityLevel);
	m_internalLoggerInstance.m_logFile.m_logFile << m_message << '\n';
	std::cout<<m_internalLoggerInstance.m_logFile.m_logFile.is_open()<<std::endl;
}
void destroyLoggerInstance(LoggerInstance* t_loggerInstance){
	InternalLoggerInstance m_internalLoggerInstance = getInternalLoggerInstance(*t_loggerInstance);
	m_internalLoggerInstance.m_logFile.m_logFile.flush();
	m_internalLoggerInstance.m_logFile.m_logFile.close();
}
