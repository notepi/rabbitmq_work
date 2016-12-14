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
////	char *p_cTA = "thread_A";
////	char *p_cTB = "thread_B";
////	pthread_t pt_PthreadID;
////	SigSet(SIGALRM, OnSIGALRMHandler);
////	alarm(2);
////
////	/*创建线程A*/
////	pthread_create(	&pt_PthreadID, 
////									NULL,
////									thread_A,
////									(void *)p_cTA);
////									
////	/*创建线程B*/
////	pthread_create(	&pt_PthreadID, 
////									NULL,
////									thread_B,
////									(void *)p_cTB);
////	while(1)
////	{
////		
////	}
//	const char *p_cP_cHostName;
//	int n_n_Port;
//	const char *p_cp_cExchange;
//	const char *p_cp_cQueueName;
//	char const *p_cp_cBindingKey,;
//	char const *p_cp_cExchangeType;
//	char const *p_cLogName;
//	char const *p_cLogPassWord;
//	int n_Channelid = 0;
//	
//	p_cP_cHostName = "localhost";
//	n_n_Port = 5672;
//	p_cp_cExchange = "test1";
//	p_cp_cQueueName = "test1";
//	p_cp_cBindingKey, = p_cp_cQueueName;
//	p_cp_cExchangeType = "direct";
//	p_cLogName = "guest";
//	p_cLogPassWord = "guest";
//	amqp_connection_state_t conn;
//	conn = Mqinit(	n_Channelid,
//									p_cHostName,
//									n_Port,
//									p_cExchange,
//									p_cQueueName,
//									p_cBindingKey,
//									p_cExchangeType,
//									p_cLogName,
//									p_cLogPassWord);
//	/*设置循环读取消息*/	
//	RabbitmqConsume(	conn,
//										n_Channelid,
//										p_cp_cQueueName);
//	/*读取消息*/
//	ReandMqMessage(	conn,
//									n_Channelid);
//	
//}

int main(int argc, const char **argv) {

	const char *hostname;
	int port;
	const char *exchange;
	const char *queuename;
	char const *bindingkey;
	char const *exchangetype;
	
	hostname = "localhost";
	port = 5672;
	exchange = "test1";
	queuename = "test1";
	bindingkey = queuename;
	exchangetype = "direct";
	
	int sockfd;
	int channelid = 1;
	amqp_connection_state_t conn;

//	conn = LogToRabbitmq(	channelid,
//												hostname,
//												port,
//												"guest",
//												"guest");
//	cout <<"ok1!" << endl;
//
//	/*申明exchange*/
//	DeclareExchange(	conn,
//										channelid,
//										exchange,
//										exchangetype);
//	cout <<"ok2!" << endl;	
//	/*申明queue*/
//	DeclareQueue(	conn,
//								channelid,
//								queuename);							
//	/*exchange和queue绑定*/
//	BlindExchangeQueue(	conn,
//											channelid,
//											queuename,
//											exchange,
//											bindingkey);
//	/*设置循环读取消息*/	
//	RabbitmqConsume(	conn,
//										channelid,
//										queuename);		
conn =  Mqinit(	channelid,
								hostname,
								port,
								exchange,
								queuename,
								bindingkey,
								exchangetype,
								"guest",
								"guest");
	/*读取消息*/
	ReandMqMessage(	conn,
									channelid);
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");

	return 0;
}