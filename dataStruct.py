#!/usr/bin/python
# -*- coding: utf-8 -*-
# dataStruct.py
#逐笔成交
import numpy as np
AD_Transaction = {
	"idx" : 0,
	"chSecurityCode" : "00000000",
	"b_isHistoryData" : True,
	"nCurDate" : 0,
	"nCurTime" : 0,
	"nDate" : 0,
	"nTime" : 0,
	"nIndex" : 0,
	"nPrice" : 0,
	"nVolume" : 0,
	"nTurnover" : 0
}

#成交队列
AD_OrderQueue = {
	"b_isHistoryData" : True,
	"nCurDate" : 0,
	"nCurTime" : 0,
	"chSecurityCode" : "00000000",
	"nDate" : 0,
	"nTime" : 0,
	"nSide" : 0,
	"nPrice" : 0,
	"nOrders" : 0,
	"nABItems" : 0,
	"nABVolume" : [0]*200
}
#行情数据
MarketDataForTrade = {
	"nindex" : 0,					#行情编号
	"chSecurityCode" : "00000000",	#证券代码
	"b_isHistoryData" : True,		#是否历史数据
	"nCurDate" : 0,					#当前日期
	"nCurTime" : 0,					#当前时间
	"nDate" : 0,					#数据的日期
	"nTime" : 0,					#数据的时间
	"nStatus" : 0,					#行情状态
	"deep" : 10,					#行情长度
	"mConversionRate" : 0,			#折算率
	"nPreClose" : 0,				#昨日收盘价
	"nOpen" : 0,					#开盘价
	"nHigh" : 0,					#最高价
	"nLow" : 0,						#最低价
	"nMatch" : 0,					#最新价
	"nAskPrice" : [0]*10,			#申卖价
	"nAskVol" : [0]*10,				#申卖量
	"nBidPrice" : [0]*10,			#申买价
	"nBidVol" : [0]*10,				#申买量
	"nNumTrades" : 0,				#成交笔数
	"iVolume" : np.int64(0),		#成交总量
	"iTurnover" : np.int64(0),		#成交总金额
	"nTotalBidVol" : np.int64(0),	#委托买入总量
	"nTotalAskVol" : np.int64(0),	#委托卖出总量
	"nWeightedAvgBidPrice" : 0,
	"nWeightedAvgAskPrice" : 0,
	"nIOPV" : 0,
	"nYieldToMaturity" : 0,
	"nHighLimited" : 0,
	"nLowLimited" : 0,
	"chPrefix" : 0
}