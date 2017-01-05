#ifndef __MQTT_H__
#define __MQTT_H__
#include <iostream>


#include "mqttpublic.h"

class MQTTUtil {
	public:
		MQTTUtil(std::string sIP, std::string sClientID);
		~MQTTUtil();
		void MqttInit();
		void MQTTSubscribe(std::string sTopic, int nQOS);
		MQTTClient &GetClient();
	private:
		MQTTClient m_mqClient;
		MQTTClient_connectOptions m_mqConnOpts = \					
			MQTTClient_connectOptions_initializer;				//初始化参数配置
		std::string m_sIP;
		std::string m_sAddress;
		std::string m_sClientID;
		std::string m_sTopic;
		int m_nQOS;
	private:
		;
};


#endif