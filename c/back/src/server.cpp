#include <stdio.h>
#include <libgen.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>

#include "epoll.h"
#include "IPv4socket.h"
#include "signal.h"
#include "rabbitmq.h"
#include "mq.h"
#include <iostream>

using namespace std;
//
//void OnSIGALRMHandler(int sig)
//{
//	printf("time up!\n");
//	alarm(2);
//}
//void *thread_A(void *arg)
//{
//	printf("%s\n", arg);
//}
//
//void *thread_B(void *arg)
//{
//	printf("%s\n", arg);
//}
//int main(int argc, char *argv[])
//{
//	char *p_cTA = "thread_A";
//	char *p_cTB = "thread_B";
//	pthread_t pt_PthreadID;
//	SigSet(SIGALRM, OnSIGALRMHandler);
//	alarm(2);
//
//	/*�����߳�A*/
//	pthread_create(	&pt_PthreadID, 
//									NULL,
//									thread_A,
//									(void *)p_cTA);
//									
//	/*�����߳�B*/
//	pthread_create(	&pt_PthreadID, 
//									NULL,
//									thread_B,
//									(void *)p_cTB);
//	while(1)
//	{
//		
//	}
//	
//}

int main(int argc, const char **argv) {

	const char *p_cHostName;
	int nPort;
	const char *p_cExchange;
	const char *p_cQueueName;
	char const *p_cBindingKey;
	char const *p_cExchangeType;
	
	p_cHostName = "localhost";
	nPort = 5672;
	p_cExchange = "test1";
	p_cQueueName = "test1";
	p_cBindingKey = p_cQueueName;
	p_cExchangeType = "direct";
	
	int sockfd;
	int nChannelid = 1;
	amqp_connection_state_t conn;

	conn =  Mqinit(	nChannelid,
									p_cHostName,
									nPort,
									p_cExchange,
									p_cQueueName,
									p_cBindingKey,
									p_cExchangeType,
									"guest",
									"guest");
//	/*��ȡ��Ϣ*/
//	ReandMqMessage(	conn,
//									channelid);
	MqMessagePublish(	conn,
										nChannelid,
										p_cExchange,
										p_cQueueName,
										"asdfas");
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");

	return 0;
}