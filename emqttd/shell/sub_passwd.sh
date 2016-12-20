#!/bin/bash
#订阅消息，用户名：admin, 密码：admin。topic是sensor
mosquitto_sub -u admin -P admin -i 123 -t sensor
