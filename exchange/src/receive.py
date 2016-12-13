#!/usr/bin/env python  
# -*- coding: UTF-8 -*-

import pika  
import thread
import time
import threading

gg_nFlags = 0

# 为线程定义一个函数  
def threadB(threadName):
	global gg_nFlags
	connection = pika.BlockingConnection(pika.ConnectionParameters(  
	        host='localhost'))  
	channel = connection.channel()  

	channel.exchange_declare(exchange="exchange",
	                         auto_delete=False)  
	                         
	channel.queue_declare(queue='B',
	                      auto_delete=True)  
	                      
	channel.queue_bind(queue="B",
	                   exchange="exchange",
	                   routing_key="B")
	                   
	print ' [Bc] Waiting for messages. To exit press CTRL+C'  
	  
	def callback(ch, method, properties, body):
	    global gg_nFlags  
	    print " [Bc] Received %r" % (body,)
	    if (body == 'm'):
	      gg_nFlags = 1
	      
	      def func():
	        global gg_nFlags 
	        print 'hello timer!'
	        gg_nFlags=0
	        
	      timer = threading.Timer(30, func)
	      timer.start()
	      
	channel.basic_consume(callback,  
	                      queue='B',  
	                      no_ack=True)  
	  
	channel.start_consuming()  
	
# 为线程定义一个函数  
def threadA(threadName):
	global gg_nFlags
	connection = pika.BlockingConnection(pika.ConnectionParameters(  
        host='localhost'))  
	channel = connection.channel()  
	while(1):
	  if(gg_nFlags):  
	    channel.basic_publish(exchange='exchange',  
	                          routing_key='A',  
	                          body='Hello World!')  
	    print " [Ap] Sent 'Hello World!'"
	  time.sleep(5) 
	  print "value is :%d" % gg_nFlags
	connection.close()
	
# 创建两个线程
try:
   thread.start_new_thread( threadA, ("threadA", ) )
   thread.start_new_thread( threadB, ("threadB", ) )
except:
   print "Error: unable to start thread"
   


while 1:
   pass
	
