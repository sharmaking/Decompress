#!/usr/bin/python
# -*- coding: utf-8 -*-
# decompress.py
import numpy as np
import struct

def unpackBinaryStr(s,length):
	fmt = "B"
	if length > 1:
		fmt = str(length) + fmt
	_p = struct.unpack(fmt,s)
	#倒序
	p = []
	for x in reversed(_p):
		p.append(x)
	p = tuple(p)
	return p
def decompressData(iData, p):
	size = 1
	vol = 1
	iData = np.int64(0)

	if ((p[0]&0x03) == 3): vol = 1000
	elif ((p[0]&0x03) == 2): vol = 100
	elif ((p[0]&0x03) == 1): vol = 10
	else: vol = 1

	if ((p[0]&0x70) == 0 and (p[0]&0x0c) == 0): iData = 0
	elif ((p[0]&0x0c) == 0x04): iData = 1
	elif ((p[0]&0x0c) == 0x08): iData = 2
	elif ((p[0]&0x0c) == 0x0c): iData = 3
	else:
		expression = p[0]&0x70
		if expression == 0x10:
			iData = p[1]&0xff
			size = 2
		elif expression == 0x20:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00)
			size = 3
		elif expression == 0x30:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000)
			size = 4
		elif expression == 0x40:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000)
			size = 5
		elif expression == 0x50:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | ((np.int64(p[5]&0xff)<<32)&0xff00000000)
			size = 6
		elif expression == 0x60:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | ((np.int64(p[5]&0xff)<<32)&0xff00000000) | ((np.int64(p[6]&0xff)<<40)&0xff0000000000)
			size = 7
		elif expression == 0x70:
			iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | ((np.int64(p[5]&0xff)<<32)&0xff00000000) | ((np.int64(p[6]&0xff)<<40)&0xff0000000000) | ((np.int64(p[7]&0xff)<<48)&0xff000000000000)
			size = 8
	iData = iData * vol
	if p[0]&0x80:
		iData = -1 * iData
	return size
#解压逐笔成交数据
def DecompressTransactionData(pTransactions, p, nItems):	#pTransactions为一list
	nSize = 0
	iData = np.int64(0)
	nPreTime = 0
	nPreIndex = 0
	nPrePrice = 0
	for i in range(nItems):
		#成交时间
		nSize = nSize + decompressData(iData, p[nSize:])
		pTransactions[i]["nTime"] = nPreTime + iData 
		nPreTime = pTransactions[i]["nTime"]
		#成交序号
		nSize = nSize + decompressData(iData, p[nSize:])			
		pTransactions[i]["nIndex"] = iData + nPreIndex
		nPreIndex = pTransactions[i]["nIndex"]
		#成交价格
		nSize = nSize + decompressData(iData, p[nSize:])				
		pTransactions[i]["nPrice"] = nPrePrice + iData
		nPrePrice = pTransactions[i]["nPrice"]
		#成交数量
		nSize = nSize + decompressData(iData, p[nSize:])					
		pTransactions[i]["nVolume"] = iData					

		pTransactions[i]["nTurnover"] = pTransactions[i]["nPrice"]/10000.0 * pTransactions[i]["nVolume"]
	return nSize
#解压成交队列
def DecompressOrderQueueData(pQueues, p, nItems, pIdnum):
	nSize = 0
	iData = np.int64(0)
	for i in range(nItems):
		#本日编号
		nSize = nSize + decompressData(iData, p[nSize:])
		pIdnum[i] = iData
		#订单编号
		nSize += decompressData(iData, p[nSize:])
		pQueues[i]["nTime"] = iData
		#买卖方向(A:Ask, B:Bid)
		nSize = nSize + 1
		pQueues[i]["nSide"] = p[nSize]
		#订单价格
		nSize = nSize + decompressData(iData, p[nSize:])
		pQueues[i]["nPrice"] = iData
		#订单数量
		nSize = nSize + decompressData(iData, p[nSize:])
		pQueues[i]["nOrders"] = iData
		#队列个数
		nSize = nSize + decompressData(iData, p[nSize:])
		pQueues[i]["nABItems"] = iData
		#订单数量
		for k in range(pQueues[i]["nABItems"]):
			nSize = nSize + decompressData(iData, p[nSize:])	
			pQueues[i]["nABVolume"][k] = iData
	return nSize
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