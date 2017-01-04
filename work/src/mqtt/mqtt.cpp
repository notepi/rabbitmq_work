#include "mqtt.h"

#include "MQTTClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ADDRESS = "tcp://localhost:1883";
char *CLIENTID = "ExampleClientSub";

volatile MQTTClient_deliveryToken deliveredtoken;

/*
						int nKeepAliveInterval,				//��������ʱ��
						int nCleanSession,					//����ȡ���󣬷������Ƿ�ɾ������
						int nReliable,
						MQTTClient_willOptions *pfmqcWill,	//will��Ϣ
						char *pfcUserName,					//�û���
						char *pfcPassWord,					//�û�����
						int nConnectTimeout,				//�ر�����ʱ��
						int nRetryInterval,					//�������
						MQTTClient_SSLOptions *pfmqcSSL,	//SSL
						int nServerURIcount,
						char **ppfcServerURIs,
*/
MQTTClient MqttConnt(	
					)
{
    MQTTClient mqClient;
	/*MQTTClient_connectOptions_initializer   
		{ {'M', 'Q', 'T', 'C'}, 4, 60, 1, 1, NULL, NULL, NULL, 30, 20, NULL, 0, NULL, 0}
	*/
	/*
		MQTTClient_connectOptions Data Fields

		char 	struct_id [4]						{'M', 'Q', 'T', 'C'}
		int 	struct_version						4
		int 	keepAliveInterval					60
		int 	cleansession						1
		int 	reliable							1
		MQTTClient_willOptions * 	will			NULL
		char * 	username							NULL
		char * 	password							NULL
		int 	connectTimeout						30
		int 	retryInterval						20
		MQTTClient_SSLOptions * 	ssl				NULL
		int 	serverURIcount						0
		char ** 	serverURIs						NULL
		int 	MQTTVersion							0
		struct {
		   char *   serverURI
		   int   MQTTVersion
		   int   sessionPresent
		}
	*/
    MQTTClient_connectOptions mqConnOpts = MQTTClient_connectOptions_initializer;
    int nRc;
    int nCh;

    MQTTClient_create(&mqClient, ADDRESS, CLIENTID,
        				MQTTCLIENT_PERSISTENCE_NONE, NULL);
    mqConnOpts.keepAliveInterval = 20;
    mqConnOpts.cleansession = 1;
    MQTTClient_setCallbacks(mqClient, NULL, _OnMqttConnLostED, _OnMqttMsgArrvED, _OnMqttDeliverED);

    if ((nRc = MQTTClient_connect(mqClient, &mqConnOpts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", nRc);
        exit(-1);       
    }
	printf("ok!");
	return mqClient;
}