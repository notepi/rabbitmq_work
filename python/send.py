#!/usr/bin/env python  
# -*- coding: UTF-8 -*-
import pika  

cQueue = "B"									#queue name
cRouting_key = cQueue					#Routing_key
cMessage = "m"								#meaasge body
cExchange = "demo"						#exchange name

#connect to rabbitmq
connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
        
#open rabbit channel
channel = connection.channel()   

#declare queue
#channel.queue_declare(queue='hello',
#											auto_delete=True)  

#publish message 
channel.basic_publish(exchange=cExchange,  
                      routing_key=cRouting_key,  
                      body=cMessage)  
print " [x] Sent 'Hello World!'"  
connection.close()