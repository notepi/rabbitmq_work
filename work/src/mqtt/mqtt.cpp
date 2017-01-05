#include "mqtt.h"

#include "MQTTClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ADDRESS = "tcp://localhost:1883";
char *CLIENTID = "ExampleClientSub";

volatile MQTTClient_deliveryToken deliveredtoken;

/*
						int nKeepAliveInterval,				//心跳保持时间
						int nCleanSession,					//订阅取消后，服务器是否删除主题
						int nReliable,
						MQTTClient_willOptions *pfmqcWill,	//will消息
						char *pfcUserName,					//用户名
						char *pfcPassWord,					//用户密码
						int nConnectTimeout,				//关闭链接时间
						int nRetryInterval,					//重连间隔
						MQTTClient_SSLOptions *pfmqcSSL,	//SSL
						int nServerURIcount,
						char **ppfcServerURIs,
*/
MQTTUtil::MQTTUtil(std::string sIP, std::string sClientID)
	{
		m_sIP = sIP;
		m_sAddress = "tcp://" + sIP + ":1883";
		m_sClientID = sClientID;
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
		//m_mqConnOpts = MQTTClient_connectOptions_initializer;	//初始化默认参数
		MQTTClient_create(	&m_mqClient,
							m_sAddress.c_str(),
							m_sClientID.c_str(),
        					MQTTCLIENT_PERSISTENCE_NONE, 
        					NULL);
        MQTTClient_setCallbacks(m_mqClient, NULL, _OnMqttConnLostED, _OnMqttMsgArrvED, _OnMqttDeliverED);
	}
	
MQTTUtil::~MQTTUtil()
	{
		MQTTClient_disconnect(m_mqClient, 10000);
		MQTTClient_destroy(&m_mqClient);	
	}	

void MQTTUtil::MqttInit()
	{
		int nRc;
		if ((nRc = MQTTClient_connect(m_mqClient, &m_mqConnOpts)) != MQTTCLIENT_SUCCESS)
			{
				printf("Failed to connect, return code %d\n", nRc);
				exit(-1);       
			}
		printf("ok!\n");
	}
void MQTTUtil::MQTTSubscribe(std::string sTopic, int nQOS)
	{
		m_sTopic = sTopic;
		m_nQOS = nQOS;
		MqttInit();
		MQTTClient_subscribe(m_mqClient, sTopic.c_str(), m_nQOS);
	}
MQTTClient &MQTTUtil::GetClient()
{
	return m_mqClient;
}