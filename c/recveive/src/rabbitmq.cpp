#include "rabbitmq.h"
#include <string.h>
/*登录到rabbitmq*/
amqp_connection_state_t LogToRabbitmq(	int n_Channelid,
																				char const *p_cHostName,
																				int n_Port, 
																				char const *p_cLogName,
																				char const *p_cLogPassWord)
	{
		amqp_connection_state_t conn;
		int n_SockFd;
		conn = amqp_new_connection();
		/*打开链接*/
		die_on_error(	n_SockFd = amqp_open_socket(	p_cHostName,						//[in]hostname 
																								n_Port),								//[in]port 
									"Opening socket");
		amqp_set_sockfd(conn, n_SockFd);
		/*登录*/
		die_on_amqp_error(amqp_login(	conn,																	//[in] state the connection object  
																	"/",																	//[in] vhost the virtual host to connect to on the broker. 
																																				//The default on most brokers is "/"  
																	n_Channelid,													//[in] channel_max the limit for number of channels for the connection. 
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
																	p_cLogName,														//[in]username
																	p_cLogPassWord),											//[in]password
											"Logging in");
		amqp_channel_open(	conn,																						//[in] state connection state
												n_Channelid);																		//[in] channel the channel to do the RPC on
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");
		return conn;
	}

/*申明exchange*/
void DeclareExchange(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cExchange,
											char const *P_cExchangeType)
	{
	  amqp_exchange_declare(	conn,																			//[in] state connection state  
	  												n_Channelid,															//[in] channel the channel to do the RPC on
	  												amqp_cstring_bytes(p_cExchange),					//[in] exchange exchange   
	  												amqp_cstring_bytes(P_cExchangeType),			//[in] type type  
														0,																				//[in] passive passive
														0,																				//[in] durable durable  
														0,																				//[in] auto_delete auto_delete
														0,																				//[in] internal internal  
														amqp_empty_table);												//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring exchange");
	}

///*申明queue*/
//void DeclareQueue(	amqp_connection_state_t conn,
//										int n_Channelid,
//										char const *p_cQueueName
//										int nAutoDelete)
//	{
//		amqp_queue_declare(	conn,																						//[in] state connection state  
//												n_Channelid,																		//[in] channel the channel to do the RPC on  
//												amqp_cstring_bytes(p_cQueueName),								//[in] queue queue
//												0,																							//[in] passive passive
//												1,																							//[in] durable durable
//												0,																							//[in] exclusive exclusive   
//												0,																							//[in] auto_delete auto_delete 
//												amqp_empty_table);															//[in] arguments arguments  
//		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
//	}	
	
	/*申明queue*/
void DeclareQueue(	amqp_connection_state_t conn,
										int n_Channelid,
										char const *p_cQueueName,
										int nPassive,
										int nDurable,
										int nExclusive,
										int nAutoDelete)
	{
		amqp_queue_declare(	conn,																						//[in] state connection state  
												n_Channelid,																		//[in] channel the channel to do the RPC on  
												amqp_cstring_bytes(p_cQueueName),								//[in] queue queue
												nPassive,																				//[in] passive passive
												nDurable,																				//[in] durable durable
												nExclusive,																			//[in] exclusive exclusive   
												nAutoDelete,																		//[in] auto_delete auto_delete 
												amqp_empty_table);															//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
	}	

/*exchange和queue绑定*/
void BlindExchangeQueue(	amqp_connection_state_t conn,
													int n_Channelid,
													char const *p_cQueueName,
													char const *p_cExchange,
													char const *p_cBindingKey)
	{
	  amqp_queue_bind(	conn,																							//[in] state connection state   
	  									n_Channelid,																			//[in] channel the channel to do the RPC on  
											amqp_cstring_bytes(p_cQueueName),									//[in] queue queue  											
											amqp_cstring_bytes(p_cExchange),									//[in] exchange exchange  
											amqp_cstring_bytes(p_cBindingKey),								//[in] routing_key routing_key  
											amqp_empty_table);																//[in] arguments arguments  
	  die_on_amqp_error(amqp_get_rpc_reply(conn), "Unbinding");
	}

/*设置循环读取消息*/	
void RabbitmqConsume(	amqp_connection_state_t conn,
											int n_Channelid,
											char const *p_cQueueName)
{
	/*预取消息的条数*/
	amqp_basic_qos(	conn,																									//[in] state connection state  
									n_Channelid,																					//[in] channel the channel to do the RPC on
									0,																										//[in] prefetch_size prefetch_size 
									1,																										//[in] prefetch_count prefetch_count  
									0);																										//[in] global global
	
	/*循环获取消息*/
	{								
		amqp_basic_consume(	conn,																						//[in] state connection state  
												n_Channelid,																		//[in] channel the channel to do the RPC on  
												amqp_cstring_bytes(p_cQueueName),								//[in] queue queue  
												amqp_empty_bytes,																//[in] consumer_tag consumer_tag 
												0,																							//[in] no_local no_local  
												0,																							//[in] no_ack 
												0,																							//[in] exclusive exclusive  
												amqp_empty_table);															//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
	}
}

/*读取消息*/
void ReandMqMessage(	amqp_connection_state_t conn,
											int n_Channelid)
	{
		amqp_frame_t frame;
		int result;
		amqp_basic_deliver_t *d;
		amqp_basic_properties_t *p;
		size_t body_target;
		size_t body_received;

		while (1) {
			/*释放conn的内存*/
			amqp_maybe_release_buffers(conn);
			/*第一次读取*/
			result = amqp_simple_wait_frame(conn, &frame);
			printf("Result %d\n", result);
			if (result < 0){
			break;}

			printf("Frame type %d, channel %d\n", frame.frame_type, frame.channel);
			if (frame.frame_type != AMQP_FRAME_METHOD)
				continue;

			printf("Method %s\n", amqp_method_name(frame.payload.method.id));
			if (frame.payload.method.id != AMQP_BASIC_DELIVER_METHOD)
				continue;

			d = (amqp_basic_deliver_t *) frame.payload.method.decoded;
			printf("Delivery %u, exchange %.*s routingkey %.*s\n",(unsigned) d->delivery_tag,
				(int) d->exchange.len, (char *) d->exchange.bytes,
				(int) d->routing_key.len, (char *) d->routing_key.bytes);

			/*第二次读取*/
			result = amqp_simple_wait_frame(conn, &frame);
			if (result < 0)
				break;

			if (frame.frame_type != AMQP_FRAME_HEADER) {
				fprintf(stderr, "Expected header!");
				break;
			}

			p = (amqp_basic_properties_t *) frame.payload.properties.decoded;
			if (p->_flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
				printf("Content-type: %.*s\n",
				(int) p->content_type.len, (char *) p->content_type.bytes);
			}


			body_target = frame.payload.properties.body_size;
			body_received = 0;

			while (body_received < body_target) {
				/*第三次读取，读取消息体*/
				result = amqp_simple_wait_frame(conn, &frame);
				if (result < 0)
					break;

				if (frame.frame_type != AMQP_FRAME_BODY) {
					fprintf(stderr, "Expected body!");
					break;//进程断掉
				}
				printf("ok!\n");

				body_received += frame.payload.body_fragment.len;
				if(body_received > body_target){//消息数据不对
					break;
				};

//				int i;
//				for(i = 0; i<frame.payload.body_fragment.len; i++)
//				{
//					printf("%c",*((char*)frame.payload.body_fragment.bytes+i));
//				}
//				printf("\n");
				HandlerMqMessage(	(const char*)(frame.payload.body_fragment.bytes), 
													frame.payload.body_fragment.len);

			}

			if (body_received != body_target) {
				/* Can only happen when amqp_simple_wait_frame returns <= 0 */
				/* We break here to close the connection */
				break;
			}
			/* do something */
			/*手动ack*/
			amqp_basic_ack(conn, n_Channelid, d->delivery_tag,0);
		}
	}
void MqMessagePublish(	amqp_connection_state_t conn,
												int nChannelid,
												char const *p_cExchange,
												char const *p_cQueueName,
												char const *p_cMessageBody)
	{
		amqp_basic_properties_t props;
		props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
		/*props.content_type = amqp_cstring_bytes("text/plain");*/
		props.delivery_mode = 2; /* persistent delivery mode */
		
		/*推送消息*/
		die_on_error(	amqp_basic_publish(	conn,																//[in] state the connection object  
																			nChannelid,													//[in] channel the channel identifier  
																			amqp_cstring_bytes(p_cExchange),		//[in] exchange the exchange on the broker to publish to  
																			amqp_cstring_bytes(p_cQueueName),		//[in] routing_key the routing key to use when publishing the message  
																			0,																	//[in] mandatory indicate to the broker that the message MUST be routed to a queue. 
																																					//If the broker cannot do this it should respond with a basic.return method.  
																			0,																	//[in] immediate indicate to the broker that the message MUST be delivered to a 
																																					//consumer immediately. If the broker cannot do this it should response with a basic.return method.  
																			NULL,																//[in] properties the properties associated with the message  
																			amqp_cstring_bytes(p_cMessageBody)),//[in] body the message body  
									"Publishing");
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