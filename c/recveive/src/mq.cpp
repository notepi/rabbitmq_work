#include "rabbitmq.h"

amqp_connection_state_t Mqinit(	int n_Channelid,
																const char *p_cHostName,
																int n_Port,
																const char *p_cExchange,
																const char *p_cQueueName,
																char const *p_cBindingKey,
																char const *p_cExchangeType,
																char const *p_cLogName,
																char const *p_cLogPassWord)
{
	amqp_connection_state_t conn;
	/*��¼��rabbitmq*/
	conn = LogToRabbitmq(	n_Channelid,
												p_cHostName,
												n_Port, 
												p_cLogName,
												p_cLogPassWord);
	
	/*����exchange*/
	DeclareExchange(	conn,
										n_Channelid,
										p_cExchange,
										p_cExchangeType);
	/*����queue*/
	DeclareQueue(	conn,
								n_Channelid,
								p_cQueueName);
	
	/*exchange��queue��*/
	BlindExchangeQueue(	conn,
											n_Channelid,
											p_cQueueName,
											p_cExchange,
											p_cBindingKey);
	/*����ѭ����ȡ��Ϣ*/	
	RabbitmqConsume(	conn,
										n_Channelid,
										p_cQueueName);
	return conn;		
}