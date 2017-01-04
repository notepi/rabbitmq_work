#ifndef __MQTTPUBLIC_H__
#define __MQTTPUBLIC_H__
#include "mqttpublic.h"

#include "MQTTClient.h"



void _OnMqttDeliverED(void *context, MQTTClient_deliveryToken dt);
int _OnMqttMsgArrvED(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void _OnMqttConnLostED(void *context, char *cause);

#endif