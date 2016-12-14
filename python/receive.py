#!/usr/bin/env python  
import pika  
  
connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
channel = connection.channel()  

channel.exchange_declare(exchange="demo",
												 auto_delete=False)  
												 
#channel.queue_declare(queue='hello',
#											auto_delete=True)  
											
#channel.queue_bind( queue="hello",
#										exchange="exchange",
#										routing_key="hello" )
#  
#print ' [*] Waiting for messages. To exit press CTRL+C'  
#  
#def callback(ch, method, properties, body):  
#    print " [x] Received %r" % (body,)  
#  
#channel.basic_consume(callback,  
#                      queue='hello',  
#                      no_ack=True)  
#  
#channel.start_consuming()  
