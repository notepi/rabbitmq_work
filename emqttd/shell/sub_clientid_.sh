#!/bin/bash

echo "send_nomal start!"
echo "-------------------------------"
#打开调试信息，订阅的topic是sensor
mosquitto_sub -i 123456 -d -t sensor


