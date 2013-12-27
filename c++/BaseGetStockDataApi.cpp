#include "StdAfx.h"
#include "BaseGetStockDataApi.h"
#include "TransferConnectionParser.h"
#include "DecompressFuncitons.h"
CTransferConnectionClientControl BaseGetStockDataApi::connection;
BaseGetStockDataSpi::BaseGetStockDataSpi()
{
	for(int i = 0; i <12; i++)
	{
		m_pHQList[i] = NULL;
	}

	m_dataLength = 10; // 预设10个
	m_pIDnum = new int[m_dataLength];
	m_pTransaction = new AD_Transaction[m_dataLength];
	m_pOrderQueue = new AD_OrderQueue[m_dataLength];
	m_pMarketData = new MarketDataForTrade[m_dataLength];
	m_pMDForFuture = new MarketDataForTrade_Future[m_dataLength];
	m_pADIndex	= new AD_Index[m_dataLength];
}
void BaseGetStockDataSpi::RecvMsgHandler(TransferMsg msg)
{	
	if (msg.msgType==999)
	{
		char* p_ch=(char*)msg.msgValue;
		curdate=TransferConnection::bytesToInt((byte*)p_ch,0);
	}
	else if(msg.msgType== 0)
	{
		vector<TDFDefine_SecurityCode *> p_rev;
		TransferConnectionParser<TDFDefine_SecurityCode>::decode(msg,p_rev);
		hqlist.clear();
		DeleteHQList();
		for (vector<TDFDefine_SecurityCode *>::iterator it= p_rev.begin();it!=p_rev.end();it++)
		{
			int exchid=(*it)->nIdnum%100;
			int k=(*it)->nIdnum/100;
			if (m_pHQList[exchid]==NULL)
			{
				m_pHQList[exchid]=new StockHQList[4000];
			}
			m_pHQList[exchid][k].code=(*(*it));
			hqlist.push_back(*(*it));
		}
	}
	else
	{
		if (msg.msgType==1)
		{
			char* p_ch=(char*)msg.msgValue;
			int INo=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			int count=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			if(count > m_dataLength)
			{
				ReNew(count);
			}

			CDecompressFuncitons::DecompressTransactionData(m_pTransaction, p_ch, count);
			// 构造CString
			CString stockCode = GetStockCode(INo);
			
			bool bSend = true;
			for(int i = 0; i < count; i++)
			{
				if(m_pTransaction[i].nPrice == 0 || m_pTransaction[i].nTime < 93000000)
				{
					bSend = false;					
				}
				strcpy(m_pTransaction[i].chSecurityCode,stockCode);
				m_pTransaction[i].nDate = curdate;
				// 统一价格和成交量
				m_pTransaction[i].nPrice = m_pTransaction[i].nPrice / 10 ;
				m_pTransaction[i].nVolume = m_pTransaction[i].nVolume / 100;
			}
			if(bSend)
				OnTransaction(m_pTransaction,count);			
		}
		else if (msg.msgType==2)
		{		
			char* p_ch=(char*)msg.msgValue;			
			int count=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			if(count > m_dataLength)
			{
				ReNew(count);
			}
			//AD_OrderQueue *pOrder = new AD_OrderQueue[pOrderHead->nItems];
			//int *pIdnum = new int[pOrderHead->nItems];
			CDecompressFuncitons::DecompressOrderQueueData(m_pOrderQueue, p_ch, count,m_pIDnum);
			bool bSend = true;
			for(int i = 0; i<count; i++)
			{
				CString stockCode = GetStockCode(m_pIDnum[i]);
				if(m_pOrderQueue[i].nPrice == 0 || m_pOrderQueue[i].nTime < 93000000 || stockCode == "")
				{
					bSend = false;
					break;
				}
				strcpy(m_pOrderQueue[i].chSecurityCode, stockCode);
				m_pOrderQueue[i].nDate = curdate;
				// 统一价格和成交量
				m_pOrderQueue[i].nPrice = m_pOrderQueue[i].nPrice / 10;
				for(int j = 0; j< m_pOrderQueue[i].nABItems; j++)
				{
					m_pOrderQueue[i].nABVolume[j] = m_pOrderQueue[i].nABVolume[j] / 100;
				}
			}
			if(bSend)
				OnRevOrderQueue(m_pOrderQueue,count);
		}
		else if (msg.msgType==3)
		{
			char* p_ch=(char*)msg.msgValue;			
			int count=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			if(count > m_dataLength)
			{
				ReNew(count);
			}
			//ONEASSERT(pMarketDataHead->nItems ==1 );

			int nDecompressPos=0;
			bool bSend = true;
			for(int npos =0;npos<count;++npos)
			{
				int nIdnum = 0;
				nDecompressPos += CDecompressFuncitons::DecompressMarketData(m_pMarketData+npos, p_ch + nDecompressPos ,nIdnum);

				CString stockCode = GetStockCode(nIdnum);
				if(m_pMarketData[npos].nTime < 93000000 || m_pMarketData[npos].nMatch == 0 || (m_pMarketData[npos].nBidVol[0] == 0 && m_pMarketData[npos].nAskVol[0] == 0) || (m_pMarketData[npos].nBidPrice[0] == 0 && m_pMarketData[npos].nAskPrice[0] == 0) || m_pMarketData[npos].iVolume == 0 || stockCode == "")
				{
					bSend = false;
					break;
				}
				strcpy(m_pMarketData[npos].chSecurityCode, stockCode);
				m_pMarketData[npos].nDate = curdate;

				m_pMarketData[npos].deep = 10;
				m_pMarketData[npos].nPreClose = m_pMarketData[npos].nPreClose / 10;
				m_pMarketData[npos].nOpen		= m_pMarketData[npos].nOpen / 10;
				m_pMarketData[npos].nHigh		= m_pMarketData[npos].nHigh / 10;
				m_pMarketData[npos].nLow		= m_pMarketData[npos].nLow / 10;
				m_pMarketData[npos].nMatch	= m_pMarketData[npos].nMatch / 10;
				for(int i = 0; i<10; i++)
				{
					m_pMarketData[npos].nAskPrice[i] = m_pMarketData[npos].nAskPrice[i] /10;
					m_pMarketData[npos].nBidPrice[i] = m_pMarketData[npos].nBidPrice[i] /10;
					m_pMarketData[npos].nBidVol[i]   = m_pMarketData[npos].nBidVol[i] / 100;
					m_pMarketData[npos].nAskVol[i]	= m_pMarketData[npos].nAskVol[i] / 100;
				}
				m_pMarketData[npos].iVolume = m_pMarketData[npos].iVolume / 100;
				m_pMarketData[npos].iTurnover = m_pMarketData[npos].iTurnover / 10;
				m_pMarketData[npos].nTotalAskVol = m_pMarketData[npos].nTotalAskVol / 100;
				m_pMarketData[npos].nTotalBidVol = m_pMarketData[npos].nTotalBidVol / 100;
				m_pMarketData[npos].nWeightedAvgAskPrice = m_pMarketData[npos].nWeightedAvgAskPrice /10;
				m_pMarketData[npos].nWeightedAvgBidPrice = m_pMarketData[npos].nWeightedAvgBidPrice /10;
				m_pMarketData[npos].nHighLimited = m_pMarketData[npos].nHighLimited /10;
				m_pMarketData[npos].nLowLimited  = m_pMarketData[npos].nLowLimited / 10;
			}
			if(bSend)
				OnRevMarket(m_pMarketData,count);
		}
		else if (msg.msgType==4)
		{
			char* p_ch=(char*)msg.msgValue;			
			int count=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			if(count > m_dataLength)
			{
				ReNew(count);
			}

			int nDecompressPos=0;
			bool bSend = true;
			for(int npos =0;npos<count;++npos)
			{
				nDecompressPos += CDecompressFuncitons::DecompressMarketData_Futures(m_pMDForFuture + npos, p_ch +nDecompressPos );
				strcpy_s(m_pMDForFuture[npos].chSecurityCode, GetStockCode(m_pMDForFuture[npos].nIndex));
				if(m_pMDForFuture[npos].nTime < 90000000 || m_pMDForFuture[npos].nMatch == 0)
				{
					bSend = false;
				}
				m_pMDForFuture[npos].nMatch = m_pMDForFuture[npos].nMatch / 10;
				m_pMDForFuture[npos].nLow   = m_pMDForFuture[npos].nLow / 10;
				m_pMDForFuture[npos].nHigh	= m_pMDForFuture[npos].nHigh / 10;
				m_pMDForFuture[npos].nPreClose	= m_pMDForFuture[npos].nPreClose / 10;
				m_pMDForFuture[npos].nOpen	= m_pMDForFuture[npos].nOpen / 10;
				m_pMDForFuture[npos].nPreSettlePrice = m_pMDForFuture[npos].nPreSettlePrice / 10;
				m_pMDForFuture[npos].nHighLimited	= m_pMDForFuture[npos].nHighLimited / 10;
				m_pMDForFuture[npos].nLowLimited	= m_pMDForFuture[npos].nLowLimited / 10;
				m_pMDForFuture[npos].nClose	= m_pMDForFuture[npos].nClose / 10;
				m_pMDForFuture[npos].nSettlePrice	= m_pMDForFuture[npos].nSettlePrice / 10;
				m_pMDForFuture[npos].nBidPrice[0] = m_pMDForFuture[npos].nBidPrice[0] / 10;
				m_pMDForFuture[npos].nAskPrice[0] = m_pMDForFuture[npos].nAskPrice[0] / 10;
				m_pMDForFuture[npos].nDate	= curdate;
			}
			if (bSend)
			{
				OnRevMarket_Future(m_pMDForFuture,count);
			}
		}
		else if (msg.msgType==5)
		{
			char* p_ch=(char*)msg.msgValue;			
			int count=TransferConnection::bytesToInt((byte*)p_ch,0);
			p_ch+=4;
			if(count > m_dataLength)
			{
				ReNew(count);
			}
			int nDecompressPos=0;
			bool bSend = true;
			for(int npos=0;npos<count;++npos)
			{
				nDecompressPos += CDecompressFuncitons::DecompressIndexData(m_pADIndex + npos, p_ch + nDecompressPos);
				if(m_pADIndex[npos].nTime < 90000000)
					bSend = false;
				strcpy_s(m_pADIndex[npos].chSecurityCode, GetStockCode(m_pADIndex[npos].nIndex));
				m_pADIndex[npos].nDate = curdate;
			}
			if(bSend)
				OnRevIndex(m_pADIndex,count);
		}
	}
}
CString BaseGetStockDataSpi::GetStockCode(int nIdnum)
{
	int k = nIdnum/100;
	int exch_id = nIdnum%100;
	CString result("");
	if(exch_id < 12 && exch_id >=0)
	{
		result = m_pHQList[exch_id][k].code.chSecurityCode;
	}
	return result;
}

void BaseGetStockDataSpi::Delete()
{
	delete []m_pIDnum;
	delete []m_pTransaction;
	delete []m_pOrderQueue;
	delete []m_pMarketData;
	delete []m_pMDForFuture;
	delete []m_pADIndex;
}
void BaseGetStockDataSpi::DeleteHQList()
{
	for(int i = 0; i <12; i++)
	{
		if(m_pHQList[i] != NULL)
		{
			delete []m_pHQList[i];
			m_pHQList[i] = NULL;
		}
	}
}
void BaseGetStockDataSpi::ReNew(int datalength)
{
	Delete();
	m_dataLength = datalength;
	m_pIDnum = new int[m_dataLength];
	m_pTransaction = new AD_Transaction[m_dataLength];
	m_pOrderQueue = new AD_OrderQueue[m_dataLength];
	m_pMarketData = new MarketDataForTrade[m_dataLength];
	m_pMDForFuture = new MarketDataForTrade_Future[m_dataLength];
	m_pADIndex	= new AD_Index[m_dataLength];
}
void BaseGetStockDataSpi::OnPeerDisconnect(TransferConnection* client)
{
	OnDisconnect();
}
bool BaseGetStockDataApi::InitConnection(char ip[MaxIPLen],int port,BaseGetStockDataSpi *traderSpi){
	connection.startup();
	m_traderSpi = traderSpi;
	traderSpi->p_api=this;	
	transferConnection = connection.ConnectToServer(ip,port, m_traderSpi, Enum_Socket);
	if (transferConnection==NULL)
	{
		return false;
	}
	else
		return true;
	
	return false;
}
void BaseGetStockDataApi::SubStockCode(vector<char *> stockcodes)
{
	TransferMsg msg;
	msg.msgType=0;
	TransferConnectionParser<TransferMsg>::sub_encode(msg,stockcodes);
	transferConnection->PostSendMsg(msg);
	delete[] msg.msgValue;	
}
void BaseGetStockDataApi::ReqTodayData(int flag,int begintime/* =0 */)
{
	TransferMsg msg;
	msg.msgType=1;
	msg.msgSize=8;
	LPBYTE lpby=new BYTE[msg.msgSize];
	LPBYTE lpbycpy=lpby;
	memcpy(lpby,&flag,4);
	lpby+=4;
	memcpy(lpby,&begintime,4);
	msg.msgValue=lpbycpy;
	transferConnection->PostSendMsg(msg);
	delete[] lpbycpy;
}
void BaseGetStockDataApi::ReqHistoryData(int date,int begintime/* =0 */,int endtime/* =153000 */)
{
	TransferMsg msg;
	msg.msgType=2;
	msg.msgSize=12;
	LPBYTE lpby=new BYTE[msg.msgSize];
	LPBYTE lpbycpy=lpby;
	memcpy(lpby,&date,4);
	lpby+=4;
	memcpy(lpby,&begintime,4);
	lpby+=4;
	memcpy(lpby,&endtime,4);
	msg.msgValue=lpbycpy;
	transferConnection->PostSendMsg(msg);
	delete[] lpbycpy;
}
void BaseGetStockDataApi::ReqHistorySectionData(int begindate,int enddata)
{	
	TransferMsg msg;
	msg.msgType=3;
	msg.msgSize=8;
	LPBYTE lpby=new BYTE[msg.msgSize];
	LPBYTE lpbycpy=lpby;
	memcpy(lpby,&begindate,4);
	lpby+=4;
	memcpy(lpby,&enddata,4);
	msg.msgValue=lpbycpy;
	transferConnection->PostSendMsg(msg);
	delete[] lpbycpy;
}
BaseGetStockDataApi::BaseGetStockDataApi(void)
{
	
}

BaseGetStockDataApi::~BaseGetStockDataApi(void)
{
}
