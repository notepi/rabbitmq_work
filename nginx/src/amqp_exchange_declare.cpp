#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>

#include <utils.h>

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
	conn = amqp_new_connection();
	/*打开链接*/
	die_on_error(	sockfd = amqp_open_socket(	hostname,	//[in]hostname 
																						port),		//[in]port 
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
	/*申明exchange*/
	{
	  amqp_exchange_declare(	conn,																			//[in] state connection state  
	  												channelid,																//[in] channel the channel to do the RPC on
	  												amqp_cstring_bytes(exchange),							//[in] exchange exchange   
	  												amqp_cstring_bytes(exchangetype),					//[in] type type  
														0,																				//[in] passive passive
														0,																				//[in] durable durable  
														0,																				//[in] auto_delete auto_delete
														0,																				//[in] internal internal  
														amqp_empty_table);												//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring exchange");
	}
	/*申明queue*/
	{
		amqp_queue_declare(	conn,																						//[in] state connection state  
												channelid,																			//[in] channel the channel to do the RPC on  
												amqp_cstring_bytes(queuename),									//[in] queue queue
												0,																							//[in] passive passive
												1,																							//[in] durable durable
												0,																							//[in] exclusive exclusive   
												0,																							//[in] auto_delete auto_delete 
												amqp_empty_table);															//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
	}										
	/*exchange和queue绑定*/
	{
	  amqp_queue_bind(	conn,																							//[in] state connection state   
	  									channelid,																				//[in] channel the channel to do the RPC on  
											amqp_cstring_bytes(queuename),										//[in] queue queue  											
											amqp_cstring_bytes(exchange),											//[in] exchange exchange  
											amqp_cstring_bytes(bindingkey),										//[in] routing_key routing_key  
											amqp_empty_table);																//[in] arguments arguments  
	  die_on_amqp_error(amqp_get_rpc_reply(conn), "Unbinding");
	}

	/*预取消息的条数*/
	amqp_basic_qos(	conn,																									//[in] state connection state  
									channelid,																						//[in] channel the channel to do the RPC on
									0,																										//[in] prefetch_size prefetch_size 
									1,																										//[in] prefetch_count prefetch_count  
									0);																										//[in] global global
	
	/*循环获取消息*/
	{								
		amqp_basic_consume(	conn,																						//[in] state connection state  
												channelid,																			//[in] channel the channel to do the RPC on  
												amqp_cstring_bytes(queuename),									//[in] queue queue  
												amqp_empty_bytes,																//[in] consumer_tag consumer_tag 
												0,																							//[in] no_local no_local  
												0,																							//[in] no_ack 
												0,																							//[in] exclusive exclusive  
												amqp_empty_table);															//[in] arguments arguments  
		die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
	}
	/*读取消息*/
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
				assert(body_received <= body_target);

				int i;
				for(i = 0; i<frame.payload.body_fragment.len; i++)
				{
					printf("%c",*((char*)frame.payload.body_fragment.bytes+i));
				}
				printf("\n");

			}

			if (body_received != body_target) {
				/* Can only happen when amqp_simple_wait_frame returns <= 0 */
				/* We break here to close the connection */
				break;
			}
			/* do something */
			/*手动ack*/
			amqp_basic_ack(conn,channelid,d->delivery_tag,0);
		}
	}
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");

	return 0;
}
