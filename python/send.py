#!/usr/bin/env python  
import pika  
  
connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
channel = connection.channel()  
  
#channel.queue_declare(queue='hello',
#											auto_delete=True)  
  
channel.basic_publish(exchange='hello-exchange',  
                      routing_key='hello',  
                      body='Hello World!')  
print " [x] Sent 'Hello World!'"  
connection.close()