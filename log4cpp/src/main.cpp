#include "log.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

//extern LogUtil g_cMain;

int main(int argc, char* argv[])
{
//	log4cpp::Category *logger_;
//	system("pwd");
//	//��ȡ���������ļ�
//    log4cpp::PropertyConfigurator::configure("../conf/log4cpp.conf");
//	//ʵ����category����
//    log4cpp::Category& root = log4cpp::Category::getRoot();
//    log4cpp::Category& main = 
//    log4cpp::Category::getInstance(std::string("MAIN"));
//    logger_ = & main;
//    //ʹ��category���������־����
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
