#!/usr/bin/python
# -*- coding: UTF-8 -*-

import thread
import time

flags = 0
# Ϊ�̶߳���һ������
def thread1( threadName, delay):
	global flags
	count = 0
	while count < 5:
	  time.sleep(delay)
	  count += 1
	  print "%s: %s" % ( threadName, time.ctime(time.time()) )
	flags = 1
	print "change the flags"

def thread2( threadName):
	global flags
	while (1):
		if (flags):
			print "%s: %s" % ( threadName, time.ctime(time.time()) )
			break
		

# ���������߳�
try:
   thread.start_new_thread( thread1, ("thread1", 2, ) )
   thread.start_new_thread( thread2, ("thread2", ) )
except:
   print "Error: unable to start thread"

while 1:
   pass