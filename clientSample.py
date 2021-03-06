#!/usr/bin/python
# -*- coding: utf-8 -*-
import struct
import time
import datetime
import socket
import socketFun
from socket import * 

HOST = '192.168.1.186' 
PORT = 18201 
BUFSIZ = 1024
ADDR = (HOST, PORT)

def main():
	subStock = ["999999"]

	tcpCliSock = socket(AF_INET, SOCK_STREAM) 
	tcpCliSock.connect(ADDR)
	time.sleep(2)
	if tcpCliSock:
		#订阅股票
		socketFun.subscibeStock(tcpCliSock, subStock)
		print "1"
		#请求数据
		socketFun.requestCurrentDay(tcpCliSock, 0, datetime.time(12,01,00))
		print "2"
		#监听数据返回
		socketFun.recvSubscibeRespond(tcpCliSock)

	tcpCliSock.close()
	pass

if __name__ == '__main__':
	main()