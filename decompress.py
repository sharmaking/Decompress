#!/usr/bin/python
# -*- coding: utf-8 -*-
# decompress.py
import dataStruct
import copy
import ctypes
import datetime

api = ctypes.windll.LoadLibrary("./decompress.dll")
#解压逐笔成交数据
def DecompressTransactionData(p, nItems):
	pTransactions = dataStruct.getTransactions(nItems)
	nSize = 0
	iData = ctypes.c_longlong(0)
	nPreTime = 0
	nPreIndex = 0
	nPrePrice = 0
	for i in range(nItems):
		#成交时间
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pTransactions[i]["nTime"] = nPreTime + iData.value 
		nPreTime = pTransactions[i]["nTime"]
		pTransactions[i]["nTime"] = datetime.datetime.strptime(str(nPreTime), "%H%M%S%f").time()
		#成交序号
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		nPreIndex = nPreIndex + int(iData.value)
		pTransactions[i]["nIndex"] = nPreIndex
		#成交价格
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		nPrePrice = nPrePrice + int(iData.value)
		pTransactions[i]["nPrice"] = round(float(nPrePrice)/10000,2)
		#成交数量
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pTransactions[i]["nVolume"] = int(round(iData.value,-2)/100)

		pTransactions[i]["nTurnover"] = pTransactions[i]["nPrice"] * pTransactions[i]["nVolume"] * 100
	return pTransactions
#解压成交队列
def DecompressOrderQueueData(p, nItems):
	pQueues, pIdnums = dataStruct.getOrderQueue(nItems)
	nSize = 0
	iData = ctypes.c_longlong(0)
	for i in range(nItems):
		#本日编号
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pIdnums[i] = iData.value
		#订单编号
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pQueues[i]["nTime"] = datetime.datetime.strptime(str(iData.value), "%H%M%S%f").time()
		#买卖方向(A:Ask, B:Bid)
		pQueues[i]["nSide"] = p[nSize]
		nSize = nSize + 1
		#订单价格
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pQueues[i]["nPrice"] = round(float(iData.value)/10000,2)
		#订单数量
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pQueues[i]["nOrders"] = int(iData.value)
		#队列个数
		nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))
		pQueues[i]["nABItems"] = int(iData.value)
		#订单数量
		nABVolume = []
		for k in range(pQueues[i]["nABItems"]):
			nSize = nSize + api.decompressData(ctypes.addressof(iData), ctypes.c_char_p(p[nSize:]))	
			nABVolume.append(int(round(iData.value, -2)/100))
		pQueues[i]["nABVolume"] = nABVolume
	return pQueues, pIdnums
"""
#解压行情数据
def DecompressMarketData(pMarketData, p, pIdnum):
	nSize = 0
	iData = np.int64(0)

	nSize =  decompressData(iData, p[nSize:])
	pIdnum = iData
	#状态
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nStatus"]	= iData
	#时间
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nTime"]	= iData
	#昨收
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nPreClose"] = iData
	#开盘价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nOpen"] = iData + pMarketData["nPreClose"]
	#最高价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nHigh"] = iData + pMarketData["nPreClose"]
	#最低价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nLow"]  = iData + pMarketData["nPreClose"]
	#最新价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nMatch"]= iData + pMarketData["nPreClose"]
	#竞买价
	nPrice = pMarketData["nMatch"]
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(10):
		nSize = nSize + decompressData(iData, p[nSize:])
		pMarketData["nBidPrice"][i] = nPrice - iData
		nPrice = pMarketData["nBidPrice"][i]
	#竞卖价
	nPrice = pMarketData["nMatch"]
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(10):
		nSize = nSize + decompressData(iData, p[nSize:])
		pMarketData["nAskPrice"][i] = nPrice - iData
		nPrice = pMarketData["nAskPrice"][i]
	#竞买量
	for i in range(10):
		nSize = nSize + decompressData(iData, p[nSize:])
		pMarketData["nBidVol"][i] = iData
	#竞卖量
	for i in range(10):
		nSize = nSize + decompressData(iData, p[nSize:])
		pMarketData["nAskVol"][i] = iData
	#成交笔数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nNumTrades"] = iData
	#成交总量
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["iVolume"]	= iData
	#成交总金额
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["iTurnover"]	= iData
	#委托买入总量
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nTotalBidVol"] = iData
	#加权平均委买价格
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nWeightedAvgBidPrice"] = iData
	#委托卖出总量
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nTotalAskVol"] = iData
	#加权平均委卖价格
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nWeightedAvgAskPrice"] = iData
	#IOPV净值估值
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nIOPV"] = iData
	#到期收益率
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nYieldToMaturity"] = iData
	#涨停价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nHighLimited"] = pMarketData["nPreClose"] + iData
	#跌停价
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nLowLimited"] = pMarketData["nPreClose"] + iData
	pMarketData["chPrefix"] = p[nSize:nSize+4]
	nSize += 4;
	return nSize
#解压期货行情数据
def DecompressMarketData_Futures(pMarketData, p):
	nSize = 0
	iData = np.int64(0)

	nSize =  decompressData(iData, p[nSize:])
	pMarketData["nIndex"]  = iData
	#状态
	nSize += decompressData(iData, p[nSize:]);	
	pMarketData["nStatus"] = iData
	#时间
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nTime"]	 = iData
	#昨持仓
	nSize += decompressData(iData, p[nSize:])
	pMarketData["iPreOpenInterest"] = iData
	#前收盘价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nPreClose"] = iData
	#昨日结算
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nPreSettlePrice"] = iData
	#开盘价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nOpen"] = iData + pMarketData["nPreClose"]
	#最高价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nHigh"] = iData + pMarketData["nPreClose"]
	#最低价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nLow"]  = iData + pMarketData["nPreClose"]
	#最新价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nMatch"]= iData + pMarketData["nPreClose"]
	#成交总量
	nSize += decompressData(iData, p[nSize:])
	pMarketData["iVolume"]	= iData
	#成交总金额
	nSize += decompressData(iData, p[nSize:])
	pMarketData["iTurnover"]	= iData
	#持仓
	nSize += decompressData(iData, p[nSize:])
	pMarketData["iOpenInterest"] = iData
	#收盘价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nClose"] = iData
	#结算价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nSettlePrice"] = iData
	#涨停价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nHighLimited"] = iData
	#跌停价
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nLowLimited"] = iData
	#昨虚实度
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nPreDelta"] = iData
	#今虚实度
	nSize += decompressData(iData, p[nSize:])
	pMarketData["nCurrDelta"] = iData
	#竞买价
	nPrice = pMarketData["nMatch"]
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(5):
		nSize += decompressData(iData, p[nSize:])
		pMarketData["nBidPrice"][i] = nPrice - iData
		nPrice = pMarketData["nBidPrice"][i]
	#竞卖价
	nPrice = pMarketData["nMatch"]
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(5):
		nSize += decompressData(iData, p[nSize:])
		pMarketData["nAskPrice"][i] = nPrice - iData
		nPrice = pMarketData["nAskPrice"][i]
	#竞买量
	for i in range(5):
		nSize += decompressData(iData, p[nSize:])
		pMarketData["nBidVol"][i] = iData
	#竞卖量
	for i in range(5):
		nSize += decompressData(iData, p[nSize:])
		pMarketData["nAskVol"][i] = iData
	return nSize
#解压指数数据
def DecompressIndexData(pMarketData, p):
	nSize = 0
	iData = np.int64(0)
	#本日编号
	nSize =  decompressData(iData, p[nSize:])
	pMarketData["nIndex"] = iData
	#时间
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nTime"]  = iData
	#今日开盘指数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nOpenIndex"] = iData
	#今日最高指数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nHighIndex"] = iData + pMarketData["nOpenIndex"]
	#今日最低指数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nLowIndex"]  = iData + pMarketData["nOpenIndex"]
	#今日最新指数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nLastIndex"] = iData + pMarketData["nOpenIndex"]
	#参与计算相应指数的交易数量
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["iTotalVolume"]	= iData
	#参与计算相应指数的成交金额
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["iTurnover"]	= iData
	#前收指数
	nSize = nSize + decompressData(iData, p[nSize:])
	pMarketData["nPreCloseIndex"] = iData + pMarketData["nOpenIndex"]
	return nSize
"""