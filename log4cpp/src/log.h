#ifndef __LOG_H__
#define __LOG_H__
#include "log4cpp.h"

extern LogUtil g_cMain;
/*常规文件log打印信息宏*/
#define debug_log(format,...)	g_cMain.m_pLogger->debug("%s , %d, %s: "format,\
														__FILE__, __LINE__,__FUNCTION__,\
														##__VA_ARGS__)
#define info_log(format,...)	g_cMain.m_pLogger->info(format,##__VA_ARGS__)
#define warn_log(format,...)	g_cMain.m_pLogger->warn(format,##__VA_ARGS__)
#define err_log(format,...)		g_cMain.m_pLogger->error(format,##__VA_ARGS__)
#define fatal_log(format,...)	g_cMain.m_pLogger->fatal(format,##__VA_ARGS__)

#endif
