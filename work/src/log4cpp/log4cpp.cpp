#include "log4cpp.h"
#include <iostream>

LogUtil::LogUtil(std::string &FilePath, std::string &Configure)
	{
		m_sConfFilePath = FilePath;
		m_sConfigure = Configure;
		LogInit();
	}
	
void LogUtil::LogInit()
	{
		//读取解析配置文件
	    log4cpp::PropertyConfigurator::configure(m_sConfFilePath);
		//实例化category对象
	    log4cpp::Category& root = log4cpp::Category::getRoot();
	    log4cpp::Category& main = 
	    	log4cpp::Category::getInstance(m_sConfigure);
	    m_pLogger = & main;
	}	

void LogUtil::LogUpdate(std::string &FilePath, std::string &Configure)
	{
		m_sConfFilePath = FilePath;	
		m_sConfigure = Configure;
		LogInit();
	}
