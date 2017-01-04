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
	char ch;
	MQTTClient mqClient;
	mqClient = MqttConnt();
    MQTTClient_subscribe(mqClient, TOPIC, QOS);

    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_disconnect(mqClient, 10000);
    MQTTClient_destroy(&mqClient);
    return 1;
}