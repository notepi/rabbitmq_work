#!/bin/bash
#推送消息，用户名：admin, 密码：admin。topic是sensor，消息是12
mosquitto_pub -u admin -P admin -i 123 -t sensor -m 12
