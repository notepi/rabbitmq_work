#!/usr/bin/env python  
# -*- coding: UTF-8 -*-

import pika  
import thread
import time

gg_nFlags = 0
# 为线程定义一个函数  
def threadA(threadName):
	connection = pika.BlockingConnection(pika.ConnectionParameters(  
	        host='localhost'))  
	channel = connection.channel()  

	channel.exchange_declare(exchange="exchange",
													 auto_delete=False)  
													 
	channel.queue_declare(queue='A',
												auto_delete=True)  
												
	channel.queue_bind( queue="A",
											exchange="exchange",
											routing_key="A" )
	  
	print ' [Ac] Waiting for messages. To exit press CTRL+C'  
	  
	def callback(ch, method, properties, body):  
	    print " [Ac] Received %r" % (body,)
  
	channel.basic_consume(callback,  
	                      queue='A',  
	                      no_ack=True)  
	  
	channel.start_consuming()  
	
# 为线程定义一个函数  
def threadB(threadName):
	connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
	channel = connection.channel()  
	while(1):
		channel.basic_publish(exchange='exchange',  
		                      routing_key='B',  
		                      body='m')  
		print " [Bp] Sent 'm'"
		time.sleep(5)  
	connection.close()
	
# 创建两个线程
try:
   thread.start_new_thread( threadA, ("threadA", ) )
   thread.start_new_thread( threadB, ("threadB", ) )
except:
   print "Error: unable to start thread"

while 1:
   pass
	
