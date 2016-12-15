#!/usr/bin/env python  
# -*- coding: UTF-8 -*-
import pika  

cQueue = "A"									#queue name
cRouting_key = cQueue					#Routing_key
cMessage = "m"								#meaasge body
cExchange = "demo"						#exchange name  

connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
channel = connection.channel()  

#channel.exchange_declare(exchange="demo",
#												 auto_delete=False)  
												 
channel.queue_declare(queue=cQueue,
											auto_delete=True)  
											
channel.queue_bind( queue=cQueue,
										exchange=cExchange,
										routing_key=cQueue )
  
print ' [*] Waiting for messages. To exit press CTRL+C'  
  
def callback(ch, method, properties, body):  
    print " [x] Received %r" % (body,)  
  
channel.basic_consume(	callback,  
			queue=cQueue,  
			no_ack=True)  
  
channel.start_consuming()  
