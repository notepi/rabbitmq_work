#include "log.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

//extern LogUtil g_cMain;

int main(int argc, char* argv[])
{
//	log4cpp::Category *logger_;
//	system("pwd");
//	//读取解析配置文件
//    log4cpp::PropertyConfigurator::configure("../conf/log4cpp.conf");
//	//实例化category对象
//    log4cpp::Category& root = log4cpp::Category::getRoot();
//    log4cpp::Category& main = 
//    log4cpp::Category::getInstance(std::string("MAIN"));
//    logger_ = & main;
//    //使用category对象进行日志处理
//    main.debug("This is debug");
//    main.info("This is info");
//    main.alert("This is alert");
//    logger_->alert("test");
//	cout << &root << endl;
//	cout << &main << endl;
//    return 0;

	int a = 5;
    debug_log("This is debug:%d", a);
}
