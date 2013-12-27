///////////////////////////////////////////////////////////////////
// ��� Open DataFeed API ���ݽṹ����
// ���м۸���int��ʾ����ֵΪʵ�ʼ۸�x10000
// ���磺�ɽ���Ϊ 16.80 ��ʾΪ 168000
// @System Transend Open DataFeed API (TDF)
// @Date 2010.4.21
// @Copyright ���ڴ���Ƽ����޹�˾
///////////////////////////////////////////////////////////////////
// @Author wli
// @History 
//     2010-4-21	�����ڻ�����ӿ�
//	   2010-11-12	����ETF�嵥.
///////////////////////////////////////////////////////////////////

#ifndef _TDFAPIDEFINE_H_
#define _TDFAPIDEFINE_H_


//��һ��TDFЭ��汾��ʼ��
#define TDF_VERSION_NG_START		(0x6000)
#define TDF_VERSION_NX_START_6001	(0x6001)	//��ǰʹ�����°汾��.



#ifndef _WINDOWS
#define __int64 long long
#endif

#pragma pack(push,1)

//�������Ͷ���
#define ID_TDFTELE_LOGIN				1		//��¼(Login)
#define ID_TDFTELE_LOGINANSWER			2		//��¼Ӧ��
#define ID_TDFTELE_LOGOUT				3		//�ǳ�(Logout)
#define ID_TDFTELE_CLOSE				4		//�������ر�(Server Close)
#define ID_TDFTELE_COCDETABLE			6		//֤��������(Security Directory)
#define ID_TDFTELE_REQDATA				7		//������������
#define ID_TDFTELE_MARKETCLOSE			8		//������Ϣ	
#define ID_TDFTELE_TRADINGMESSAGE		9		//������Ϣ
#define ID_TDFTELE_HEARTBEAT			10		//������Ϣ
#define ID_TDFTELE_TRANSACTION			1101	//�ɽ�(Transaction)
#define ID_TDFTELE_ORDERQUEUE			1102	//ί�ж���(Queue)
#define ID_TDFTELE_ORDER				1103	//���ί��(Order)
#define ID_TDFTELE_ORDERQUEUE_FAST		1104	//ί�ж���(FAST Queue)
#define ID_TDFTELE_MARKETDATA			1012	//��������(Market Data)
#define ID_HDFTELE_MARKETDATA_FUTURES	1016	//�ڻ���������(Futures Market Data)	1016
#define ID_TDFTELE_INDEXDATA			1113	//ָ��(Index)
#define ID_TDFTELE_MARKETOVERVIEW		1115	//�г��ſ�(Market Overview)
#define ID_TDFTELE_ETFLISTFILE			1116	//ETF�嵥�ļ�.



#define ID_TDFTELE_ADDCODE			2001	//��Ӵ���.
#define ID_TDFTELE_SUBSCRIPTION		2002	//��������.
#define ID_TDFTELE_PLAYSPEED		2003	//���ݻط��ٶ�����.
#define ID_TDFTELE_REQETFLIST		2004	//����ETF�嵥.
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
//�ɽ���ϸ
struct TransactionDetail
{
	int 	nTime;		//�ɽ�ʱ��(HHMMSSmmmm)
	int		nPrice;		//�ɽ��۸�
	int 	nVolume;	//�ɽ�����
	int		nTurnover;	//�ɽ����
};

struct PlantSigle
{
	//����ź�
	string sourcestockcode;	//�����źŵĹ�Ʊ����
	PlantMsgType m_MsgType;
	int m_time;
};
//�������ݣ����ڴ�ϳɽ�
struct MarketDataForTrade
{
	int  nindex;
	char chSecurityCode[8];			//֤ȯ����
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	int		nDate;						//���ձ��
	int		nTime;						//ʱ��(HHMMSSmmmm)
	int		nStatus;					//״̬
	int		deep;						//���鳤��
	int mConversionRate;		//������
	int nPreClose;				//ǰ���̼�
	int nOpen;					//���̼�
	int nHigh;					//��߼�
	int nLow;					//��ͼ�
	int nMatch;				//���¼�			
	int nAskPrice[10];			//������
	int nAskVol[10];			//������
	int nBidPrice[10];			//�����
	int nBidVol[10];			//������
	int nNumTrades;			//�ɽ�����
	__int64	iVolume;					//�ɽ�����
	__int64	iTurnover;					//�ɽ��ܽ��
	__int64	nTotalBidVol;				//ί����������
	__int64	nTotalAskVol;				//ί����������
	int nWeightedAvgBidPrice;	//��Ȩƽ��ί��۸�
	int nWeightedAvgAskPrice;  //��Ȩƽ��ί���۸�
	int		nIOPV;						//IOPV��ֵ��ֵ
	int		nYieldToMaturity;			//����������
	int nHighLimited;			//��ͣ��
	int nLowLimited;			//��ͣ��
	char	chPrefix[4];				//֤ȯ��Ϣǰ׺
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
	char	chSecurityCode[8];			//�ڻ�����
	int	nIndex;							//�ڻ����
	int	nDate;							//���� yyyymmdd
	int	nTime;							//ʱ��(HHMMSSmmmm)
	int		nStatus;					//״̬
	__int64 iPreOpenInterest;			//��ֲ�
	int nPreClose;				//�����̼�
	int nPreSettlePrice;		//�����
	int nOpen;					//���̼�	
	int nHigh;					//��߼�
	int nLow;					//��ͼ�
	int nMatch;				//���¼�
	__int64	iVolume;					//�ɽ�����
	__int64	iTurnover;					//�ɽ��ܽ��
	__int64 iOpenInterest;				//�ֲ�����
	int nClose;				//������
	int nSettlePrice;			//�����
	int nHighLimited;		//��ͣ��
	int nLowLimited;		//��ͣ��
	int		nPreDelta;			        //����ʵ��
	int     nCurrDelta;                 //����ʵ��
	int nAskPrice[5];			//������
	int nAskVol[5];			//������
	int nBidPrice[5];			//�����
	int nBidVol[5];			//������
};
struct AD_Transaction
{
	int		idx;		//�ڰ��еı��
	char chSecurityCode[8]; //֤ȯ����
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	int		nDate;
	int 	nTime;		//�ɽ�ʱ��(HHMMSSmmmm)
	int 	nIndex;		//�ɽ����
	int		nPrice;		//�ɽ��۸�
	int 	nVolume;	//�ɽ�����
	int		nTurnover;	//�ɽ����
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
	int  nsize;				//�����ʳɽ����ݰ��е�����
	bool b_isHistoryData;
	int		nCurDate;
	int		nCurTime;
	char chSecurityCode[8]; //֤ȯ����
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
	char chSecurityCode[8]; //֤ȯ����
	int		nDate;
	int 	nTime;			//�������(HHMMSSmmmm)
	int     nSide;			//��������('B':Bid 'S':Ask)
	int		nPrice;			//�ɽ��۸�
	int 	nOrders;		//��������
	int 	nABItems;		//��ϸ����
	int 	nABVolume[200];	//������ϸ
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
	char	chSecurityCode[8]; //ָ������
	int		nIndex;			//ָ�����
	int		nDate;			//���� yyyymmdd
	int     nTime;			//ʱ��(HHMMSSmmmm)
	int		nOpenIndex;		//����ָ��
	int 	nHighIndex;		//���ָ��
	int 	nLowIndex;		//���ָ��
	int 	nLastIndex;		//����ָ��
	__int64	iTotalVolume;	//���������Ӧָ���Ľ�������
	__int64	iTurnover;		//���������Ӧָ���ĳɽ����
	int		nPreCloseIndex;	//ǰ����ָ��

};
struct TDFDefine_MsgHead
{
	unsigned short  	sFlags;		//16�ֽ� ��ʶ��,������ TDF_VERSION_NX_START_6001 .
    unsigned short  	sDataType;	//16�ֽ� ��������	          
	int					nDataLen;	//32�ֽ� ���ݳ���
    int					nTime;		//32�ֽ� ʱ�������ȷ������HHMMSSmmmm��
    int     			nOrder;		//32�ֽ� ��ˮ��
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
	char chInfo[64];			//��Ϣ
	int  nAnswer;				//1:��½�ɹ�
	int  nMarkets;
	char chMarketFlag[32][4];	//�г���־
	int  nDynDate[32];			//��̬��������
};

struct TDFDefine_RequestCodeTable
{
	char chMarketFlag[4];		//�г���־(SZ;SH;HK;CF)
	int  nDate;					//�������ڣ�-1:��ʾ��̬���ݣ�
};

#define ID_HDFDATAFLAGS_RETRANSALTE			0x00000001	//���ݴӿ�ʼ����
#define ID_HDFDATAFLAGS_NOTRANSACTION		0x00000100	//��������ʳɽ�����
#define ID_HDFDATAFLAGS_NOABQUEUE			0x00000200	//������ί�ж�������
#define ID_HDFDATAFLAGS_NOINDEX				0x00000400	//������ָ������
#define ID_HDFDATAFLAGS_NOMARKETOVERVIEW	0x00000800	//������ Market OverView ����
#define ID_HDFDATAFLAGS_NOORDER				0x00001000	//���������ί�����ݣ�SZ-Level2��
#define ID_HDFDATAFLAGS_COMPRESSED			0x00010000	//��������ѹ��
#define ID_HDFDATAFLAGS_ABQUEUE_FAST		0x00020000	//��FAST��ʽ�ṩί�ж�������

struct TDFDefine_RequestMarketData
{
	char chMarketFlag[4];		//�г���־(SZ;SH;HK;CF)
	int  nFlags;				//��ID_HDFDATAFLAGS_����
	int  nTime;					//����ָ��ʱ���ط����ݣ�������ʱ��ʾ��ǰʱ�俪ʼ�طţ���̬�����ͷ��ʼ�طţ���ʷ������ʷ����ָ��ʱ���ط�ʱ����һ���ȴ�ʱ�䣬�����10:30��ʼ�ط���ȴ���Լ10��
};

///1.1.1 ֤ȯ������Security Directory��
struct TDFDefine_SecurityCodeHead
{
	int  nSource;	//������ 0:���� 2:�Ϻ�
	int  nDate;		//��������
	int  nCount;	//��������(-1:δ��Ȩ)
	int  nFlags;	//(����)
};

struct TDFDefine_SecurityCode
{
	int  nIdnum;				//���ձ��(���������*100 + ���������)
	int  nType;					//֤ȯ����
	char chSecurityCode[8];		//֤ȯ����
	char chSymbol[16];			//֤ȯ����
};

//1.1.2 ��ʳɽ�(Transaction)
struct TDFDefine_TransactionHead
{
	int 	nIdnum;			//���ձ��
	int     nItems;			//���ݸ���
};

struct TDFDefine_Transaction
{
	int 	nTime;		//�ɽ�ʱ��(HHMMSSmmmm)
	int 	nIndex;		//�ɽ����
	int		nPrice;		//�ɽ��۸�
	int 	nVolume;	//�ɽ�����
	int		nTurnover;	//�ɽ����
};

//1.1.2 ���ί��(Order)
struct TDFDefine_OrderHead
{
	int 	nIdnum;			//���ձ��
	int     nItems;			//���ݸ���
};

struct TDFDefine_Order
{
	int 	nTime;		//ί��ʱ��(HHMMSSmmmm)
	int 	nIndex;		//ί�б��
	int		nPrice;		//ί�м۸�
	int 	nVolume;	//ί������
	char    chType;		//ί�����
	char    chBSFlag;	//ί����������('B','S','C')
	char    chResv[2];	//����
};

//1.1.3 ��������(Queue)
struct TDFDefine_OrderQueueHead
{
	int     nItems;			//���ݸ���
};

struct TDFDefine_OrderQueue
{
	int 	nIdnum;			//���ձ��
	int 	nTime;			//�������(HHMMSSmmmm)
	int     nSide;			//��������('B':Bid 'S':Ask)
	int		nPrice;			//�ɽ��۸�
	int 	nOrders;		//��������
	int 	nABItems;		//��ϸ����
	int 	nABVolume[200];	//������ϸ
};

//1.1.3.1 ��������(�Ϻ�FAST Queue)
struct TDFDefine_OrderQueue_FAST
{
	int  nIdnum;			//���ձ��
	int  nTime;				//����ʱ�䣨HHMMSS��
	int  nSide;				//��������'B':Bid 'A':Ask��
 	int  nImageStatus;		//����״̬��1-Full Image 2-Update��
	int  nNoPriceLevel;		//�����ļ�λ��
}; 

struct TDFDefine_OrderQueue_FAST_Operate
{
	int		nPriceOperate;	// 1.add , 2 Update ,3 Delete  0. absent 
	int     nPrice;			//�۸�
	int		nNumOrders;		//ί�б���
	int		nOqLevel;		//��ǰ��λ��Ҫ�����Ķ�����
};

struct TDFDefine_OrderQueue_FAST_OperateItem
{
	int nOperate;	//������ʽ: 1 Add ,2 Update,3 Delete
	int nEntryID;   //����ID:  Oreder Queue Operator Entry ID
	int nQty;		//��������
};

//1.1.4 ��������(Market Data)
struct TDFDefine_MarketDataHead
{
	int     nItems;			//���ݸ���
};

typedef TDFDefine_MarketDataHead TDFDefine_MarketDataFuturesHead;

struct TDFDefine_MarketData
{
	int		nIdnum;						//���ձ��
	int		nTime;						//ʱ��(HHMMSSmmmm)
	int		nStatus;					//״̬
	unsigned int nPreClose;				//ǰ���̼�
	unsigned int nOpen;					//���̼�
	unsigned int nHigh;					//��߼�
	unsigned int nLow;					//��ͼ�
	unsigned int nMatch;				//���¼�
	unsigned int nAskPrice[10];			//������
	unsigned int nAskVol[10];			//������
	unsigned int nBidPrice[10];			//�����
	unsigned int nBidVol[10];			//������
	unsigned int nNumTrades;			//�ɽ�����
	__int64	iVolume;					//�ɽ�����
	__int64	iTurnover;					//�ɽ��ܽ��
	__int64	nTotalBidVol;				//ί����������
	__int64	nTotalAskVol;				//ί����������
	unsigned int nWeightedAvgBidPrice;	//��Ȩƽ��ί��۸�
	unsigned int nWeightedAvgAskPrice;  //��Ȩƽ��ί���۸�
	int		nIOPV;						//IOPV��ֵ��ֵ
	int		nYieldToMaturity;			//����������
	unsigned int nHighLimited;			//��ͣ��
	unsigned int nLowLimited;			//��ͣ��
	char	chPrefix[4];				//֤ȯ��Ϣǰ׺
	int		nProcessTime;
};

//1.1.5 ָ��(Index)
struct TDFDefine_IndexDataHead
{
	int     nItems;				//���ݸ���
};

struct TDFDefine_IndexData
{
	int		nIdnum;			//���ձ��
	int     nTime;			//ʱ��(HHMMSSmmmm)
	int		nOpenIndex;		//����ָ��
	int 	nHighIndex;		//���ָ��
	int 	nLowIndex;		//���ָ��
	int 	nLastIndex;		//����ָ��
	__int64	iTotalVolume;	//���������Ӧָ���Ľ�������
	__int64	iTurnover;		//���������Ӧָ���ĳɽ����
	int		nPreCloseIndex;	//ǰ��ָ��
};


//1.1.6 �г��ſ�(Market Overview)
struct TDFDefine_MarketOverview
{
	int		nSource;
	int		nOrigTime;			//ʱ��(HHMMSSmmmm)
	int		nOrigDate;			//����
	char	chEndOfDayMarket;	//������־
};


//f)	����(Close)
struct TDFDefine_MarketClose
{
	int		nSource;
	int		nTime;				//ʱ��(HHMMSSmmmm)
	char	chInfo[64];			//������Ϣ
};

//g)	������Ϣ֪ͨ (Trading Message)
struct TDFDefine_TradingMessage
{
	int		nIdnum;				//���ձ��
	char	chSecurityId[8];	//֤ȯ����
	char	chInfo[128];		//��Ϣ
	int		nTime;				//ʱ��(HHMMSSmmmm)
};


//�ڻ�Markets Data.
struct TDFDefine_MarketData_Futures
{
	int nIdnum;							//���ձ��
	int	nTime;							//ʱ��(HHMMSSmmmm)	
	int		nStatus;					//״̬
	__int64 iPreOpenInterest;			//��ֲ�
	unsigned int nPreClose;				//�����̼�
	unsigned int nPreSettlePrice;		//�����
	unsigned int nOpen;					//���̼�	
	unsigned int nHigh;					//��߼�
	unsigned int nLow;					//��ͼ�
	unsigned int nMatch;				//���¼�
	__int64	iVolume;					//�ɽ�����
	__int64	iTurnover;					//�ɽ��ܽ��
	__int64 iOpenInterest;				//�ֲ�����
	unsigned int nClose;				//������
	unsigned int nSettlePrice;			//�����
	unsigned int nHighLimited;		//��ͣ��
	unsigned int nLowLimited;		//��ͣ��
	int		nPreDelta;			        //����ʵ��
	int     nCurrDelta;                 //����ʵ��
	unsigned int nAskPrice[5];			//������
	unsigned int nAskVol[5];			//������
	unsigned int nBidPrice[5];			//�����
	unsigned int nBidVol[5];			//������
};


//Add by wli 2010-8-23
//�������鶩��Э��,ֻ��ֻ����Ʒ��.
struct TDFDefine_SubscriptionHead
{
	int nSubscriptionType;	//����: 0 ����. 1. ���� 2. ɾ��
	
	int nItems;	//ָʾ���� TDFDefine_SubscriptionData ��Ŀ.
};

struct TDFDefine_SubscriptionData
{
	char chMarket[4];	//�г�
	char chSymbol[8];	//����
};

/**
Add by wli 2010-10-14
�ط��ٶ�����.��
*/
struct TDFDefine_PlaySpeed
{
	bool bEnable;	//�Ƿ����ûط��ٶ�.
	double dSpeed;	//�ط��ٶ�.
};

/**
*����ETF�嵥
*/
struct TDFDefine_ReqETFList
{
	int		nDate;			//����, -1��ʾ�������.
	char    chMarket[4];	//�г�.
	char    chSymbol[8];	//����.
};


/**
* ����ETFʱ���ص��ļ�ͷ��Ϣ.
*/
struct TDFDefine_EtfList_Head
{
	int		nRetCode;			//�����룬0 ������-1 �޷���λ�ļ�. -2 ������Ϣ��������. -3����ȡ�ļ�ʧ��.
	int		nFileLength;		//�ļ�����,��λByte.
	char	szFileName[32];		//�ļ���
};


/**
* ������ϵͳ������
*/
struct TDFDefine_ReqHeartbeat_Head
{
	int  nFlags;	//�������ݷ��ͷ�ʽ��0: ȡ������������ 1��û�����ݷ��͵�ʱ�����������ݣ�2���й��ɷ����������ݣ�
	int  nBeatTime;	//���������ʱ�䣨0��ȡ����������>0�����������ʱ�䣨�룩��
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
