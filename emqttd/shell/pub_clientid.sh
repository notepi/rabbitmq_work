#!/bin/bash
#推送消息，topic是sensor，消息是12
mosquitto_pub -i 123 -t sensor -m 12
