#include "log.h"
static std::string s_sFilePath = "../conf/log4cpp.conf";
static std::string s_sConfigure = "MAIN";
	
LogUtil g_cMain(s_sFilePath, s_sConfigure);

