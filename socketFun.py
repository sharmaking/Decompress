#!/usr/bin/python
# -*- coding: utf-8 -*-
# socketFun.py
import struct
import datetime
import copy
import decompress
import time
#socketLink 代表是socket连接类
#--------------------------------
#定义共用成员变量
#--------------------------------
BUFSIZ = 1024
g_currentDate = 0
g_stocks = {}		#股票id与股票代码对应查询字典
#--------------------------------
#发送订阅请求
#--------------------------------
#向服务端发送订阅股票 step-1
def subscibeStock(socketLink, stocks):
	fmt = "iii%ds" %(6*len(stocks))
	sn = 0
	length = 6*len(stocks) + 4
	stocksStr = ""
	for stock in stocks:
		stocksStr = stocksStr + stock
	bytes = struct.pack(fmt, sn, length, len(stocks), stocksStr)
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
#解析发送股票时间
def resolveCurrentDate(bufferData):
	global g_currentDate
	g_currentDate = struct.unpack("i", bufferData[8:12])[0]
	g_currentDate = datetime.datetime.strptime(str(g_currentDate), "%Y%m%d") .date()
#解析股票代码
def resolveStockSecurityCode(bufferData):
	global g_stocks
	stockSecurityCodeNum = struct.unpack("i", bufferData[8:12])[0]
	stockSecurityCode = {
		"nIdnum"			: 0,
		"nType"				: 0,
		"chSecurityCode" 	: "00000000",
		"chSymbol"			: "0000000000000000"
	}
	for x in range(stockSecurityCodeNum):
		stockSecurityCode["nIdnum"], stockSecurityCode["nType"], stockSecurityCode["chSecurityCode"], stockSecurityCode["chSymbol"] = struct.unpack("ii8s16s", bufferData[(12+x*32):(12+(x+1)*32)]) 
		stockSecurityCode["chSecurityCode"] = stockSecurityCode["chSecurityCode"].replace("\x00","")
		stockSecurityCode["chSymbol"] = stockSecurityCode["chSymbol"].replace("\x00","")
		g_stocks[str(stockSecurityCode["nIdnum"])] = copy.copy(stockSecurityCode)
#解析逐笔成交
def resolveTradeSettlement(bufferData):
	p = bufferData[8:]
	nIdnum = struct.unpack("i", p[:4])[0]
	nItems = struct.unpack("i", p[4:8])[0]
	p = p[8:]

	pTransactions = decompress.DecompressTransactionData(p, nItems)
	for pTransaction in pTransactions:
		pTransaction["chSecurityCode"] = g_stocks[str(nIdnum)]["chSecurityCode"]
		pTransaction["nDate"] = g_currentDate
#解析成交队列
def resolveOrderQueue(bufferData):
	p = bufferData[8:]
	nItems = struct.unpack("i", p[0:4])[0]
	p = p[4:]

	pQueues, pIdnums = decompress.DecompressOrderQueueData(p, nItems)
	for i in range(nItems):
		pQueues[i]["chSecurityCode"] = g_stocks[str(pIdnums[i])]["chSecurityCode"]
		pQueues[i]["nDate"] = g_currentDate
#解析股票行情数据
def resolveMarketData(bufferData):
	p = bufferData[8:]
	nItems = struct.unpack("i", p[0:4])[0]
	p = p[4:]

	nSize = 0
	for i in range(nItems):
		nLength, pMarketData, pIdnum = decompress.DecompressMarketData(p[nSize:])
		nSize = nSize + nLength
		pMarketData["chSecurityCode"] = g_stocks[str(pIdnum)]["chSecurityCode"]
		pMarketData["nDate"] = g_currentDate
#解析期货行情数据
def resolveFutureMarketData(bufferData):
	p = bufferData[8:]
	nItems = struct.unpack("i", p[0:4])[0]
	p = p[4:]

	nSize = 0
	for i in range(nItems):
		nLength, pMarketData = decompress.DecompressMarketData_Futures(p[nSize:])
		nSize = nSize + nLength
		pMarketData["chSecurityCode"] = g_stocks[str(pMarketData["nIndex"])]["chSecurityCode"]
		pMarketData["nDate"] = g_currentDate
#解析指数数据
def resolveIndexMarketData(bufferData):
	p = bufferData[8:]
	nItems = struct.unpack("i", p[0:4])[0]
	p = p[4:]

	nSize = 0
	for i in range(nItems):
		nLength, pMarketData = decompress.DecompressIndexData(p[nSize:])
		nSize = nSize + nLength
		pMarketData["chSecurityCode"] = g_stocks[str(pMarketData["nIndex"])]["chSecurityCode"]
		pMarketData["nDate"] = g_currentDate

		print pMarketData
#解析接收的数据类型调用相应的方法
def resolveRecvData(bufferData):
	dataType = struct.unpack("i", bufferData[:4])[0]
	length = struct.unpack("i", bufferData[4:8])[0]
	#解析当前日期
	if dataType == 999:
		resolveCurrentDate(bufferData)
	#解析股票代码
	elif dataType == 0:
		resolveStockSecurityCode(bufferData)
	#解析逐笔成交
	elif dataType == 1:
		resolveTradeSettlement(bufferData)
	#解析成交队列
	elif dataType == 2:
		resolveOrderQueue(bufferData)
	#解析股票行情数据
	elif dataType == 3:
		resolveMarketData(bufferData)
	#解析期货行情数据
	elif dataType == 4:
		resolveFutureMarketData(bufferData)
	#解析指数数据
	elif dataType == 5:
		print "dataType:", dataType
		resolveIndexMarketData(bufferData)
#--------------------------------
#接收解析socket数据，缓存拼接成完整数据
#--------------------------------
#判断数据是否接收完整
def checkBufferDataIsComplete(bufferData):
	if len(bufferData)>8:
		length = struct.unpack("i", bufferData[4:8])[0]
		if len(bufferData) >= (length + 8):
			return True
	return False
#处理接收数据
def handleBufferData(bufferData):
	tempBufferData = bufferData
	#如果缓存区为空直接返回
	while checkBufferDataIsComplete(tempBufferData):
		dataType = struct.unpack("i", tempBufferData[:4])[0]
		length = struct.unpack("i", tempBufferData[4:8])[0]
		completBufferDate = tempBufferData[:length+8]
		resolveRecvData(completBufferDate)
		tempBufferData = tempBufferData[length+8:]
		if len(tempBufferData) < 8:
			break
		pass
	return tempBufferData
#监听socket缓存
def recvSubscibeRespond(socketLink):
	bufferData = ""
	while True:
		recvData = socketLink.recv(BUFSIZ)
		#如果缓冲没有数据
		if not bufferData:
			bufferData = recvData
		else: #继续缓冲数据
			bufferData = bufferData + recvData
		#接收数据完整，处理缓冲数据
		if checkBufferDataIsComplete(bufferData):
			bufferData = handleBufferData(bufferData)