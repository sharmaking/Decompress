///////////////////////////////////////////////////////////////////
// 宏汇 Open DataFeed API 数据结构定义
// 所有价格都用int表示，其值为实际价格x10000
// 例如：成交价为 16.80 表示为 168000
// @System Transend Open DataFeed API (TDF)
// @Date 2010.4.21
// @Copyright 深圳创真科技有限公司
///////////////////////////////////////////////////////////////////
// @Author wli
// @History 
//     2010-4-21	增加期货行情接口
//	   2010-11-12	请求ETF清单.
///////////////////////////////////////////////////////////////////

#ifndef _TDFAPIDEFINE_H_
#define _TDFAPIDEFINE_H_


//新一代TDF协议版本起始号
#define TDF_VERSION_NG_START		(0x6000)
#define TDF_VERSION_NX_START_6001	(0x6001)	//当前使用最新版本号.



#ifndef _WINDOWS
#define __int64 long long
#endif

#pragma pack(push,1)

//数据类型定义
#define ID_TDFTELE_LOGIN				1		//登录(Login)
#define ID_TDFTELE_LOGINANSWER			2		//登录应答
#define ID_TDFTELE_LOGOUT				3		//登出(Logout)
#define ID_TDFTELE_CLOSE				4		//服务器关闭(Server Close)
#define ID_TDFTELE_COCDETABLE			6		//证劵索引表(Security Directory)
#define ID_TDFTELE_REQDATA				7		//请求行情数据
#define ID_TDFTELE_MARKETCLOSE			8		//闭市信息	
#define ID_TDFTELE_TRADINGMESSAGE		9		//交易信息
#define ID_TDFTELE_HEARTBEAT			10		//心跳消息
#define ID_TDFTELE_TRANSACTION			1101	//成交(Transaction)
#define ID_TDFTELE_ORDERQUEUE			1102	//委托队列(Queue)
#define ID_TDFTELE_ORDER				1103	//逐笔委托(Order)
#define ID_TDFTELE_ORDERQUEUE_FAST		1104	//委托队列(FAST Queue)
#define ID_TDFTELE_MARKETDATA			1012	//行情数据(Market Data)
#define ID_HDFTELE_MARKETDATA_FUTURES	1016	//期货行情数据(Futures Market Data)	1016
#define ID_TDFTELE_INDEXDATA			1113	//指数(Index)
#define ID_TDFTELE_MARKETOVERVIEW		1115	//市场概况(Market Overview)
#define ID_TDFTELE_ETFLISTFILE			1116	//ETF清单文件.



#define ID_TDFTELE_ADDCODE			2001	//添加代码.
#define ID_TDFTELE_SUBSCRIPTION		2002	//订阅行情.
#define ID_TDFTELE_PLAYSPEED		2003	//数据回放速度设置.
#define ID_TDFTELE_REQETFLIST		2004	//请求ETF清单.
enum PlantMsgType{Enum_BuyOpen,Enum_SellOpen,Enum_BuyClose,Enum_SellClose}; 

enum DataType{	Enum_AD_Transaction,
Enum_AD_OrderQueue,
Enum_MarketDataForTrade,
Enum_MarketDataForTrade_Future,
Enum_AD_Index};

//////////////////////////////////////////////////////////////////////////
#define	DT_AD_Transaction				'1'
#define DT_AD_OrderQueue				'2'
#define DT_MarketDataForTrade			'3'
#define DT_MarketDataForTrade_Future	'4'
#define DT_AD_Index						'5'
//////////////////////////////////////////////////////////////////////////

struct SubStock
{
	string MarketCode;
	string StockCode;
};
struct pageStruct
{
	int pageIndex;
	int countIndex;
};
//成交明细
struct TransactionDetail
{
	int 	nTime;		//成交时间(HHMMSSmmmm)
	int		nPrice;		//成交价格
	int 	nVolume;	//成交数量
	int		nTurnover;	//成交金额
};

struct PlantSigle
{
	//板块信号
	string sourcestockcode;	//发出信号的股票代码
	PlantMsgType m_MsgType;
	int m_time;
};
//行情数据，用于撮合成交
struct MarketDataForTrade
{
	int  nindex;
	char chSecurityCode[8];			//证券代码
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	int		nDate;						//本日编号
	int		nTime;						//时间(HHMMSSmmmm)
	int		nStatus;					//状态
	int		deep;						//行情长度
	int mConversionRate;		//折算率
	int nPreClose;				//前收盘价
	int nOpen;					//开盘价
	int nHigh;					//最高价
	int nLow;					//最低价
	int nMatch;				//最新价			
	int nAskPrice[10];			//申卖价
	int nAskVol[10];			//申卖量
	int nBidPrice[10];			//申买价
	int nBidVol[10];			//申买量
	int nNumTrades;			//成交笔数
	__int64	iVolume;					//成交总量
	__int64	iTurnover;					//成交总金额
	__int64	nTotalBidVol;				//委托买入总量
	__int64	nTotalAskVol;				//委托卖出总量
	int nWeightedAvgBidPrice;	//加权平均委买价格
	int nWeightedAvgAskPrice;  //加权平均委卖价格
	int		nIOPV;						//IOPV净值估值
	int		nYieldToMaturity;			//到期收益率
	int nHighLimited;			//涨停价
	int nLowLimited;			//跌停价
	char	chPrefix[4];				//证券信息前缀
	MarketDataForTrade(){
		nindex=0;
		b_isHistoryData=true;
		nCurDate=0;
		nCurTime=0;
		nDate=0;
		nTime=0;
		nStatus=0;
		deep=10;
		mConversionRate=0;
		nPreClose=0;
		nOpen=0;
		nHigh=0;
		nMatch=0;
		memset(nAskPrice,0,sizeof(nAskPrice));
		memset(nAskVol,0,sizeof(nAskVol));
		memset(nBidPrice,0,sizeof(nBidPrice));
		memset(nBidVol,0,sizeof(nBidVol));
		nNumTrades=0;
		iVolume=0;
		iTurnover=0;
		nTotalBidVol=0;
		nTotalAskVol=0;
		nWeightedAvgBidPrice=0;
		nWeightedAvgAskPrice=0;
		nIOPV=0;
		nYieldToMaturity=0;
		nHighLimited=0;
		nLowLimited=0;
		memset(chSecurityCode,0,sizeof(chSecurityCode));
		memset(chPrefix,0,sizeof(chPrefix));};
};
struct MarketDataForTrade_Future
{
	char	chSecurityCode[8];			//期货代码
	int	nIndex;							//期货编号
	int	nDate;							//日期 yyyymmdd
	int	nTime;							//时间(HHMMSSmmmm)
	int		nStatus;					//状态
	__int64 iPreOpenInterest;			//昨持仓
	int nPreClose;				//昨收盘价
	int nPreSettlePrice;		//昨结算
	int nOpen;					//开盘价	
	int nHigh;					//最高价
	int nLow;					//最低价
	int nMatch;				//最新价
	__int64	iVolume;					//成交总量
	__int64	iTurnover;					//成交总金额
	__int64 iOpenInterest;				//持仓总量
	int nClose;				//今收盘
	int nSettlePrice;			//今结算
	int nHighLimited;		//涨停价
	int nLowLimited;		//跌停价
	int		nPreDelta;			        //昨虚实度
	int     nCurrDelta;                 //今虚实度
	int nAskPrice[5];			//申卖价
	int nAskVol[5];			//申卖量
	int nBidPrice[5];			//申买价
	int nBidVol[5];			//申买量
};
struct AD_Transaction
{
	int		idx;		//在包中的编号
	char chSecurityCode[8]; //证券代码
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	int		nDate;
	int 	nTime;		//成交时间(HHMMSSmmmm)
	int 	nIndex;		//成交编号
	int		nPrice;		//成交价格
	int 	nVolume;	//成交数量
	int		nTurnover;	//成交金额
	AD_Transaction(){
		idx=0;
		memset(chSecurityCode,0,sizeof(chSecurityCode));
		b_isHistoryData=true;
		nCurDate=0;
		nCurTime=0;
		nDate=0;
		nTime=0;
		nIndex=0;
		nPrice=0;
		nVolume=0;
		nTurnover=0;
	};
};
struct AD_TransactionPackage
{
	int  PackageIndex;
	int  nsize;				//这个逐笔成交数据包中的条数
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	char chSecurityCode[8]; //证券代码
	int	 nDate;
	list<AD_Transaction> m_Package;
	AD_TransactionPackage(){
		PackageIndex=0;
		nsize=0;
		nCurDate=0;
		nCurTime=0;
		nDate=0;
		b_isHistoryData=true;
		memset(chSecurityCode,0,sizeof(chSecurityCode));

	};
};
struct AD_OrderQueue
{
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	char chSecurityCode[8]; //证券代码
	int		nDate;
	int 	nTime;			//订单编号(HHMMSSmmmm)
	int     nSide;			//买卖方向('B':Bid 'S':Ask)
	int		nPrice;			//成交价格
	int 	nOrders;		//订单数量
	int 	nABItems;		//明细个数
	int 	nABVolume[200];	//订单明细
	AD_OrderQueue(){
		b_isHistoryData=true;
		nCurDate=0;
		nCurTime=0;
		memset(chSecurityCode,0,sizeof(chSecurityCode));
		nDate=0;
		nTime=0;
		nSide=0;
		nPrice=0;
		nOrders=0;
		nABItems=0;
		memset(nABVolume,0,sizeof(nABVolume));
	};
};
struct AD_Index
{
	char	chSecurityCode[8]; //指数代码
	int		nIndex;			//指数编号
	int		nDate;			//日期 yyyymmdd
	int     nTime;			//时间(HHMMSSmmmm)
	int		nOpenIndex;		//今开盘指数
	int 	nHighIndex;		//最高指数
	int 	nLowIndex;		//最低指数
	int 	nLastIndex;		//最新指数
	__int64	iTotalVolume;	//参与计算相应指数的交易数量
	__int64	iTurnover;		//参与计算相应指数的成交金额
	int		nPreCloseIndex;	//前收盘指数

};
struct TDFDefine_MsgHead
{
	unsigned short  	sFlags;		//16字节 标识符,升级到 TDF_VERSION_NX_START_6001 .
    unsigned short  	sDataType;	//16字节 数据类型	          
	int					nDataLen;	//32字节 数据长度
    int					nTime;		//32字节 时间戳（精确到毫秒HHMMSSmmmm）
    int     			nOrder;		//32字节 流水号
};

struct TDFDefine_Login
{
	char chUserName[16];
	char chPassword[32];
	char chID[8];
	char chMD5[32];
};

struct TDFDefine_LoginAnswer
{
	char chInfo[64];			//信息
	int  nAnswer;				//1:登陆成功
	int  nMarkets;
	char chMarketFlag[32][4];	//市场标志
	int  nDynDate[32];			//动态数据日期
};

struct TDFDefine_RequestCodeTable
{
	char chMarketFlag[4];		//市场标志(SZ;SH;HK;CF)
	int  nDate;					//数据日期，-1:表示动态数据，
};

#define ID_HDFDATAFLAGS_RETRANSALTE			0x00000001	//数据从开始传送
#define ID_HDFDATAFLAGS_NOTRANSACTION		0x00000100	//不传送逐笔成交数据
#define ID_HDFDATAFLAGS_NOABQUEUE			0x00000200	//不传送委托队列数据
#define ID_HDFDATAFLAGS_NOINDEX				0x00000400	//不传送指数数据
#define ID_HDFDATAFLAGS_NOMARKETOVERVIEW	0x00000800	//不传送 Market OverView 数据
#define ID_HDFDATAFLAGS_NOORDER				0x00001000	//不传送逐笔委托数据（SZ-Level2）
#define ID_HDFDATAFLAGS_COMPRESSED			0x00010000	//启用数据压缩
#define ID_HDFDATAFLAGS_ABQUEUE_FAST		0x00020000	//以FAST方式提供委托队列数据

struct TDFDefine_RequestMarketData
{
	char chMarketFlag[4];		//市场标志(SZ;SH;HK;CF)
	int  nFlags;				//由ID_HDFDATAFLAGS_定义
	int  nTime;					//行情指定时间点回放数据，等于零时表示当前时间开始回放（动态）或从头开始回放（历史），历史行情指定时间点回放时会有一个等待时间，比如从10:30开始回放需等待大约10秒
};

///1.1.1 证券索引表（Security Directory）
struct TDFDefine_SecurityCodeHead
{
	int  nSource;	//交易所 0:深圳 2:上海
	int  nDate;		//数据日期
	int  nCount;	//数据总数(-1:未授权)
	int  nFlags;	//(保留)
};

struct TDFDefine_SecurityCode
{
	int  nIdnum;				//本日编号(交易所编号*100 + 交易所编号)
	int  nType;					//证券类型
	char chSecurityCode[8];		//证券代码
	char chSymbol[16];			//证券名称
};

//1.1.2 逐笔成交(Transaction)
struct TDFDefine_TransactionHead
{
	int 	nIdnum;			//本日编号
	int     nItems;			//数据个数
};

struct TDFDefine_Transaction
{
	int 	nTime;		//成交时间(HHMMSSmmmm)
	int 	nIndex;		//成交编号
	int		nPrice;		//成交价格
	int 	nVolume;	//成交数量
	int		nTurnover;	//成交金额
};

//1.1.2 逐笔委托(Order)
struct TDFDefine_OrderHead
{
	int 	nIdnum;			//本日编号
	int     nItems;			//数据个数
};

struct TDFDefine_Order
{
	int 	nTime;		//委托时间(HHMMSSmmmm)
	int 	nIndex;		//委托编号
	int		nPrice;		//委托价格
	int 	nVolume;	//委托数量
	char    chType;		//委托类别
	char    chBSFlag;	//委托买卖类型('B','S','C')
	char    chResv[2];	//保留
};

//1.1.3 订单队列(Queue)
struct TDFDefine_OrderQueueHead
{
	int     nItems;			//数据个数
};

struct TDFDefine_OrderQueue
{
	int 	nIdnum;			//本日编号
	int 	nTime;			//订单编号(HHMMSSmmmm)
	int     nSide;			//买卖方向('B':Bid 'S':Ask)
	int		nPrice;			//成交价格
	int 	nOrders;		//订单数量
	int 	nABItems;		//明细个数
	int 	nABVolume[200];	//订单明细
};

//1.1.3.1 订单队列(上海FAST Queue)
struct TDFDefine_OrderQueue_FAST
{
	int  nIdnum;			//本日编号
	int  nTime;				//订单时间（HHMMSS）
	int  nSide;				//买卖方向（'B':Bid 'A':Ask）
 	int  nImageStatus;		//操作状态（1-Full Image 2-Update）
	int  nNoPriceLevel;		//操作的价位数
}; 

struct TDFDefine_OrderQueue_FAST_Operate
{
	int		nPriceOperate;	// 1.add , 2 Update ,3 Delete  0. absent 
	int     nPrice;			//价格
	int		nNumOrders;		//委托笔数
	int		nOqLevel;		//当前价位上要操作的订单数
};

struct TDFDefine_OrderQueue_FAST_OperateItem
{
	int nOperate;	//操作方式: 1 Add ,2 Update,3 Delete
	int nEntryID;   //操作ID:  Oreder Queue Operator Entry ID
	int nQty;		//订单数量
};

//1.1.4 行情数据(Market Data)
struct TDFDefine_MarketDataHead
{
	int     nItems;			//数据个数
};

typedef TDFDefine_MarketDataHead TDFDefine_MarketDataFuturesHead;

struct TDFDefine_MarketData
{
	int		nIdnum;						//本日编号
	int		nTime;						//时间(HHMMSSmmmm)
	int		nStatus;					//状态
	unsigned int nPreClose;				//前收盘价
	unsigned int nOpen;					//开盘价
	unsigned int nHigh;					//最高价
	unsigned int nLow;					//最低价
	unsigned int nMatch;				//最新价
	unsigned int nAskPrice[10];			//申卖价
	unsigned int nAskVol[10];			//申卖量
	unsigned int nBidPrice[10];			//申买价
	unsigned int nBidVol[10];			//申买量
	unsigned int nNumTrades;			//成交笔数
	__int64	iVolume;					//成交总量
	__int64	iTurnover;					//成交总金额
	__int64	nTotalBidVol;				//委托买入总量
	__int64	nTotalAskVol;				//委托卖出总量
	unsigned int nWeightedAvgBidPrice;	//加权平均委买价格
	unsigned int nWeightedAvgAskPrice;  //加权平均委卖价格
	int		nIOPV;						//IOPV净值估值
	int		nYieldToMaturity;			//到期收益率
	unsigned int nHighLimited;			//涨停价
	unsigned int nLowLimited;			//跌停价
	char	chPrefix[4];				//证券信息前缀
	int		nProcessTime;
};

//1.1.5 指数(Index)
struct TDFDefine_IndexDataHead
{
	int     nItems;				//数据个数
};

struct TDFDefine_IndexData
{
	int		nIdnum;			//本日编号
	int     nTime;			//时间(HHMMSSmmmm)
	int		nOpenIndex;		//今开盘指数
	int 	nHighIndex;		//最高指数
	int 	nLowIndex;		//最低指数
	int 	nLastIndex;		//最新指数
	__int64	iTotalVolume;	//参与计算相应指数的交易数量
	__int64	iTurnover;		//参与计算相应指数的成交金额
	int		nPreCloseIndex;	//前盘指数
};


//1.1.6 市场概况(Market Overview)
struct TDFDefine_MarketOverview
{
	int		nSource;
	int		nOrigTime;			//时间(HHMMSSmmmm)
	int		nOrigDate;			//日期
	char	chEndOfDayMarket;	//结束标志
};


//f)	闭市(Close)
struct TDFDefine_MarketClose
{
	int		nSource;
	int		nTime;				//时间(HHMMSSmmmm)
	char	chInfo[64];			//闭市信息
};

//g)	交易信息通知 (Trading Message)
struct TDFDefine_TradingMessage
{
	int		nIdnum;				//本日编号
	char	chSecurityId[8];	//证券代码
	char	chInfo[128];		//信息
	int		nTime;				//时间(HHMMSSmmmm)
};


//期货Markets Data.
struct TDFDefine_MarketData_Futures
{
	int nIdnum;							//本日编号
	int	nTime;							//时间(HHMMSSmmmm)	
	int		nStatus;					//状态
	__int64 iPreOpenInterest;			//昨持仓
	unsigned int nPreClose;				//昨收盘价
	unsigned int nPreSettlePrice;		//昨结算
	unsigned int nOpen;					//开盘价	
	unsigned int nHigh;					//最高价
	unsigned int nLow;					//最低价
	unsigned int nMatch;				//最新价
	__int64	iVolume;					//成交总量
	__int64	iTurnover;					//成交总金额
	__int64 iOpenInterest;				//持仓总量
	unsigned int nClose;				//今收盘
	unsigned int nSettlePrice;			//今结算
	unsigned int nHighLimited;		//涨停价
	unsigned int nLowLimited;		//跌停价
	int		nPreDelta;			        //昨虚实度
	int     nCurrDelta;                 //今虚实度
	unsigned int nAskPrice[5];			//申卖价
	unsigned int nAskVol[5];			//申卖量
	unsigned int nBidPrice[5];			//申买价
	unsigned int nBidVol[5];			//申买量
};


//Add by wli 2010-8-23
//增加行情订阅协议,只传只定的品种.
struct TDFDefine_SubscriptionHead
{
	int nSubscriptionType;	//类型: 0 设置. 1. 增加 2. 删除
	
	int nItems;	//指示其后的 TDFDefine_SubscriptionData 数目.
};

struct TDFDefine_SubscriptionData
{
	char chMarket[4];	//市场
	char chSymbol[8];	//代码
};

/**
Add by wli 2010-10-14
回放速度设置.在
*/
struct TDFDefine_PlaySpeed
{
	bool bEnable;	//是否启用回放速度.
	double dSpeed;	//回放速度.
};

/**
*请求ETF清单
*/
struct TDFDefine_ReqETFList
{
	int		nDate;			//日期, -1表示最后日期.
	char    chMarket[4];	//市场.
	char    chSymbol[8];	//代码.
};


/**
* 请求ETF时返回的文件头信息.
*/
struct TDFDefine_EtfList_Head
{
	int		nRetCode;			//返回码，0 正常，-1 无法定位文件. -2 请求消息长度有误. -3、读取文件失败.
	int		nFileLength;		//文件长度,单位Byte.
	char	szFileName[32];		//文件名
};


/**
* 请求发送系统心跳包
*/
struct TDFDefine_ReqHeartbeat_Head
{
	int  nFlags;	//心跳数据发送方式（0: 取消发送心跳包 1：没有数据发送的时候发送心跳数据，2：有规律发送心跳数据）
	int  nBeatTime;	//心跳包间隔时间（0：取消心跳包，>0：心跳包间隔时间（秒））
};

struct StockHQList
{
	TDFDefine_SecurityCode code;
	char chTradeFlag;
	int nTime;
	int nPreClose;
	int nPrice;
	__int64 nVolume;
	__int64 Turnover;
	int nNumTrades;
};
struct CharData{
	char * m_pChar;
	int m_length;
	CharData()
	{
		m_pChar = NULL;
		m_length = 0;
	}
	CharData(char * str,int leng)
	{
		Set(str,leng);
	}
	void Set(char * str,int leng)
	{
		memset(this, 0, sizeof(CharData));
		m_pChar = new char[leng+10];
		memcpy(m_pChar, str, leng);
		m_length = leng;
	}
	void ReSet()
	{
		if(m_pChar != NULL)
			delete []m_pChar;
		m_pChar = NULL;
		m_length = 0;
	}
	~CharData()
	{
		if(m_pChar != NULL)
			delete []m_pChar;
		m_pChar = NULL;
	}
};
#pragma pack(pop)
#endif
