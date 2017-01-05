#include "log.h"

#include "mqtt.h"
#include "mqttpublic.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

//
//int main(int argc, char* argv[])
//{
//	int a = 5;
//    debug_log("This is debug:%d", a);
//}








char *TOPIC = "sensor";
int QOS = 1;

char *PAYLOAD = "Hello World!";
long TIMEOUT = 10000L;

int main(int argc, char* argv[])
{
	MQTTUtil hh("localhost", "123");
	hh.MQTTSubscribe(TOPIC, QOS);

	while (1);
    return 1;
}