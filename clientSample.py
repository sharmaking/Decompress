#!/usr/bin/python
# -*- coding: utf-8 -*-
import struct
import time
import socketFun
from socket import * 

HOST = '192.168.1.108' 
PORT = 18307 
BUFSIZ = 1024
ADDR = (HOST, PORT)

class TDFDefine_SecurityCode(object):
	def __init__(self):
		super(TDFDefine_SecurityCode, self).__init__()
		nIdnum = 0		
		nType = 0			
		chSecurityCode = "00000000"	
		chSymbol = "0000000000000000"

g_securityCodes = []

#订阅股票
def subscibeStock(socketLink, stocks):
	fmt = "ii%ds" %(6*len(stocks))
	sn = 0
	length = 6*len(stocks)
	stocksStr = ""
	for stock in stocks:
		stocksStr = stocksStr + stock
	bytes = struct.pack(fmt, sn, length, stocksStr)
	#发送订阅代码
	socketLink.send(bytes)

#请求数据
def requestData(socketLink):
	fmt = "iiii"
	sn = 1
	length = 8
	flag = 1 		#0从当前时间发送，1从头开始发送，2从time的时间开始发送
	time = 102323	#10点23分23秒
	bytes = struct.pack(fmt, sn, length, flag, time)
	#发送订阅代码
	socketLink.send(bytes)

#接受订阅返回
def checkRecvDataIsLegal(recvData):
	dataType = struct.unpack("i", recvData[:4])[0]
	legalTypes = [0,1,2,3,4,5,999]
	if dataType in legalTypes:
		return True
	return False

def checkBufferDataIsComplete(bufferData):
	length = struct.unpack("i", bufferData[4:8])[0]
	if len(bufferData) >= (length + 8):
		return True
	return False

#解析接受的完整数据
def resolveRecvData(bufferData):
	dataType = struct.unpack("i", bufferData[:4])[0]
	length = struct.unpack("i", bufferData[4:8])[0]
	print "resolveRecvData:", dataType, length, len(bufferData)
	pass
#接受订阅返回
def recvSubscibeRespond(socketLink):
	bufferData = ""
	while True:
		#time.sleep(5)
		recvData = socketLink.recv(BUFSIZ)#.strip()
		print len(recvData)
		#如果缓冲没有数据，且接受过来的数据合法
		if not bufferData:
			if checkRecvDataIsLegal(recvData):
				bufferData = recvData
		else:
			print struct.unpack("i", bufferData[:4])[0], struct.unpack("i", bufferData[4:8])[0], len(bufferData)
			#继续缓冲数据
			bufferData = bufferData + recvData
		#接收数据完整，处理数据
		if checkBufferDataIsComplete(bufferData):
			resolveRecvData(bufferData)
			bufferData = ""

def main():
	subStock = ["600000", "000001", "300121"]

	tcpCliSock = socket(AF_INET, SOCK_STREAM) 
	tcpCliSock.connect(ADDR)
	time.sleep(5)
	if tcpCliSock:
		#订阅股票
		subscibeStock(tcpCliSock, subStock)
		print "1"
		#请求数据
		print "2"
		requestData(tcpCliSock)

		recvSubscibeRespond(tcpCliSock)

	tcpCliSock.close()
	pass

if __name__ == '__main__':
	main()