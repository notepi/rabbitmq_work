#include "log.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

//extern LogUtil g_cMain;

int main(int argc, char* argv[])
{
	int a = 5;
    debug_log("This is debug:%d", a);
}
