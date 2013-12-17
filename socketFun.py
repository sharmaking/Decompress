#!/usr/bin/python
# -*- coding: utf-8 -*-
# socketFun.py
import struct
import datetime
import copy
BUFSIZ = 1024
#socketLink 代表是socket连接类
#--------------------------------
#发送订阅请求
#--------------------------------
#向服务端发送订阅股票 step-1
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
#向服务端发送订阅请求时间段
#请求当天数据
def requestCurrentDay(socketLink, flag, time):  #0从当前时间发送，1从头开始发送，2从time的时间开始发送
	fmt = "iiii"
	sn = 1
	length = 8
	time = int(time.strftime("%H%M%S"))     #10点23分23秒
	bytes = struct.pack(fmt, sn, length, flag, time)
	#发送订阅代码
	socketLink.send(bytes)
#请求一天历史
def requestOneDay(socketLink, date, time1, time2):
	fmt = "iiiii"
	sn = 2
	length = 12
	date = int(date.strftime("%Y%m%d"))
	time1 = int(time1.strftime("%H%M%S"))	#10点23分23秒
	time2 = int(time2.strftime("%H%M%S"))	#10点23分23秒
	bytes = struct.pack(fmt, sn, length, date, time1, time2)
	#发送订阅代码
	socketLink.send(bytes)
#请求一段历史
def requestSomeTimes(socketLink, date1, date2):
	fmt = "iiii"
	sn = 1
	length = 8
	date1 = int(date.strftime("%Y%m%d"))     #10点23分23秒
	date2 = int(date.strftime("%Y%m%d"))
	bytes = struct.pack(fmt, sn, length, date1, date2)
	#发送订阅代码
	socketLink.send(bytes)
#--------------------------------
#接收解析socket完整缓存数据
#--------------------------------
#解析股票代码
def resolveStockSecurityCode(bufferData, stockSecurityCodes):	#stockSecurityCodes为一list
	stockSecurityCodeNum = struct.unpack("i", bufferData[8:12])[0]
	stockSecurityCode = {
		"nIdnum"			: 0,
		"nType"				: 0,
		"chSecurityCode" 	: "00000000",
		"chSymbol"			: "0000000000000000"
	}
	for x in range(stockSecurityCodeNum):
		stockSecurityCode["nIdnum"], stockSecurityCode["nType"], stockSecurityCode["chSecurityCode"], stockSecurityCode["chSymbol"] = struct.unpack("ii8s16s", bufferData[(12+x*32):(12+(x+1)*32)]) 
		stockSecurityCode["chSecurityCode"] = stockSecurityCode["chSecurityCode"].strip()
		stockSecurityCode["chSymbol"] = stockSecurityCode["chSymbol"].strip()
		stockSecurityCodes.append(copy.copy(stockSecurityCode))
#解析逐笔成交
def resolveTradeSettlement(bufferData, tradeSettlements):		#tradeSettlements为一list
	nItems = struct.unpack("i", bufferData[12:16])[0]
	pass

#解析接收的数据类型调用相应的方法
def resolveRecvData(bufferData):
	dataType = struct.unpack("i", bufferData[:4])[0]
	length = struct.unpack("i", bufferData[4:8])[0]
	#print "resolveRecvData:", dataType, length, len(bufferData)
	if dataType == 0:
		stockSecurityCodes = []
		resolveStockSecurityCode(bufferData,stockSecurityCodes)
		print stockSecurityCodes[0], stockSecurityCodes[0]["chSymbol"], len(stockSecurityCodes)
		print stockSecurityCodes[-1], stockSecurityCodes[-1]["chSymbol"], len(stockSecurityCodes)
	pass
#--------------------------------
#接收解析socket数据，缓存拼接成完整数据
#--------------------------------
#判断数据是否合法
def checkRecvDataIsLegal(recvData):
	dataType = struct.unpack("i", recvData[:4])[0]
	legalTypes = [0,1,2,3,4,5,999]
	if dataType in legalTypes:
		return True
	return False
#判断数据是否接收完整
def checkBufferDataIsComplete(bufferData):
	if bufferData:
		length = struct.unpack("i", bufferData[4:8])[0]
		if len(bufferData) >= (length + 8):
			return True
	return False
#监听socket缓存
def recvSubscibeRespond(socketLink):
	bufferData = ""
	while True:
		recvData = socketLink.recv(BUFSIZ)#.strip()
		#如果缓冲没有数据，且接受过来的数据合法
		if not bufferData:
			if checkRecvDataIsLegal(recvData):
				bufferData = recvData
		else:
			#继续缓冲数据
			bufferData = bufferData + recvData
		#接收数据完整，处理数据
		if checkBufferDataIsComplete(bufferData):
			resolveRecvData(bufferData)
			bufferData = ""