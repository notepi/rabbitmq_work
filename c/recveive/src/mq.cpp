#include "rabbitmq.h"

amqp_connection_state_t Mqinit(	int nChannelid,
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
	/*登录到rabbitmq*/
	conn = LogToRabbitmq(	nChannelid,
												p_cHostName,
												n_Port, 
												p_cLogName,
												p_cLogPassWord);
	
	/*申明exchange*/
	DeclareExchange(	conn,
										nChannelid,
										p_cExchange,
										p_cExchangeType,
										0,//nPassive
										0,//nDurable
										0,//nExclusive
										0);//nAutoDelete
	/*申明queue*/
	DeclareQueue(	conn,
								nChannelid,
								p_cQueueName,
								0,//nPassive
								1,//nDurable
								0,//nExclusive
								0);//nAutoDelete
	
	/*exchange和queue绑定*/
	BlindExchangeQueue(	conn,
											nChannelid,
											p_cQueueName,
											p_cExchange,
											p_cBindingKey);
	/*设置循环读取消息*/	
	RabbitmqConsume(	conn,
										nChannelid,
										p_cQueueName);
	return conn;		
}

void	HandlerMqMessage(const char *p_cBytes, int nLen)
	{
		char *p_cBuffer;		
		p_cBuffer = (char *)malloc(sizeof(char) * (nLen + 1));
		bcopy(p_cBytes, p_cBuffer, nLen);
		p_cBuffer[nLen] = 0x00;
		printf("receive:%s\n", p_cBuffer);
		free(p_cBuffer);
	}