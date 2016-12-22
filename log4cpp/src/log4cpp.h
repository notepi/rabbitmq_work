#ifndef __LOG4CPP_H__
#define __LOG4CPP_H__
#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"

class LogUtil {
	public:
		LogUtil(std::string &FilePath, std::string &Configure);
		log4cpp::Category *m_pLogger;
	private:
		std::string m_sConfFilePath;
		std::string m_sConfigure;
	private:
		void LogInit();
		void LogUpdate(std::string &FilePath, std::string &Configure);
};

#endif