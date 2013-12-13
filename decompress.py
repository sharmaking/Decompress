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
			#iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | ((np.int64(p[5]&0xff)<<32)&0xff00000000) | ((np.int64(p[6]&0xff)<<40)&0xff0000000000) | ((np.int64(p[7]&0xff)<<48)&0xff000000000000)
			size = 8
	iData = iData * vol
	if p[0]&0x80:
		iData = -1 * iData
	return size

#解压行情数据
def DecompressMarketData(pMarketData, p):
	size = 0
	iData = np.int64(0)
	#启用数据压缩
	size =  decompressData(iData, p[size:])
	pMarketData["nIdnum"]	= iData
	#状态
	size = size + decompressData(iData, p[size:])
	pMarketData["nStatus"]	= iData
	size = size + decompressData(iData, p[size:])
	pMarketData["nTime"]	= iData
	size = size + decompressData(iData, p[size:])
	pMarketData["nPreClose"]	= iData
	#开盘价
	size = size + decompressData(iData, p[size:])
	pMarketData["nOpen"]	= iData + pMarketData["nPreClose"]
	#最高价
	size = size + decompressData(iData, p[size:])
	pMarketData["nHigh"]	= iData + pMarketData["nPreClose"]
	#最低价
	size = size + decompressData(iData, p[size:])
	pMarketData["nLow"]		= iData + pMarketData["nPreClose"]
	#最新价
	size = size + decompressData(iData, p[size:])
	pMarketData["nMatch"]	= iData + pMarketData["nPreClose"]
	#竟买价
	nPrice = pMarketData["nMatch"]
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(0,10):
		size = size + decompressData(iData, p[size:])
		pMarketData["nBidPrice"][i] = nPrice - iData
		nPrice = pMarketData["nBidPrice"][i]
	#竟卖价
	nPrice = pMarketData["nMatch"];
	if not nPrice:
		nPrice = pMarketData["nPreClose"]
	for i in range(0,10):
		size = size + decompressData(iData, p[size:])
		pMarketData["nAskPrice"][i] = nPrice - iData
		nPrice = pMarketData["nAskPrice"][i]
	#竟价量
	for i in range(0,10):
		size = size + decompressData(iData, p[size:])
		pMarketData["nBidVol"][i] = iData
	for i in range(0,10):
		size = size + decompressData(iData, p[size:])
		pMarketData["nAskVol"][i] = iData
	#成交笔数
	size = size + decompressData(iData, p[size:])
	pMarketData["nNumTrades"] = iData
	#成交总量
	size = size + decompressData(iData, p[size:])
	pMarketData["iVolume"] = iData
	#成交总金额
	size = size + decompressData(iData, p[size:])
	pMarketData["iTurnover"] = iData
	#委托买入总量
	size = size + decompressData(iData, p[size:])
	pMarketData["nTotalBidVol"] = iData
	#加权平均委买价格
	size = size + decompressData(iData, p[size:])
	pMarketData["nWeightedAvgBidPrice"] = iData
	#委托卖出总量
	size = size + decompressData(iData, p[size:])
	pMarketData["nTotalAskVol"] = iData
	#加权平均委卖价格
	size = size + decompressData(iData, p[size:])
	pMarketData["nWeightedAvgAskPrice"] = iData
	#IOPV净值估值
	size = size + decompressData(iData, p[size:])
	pMarketData["nIOPV"] = iData
	#到期收益率
	size = size + decompressData(iData, p[size:])
	pMarketData["nYieldToMaturity"] = iData
	#涨停价
	size = size + decompressData(iData, p[size:])
	pMarketData["nHighLimited"] = iData + pMarketData["nPreClose"]
	#跌停价
	size = size + decompressData(iData, p[size:])
	pMarketData["nLowLimited"] = iData + pMarketData["nPreClose"]

	pMarketData["chPrefix"] = p[size:(size+4)]
	size = size + 4

	return size

def DecompressTransactionData(pTransaction, p, nItems):
	nSize = 0
	iData = np.int64(0)
	nPreTime = 0
	nPreIndex = 0
	nPrePrice = 0
	for i in range(0, nItems):
		#成交时间
		size = size + decompressData(iData, p[size:])
		pTransaction[i]["nTime"] = iData + nPreTime
		nPreTime = pTransaction[i]["nTime"]
		#成交序号
		size = size + decompressData(iData, p[size:])
		pTransaction[i]["nIndex"] = iData + nPreIndex
		nPreIndex = pTransaction[i]["nIndex"]
		#成交价格
		size = size + decompressData(iData, p[size:])
		pTransaction[i]["nPrice"] = iData + nPrePrice
		nPrePrice = pTransaction[i]["nPrice"]
		#成交数量
		pass
	pass

def main():
	a = 0xf0ffe13c
	_a = struct.pack("I",a)
	print _a, a

	p = unpackBinaryStr(_a, 4)

	print p
	#decompressData(10, p)
	pass

if __name__ == '__main__':
	main()