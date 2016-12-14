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

/*��¼��rabbitmq*/
amqp_connection_state_t LogToRabbitmq(	int n_Channelid,
																				char const *p_cHostName,
																				int n_Port, 
																				char const *p_cLogName,
																				char const *p_cLogPassWord);
																				
/*����exchange*/
void DeclareExchange(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cExchange,
											char const *P_cExchangeType);
											
	/*����queue*/
void DeclareQueue(	amqp_connection_state_t conn,
										int n_Channelid,
										char const *p_cQueueName,
										int nPassive,
										int nDurable,
										int nExclusive,
										int nAutoDelete);
										
/*exchange��queue��*/
void BlindExchangeQueue(	amqp_connection_state_t conn,
													int n_Channelid,
													char const *p_cQueueName,
													char const *p_cExchange,
													char const *p_cBindingKey);										
/*����ѭ����ȡ��Ϣ*/	
void RabbitmqConsume(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cQueueName);
											
/*��ȡ��Ϣ*/
void ReandMqMessage(	amqp_connection_state_t conn,
											int n_Channelid);
											
/*������Ϣ*/
void MqMessagePublish(	amqp_connection_state_t conn,
												int nChannelid,
												char const *p_cExchange,
												char const *p_cQueueName,
												char const *p_cMessageBody);
/*�����յ�����Ϣ*/
void HandlerMqMessage(	const char *p_cBytes, 
									int nLen);
#endif