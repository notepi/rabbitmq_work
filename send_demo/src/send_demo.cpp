#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "utils.h"

int main(int argc, const char **argv) {

	const char *hostname;
	int port;
	const char *exchange;
	const char *queuename;
	const char *messagebody;
  char const *bindingkey;

	hostname = "localhost";
	port = 5672;
	exchange = "test1";
	queuename = "test1";
	bindingkey = queuename;
	messagebody = "hello world";

	int sockfd;
	int channelid = 1;
	amqp_connection_state_t conn;
	conn = amqp_new_connection();
	/*打开链接*/
	die_on_error(	sockfd = amqp_open_socket(	hostname,									//[in]hostname 
																						port),										//[in]port 
								"Opening socket");
	amqp_set_sockfd(conn, sockfd);
	/*登录*/
	die_on_amqp_error(amqp_login(	conn,																	//[in] state the connection object  
																"/",																	//[in] vhost the virtual host to connect to on the broker. 
																																			//The default on most brokers is "/"  
																channelid,														//[in] channel_max the limit for number of channels for the connection. 
																																			//0 means no limit, and is a good default (AMQP_DEFAULT_MAX_CHANNELS) 
																																			//Note that the maximum number of channels the protocol supports is 
																																			//65535 (2^16, with the 0-channel reserved). The server can set a lower channel_max 
																																			//and then the client will use the lowest of the two  
																131072,																//[in] frame_max the maximum size of an AMQP frame on the wire to request of the broker
																																			//for this connection. 4096 is the minimum size, 2^31-1 is the maximum, a good default is 131072 (128KB), 
																																			//or AMQP_DEFAULT_FRAME_SIZE  
																0,																		//[in] heartbeat the number of seconds between heartbeat frames to request of the broker. 
																																			//A value of 0 disables heartbeats. Note rabbitmq-c only has partial support for heartbeats, 
																																			//as of v0.4.0 they are only serviced during amqp_basic_publish() and amqp_simple_wait_frame()/amqp_simple_wait_frame_noblock()  
																AMQP_SASL_METHOD_PLAIN,								//[in] sasl_method the SASL method to authenticate with the broker. followed by the authentication information. For AMQP_SASL_METHOD_PLAIN, 
																																			//the AMQP_SASL_METHOD_PLAIN should be followed by two arguments in this order: 
																																			//const char* username, and const char* password.  
																"guest",															//[in]username
																"guest"),															//[in]password
										"Logging in");
	amqp_channel_open(	conn,																						//[in] state connection state
											channelid);																			//[in] channel the channel to do the RPC on
	die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

//	/*申明queue*/
//	{
//		amqp_queue_declare(	conn,																						//[in] state connection state  
//												channelid,																			//[in] channel the channel to do the RPC on  
//												amqp_cstring_bytes(queuename),									//[in] queue queue
//												0,																							//[in] passive passive
//												1,																							//[in] durable durable
//												0,																							//[in] exclusive exclusive   
//												0,																							//[in] auto_delete auto_delete 
//												amqp_empty_table);															//[in] arguments arguments  
//		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
//	}		

	{
		amqp_basic_properties_t props;
		props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
		/*props.content_type = amqp_cstring_bytes("text/plain");*/
		props.delivery_mode = 2; /* persistent delivery mode */
		
		/*推送消息*/
		die_on_error(	amqp_basic_publish(	conn,																//[in] state the connection object  
																			channelid,													//[in] channel the channel identifier  
																			amqp_cstring_bytes(exchange),				//[in] exchange the exchange on the broker to publish to  
																			amqp_cstring_bytes(queuename),			//[in] routing_key the routing key to use when publishing the message  
																			0,																	//[in] mandatory indicate to the broker that the message MUST be routed to a queue. 
																																					//If the broker cannot do this it should respond with a basic.return method.  
																			0,																	//[in] immediate indicate to the broker that the message MUST be delivered to a 
																																					//consumer immediately. If the broker cannot do this it should response with a basic.return method.  
																			NULL,																//[in] properties the properties associated with the message  
																			amqp_cstring_bytes(messagebody)),		//[in] body the message body  
									"Publishing");
	}
	printf("end!\n");
	/*关闭channel*/
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	/*关闭链接*/
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	/*销毁链接*/
	die_on_error(amqp_destroy_connection(conn), "Ending connection");
	return 0;
}
