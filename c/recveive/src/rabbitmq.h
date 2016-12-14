#ifndef __RABBITMQ_H__
#define __RABBITMQ_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>

#include "utils.h"

/*登录到rabbitmq*/
amqp_connection_state_t LogToRabbitmq(	int n_Channelid,
																				char const *p_cHostName,
																				int n_Port, 
																				char const *p_cLogName,
																				char const *p_cLogPassWord);
																				
/*申明exchange*/
void DeclareExchange(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cExchange,
											char const *P_cExchangeType);
											
	/*申明queue*/
void DeclareQueue(	amqp_connection_state_t conn,
										int n_Channelid,
										char const *p_cQueueName,
										int nPassive,
										int nDurable,
										int nExclusive,
										int nAutoDelete);
										
/*exchange和queue绑定*/
void BlindExchangeQueue(	amqp_connection_state_t conn,
													int n_Channelid,
													char const *p_cQueueName,
													char const *p_cExchange,
													char const *p_cBindingKey);										
/*设置循环读取消息*/	
void RabbitmqConsume(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cQueueName);
											
/*读取消息*/
void ReandMqMessage(	amqp_connection_state_t conn,
											int n_Channelid);
											
/*推送消息*/
void MqMessagePublish(	amqp_connection_state_t conn,
												int nChannelid,
												char const *p_cExchange,
												char const *p_cQueueName,
												char const *p_cMessageBody);
/*处理收到的信息*/
void HandlerMqMessage(	const char *p_cBytes, 
									int nLen);
#endif