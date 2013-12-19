#pragma once
#include "TDFAPIDefine.h"



////////////////////////////////////////////////////////////////////////////
class CDecompressFuncitons
{
public:
	static int decompressData(__int64 &iData, char *p)
	{
		int size = 1;
		int vol = 1;
		iData = 0;
		if((p[0]&0x03)==3) vol = 1000;
		else if((p[0]&0x03)==2) vol = 100;
		else if((p[0]&0x03)==1) vol = 10;
		else vol = 1;

		if((p[0]&0x70)==0 && (p[0]&0x0c)==0) iData = 0;
		else if((p[0]&0x0c)==0x04) iData = 1;
		else if((p[0]&0x0c)==0x08) iData = 2;
		else if((p[0]&0x0c)==0x0c) iData = 3;
		else 
		{
			switch(p[0]&0x70)
			{
			case 0x10:
				iData = p[1]&0xff;
				size = 2;
				break;
			case 0x20:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00);
				size = 3;
				break;
			case 0x30:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000);
				size = 4;
				break;
			case 0x40:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000);
				size = 5;
				break;
			case 0x50:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | (((__int64)(p[5]&0xff)<<32)&0xff00000000L);
				size = 6;
				break;
			case 0x60:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | (((__int64)(p[5]&0xff)<<32)&0xff00000000) | (((__int64)(p[6]&0xff)<<40)&0xff0000000000);
				size = 7;
				break;
			case 0x70:
				iData = (p[1]&0xff) | ((p[2]<<8)&0xff00) | ((p[3]<<16)&0xff0000) | ((p[4]<<24)&0xff000000) | (((__int64)(p[5]&0xff)<<32)&0xff00000000) | (((__int64)(p[6]&0xff)<<40)&0xff0000000000) | (((__int64)(p[7]&0xff)<<48)&0xff000000000000);
				size = 8;
				break;
			}
		}
		iData *= vol;
		if(p[0]&0x80)
		{
			iData *= -1;
		}
		return size;
	}

	static int DecompressMarketData(TDFDefine_MarketData *pMarketData, char *p)
	{
		int size = 0;
		__int64 iData;
		{	//��������ѹ��
			int i;
			size =  decompressData(iData, &p[size]);	pMarketData->nIdnum  = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nStatus = iData;							//״̬
			size += decompressData(iData, &p[size]);	pMarketData->nTime	 = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nPreClose = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nOpen = iData + pMarketData->nPreClose;	//���̼�
			size += decompressData(iData, &p[size]);	pMarketData->nHigh = iData + pMarketData->nPreClose;	//��߼�
			size += decompressData(iData, &p[size]);	pMarketData->nLow  = iData + pMarketData->nPreClose;	//��ͼ�
			size += decompressData(iData, &p[size]);	pMarketData->nMatch= iData + pMarketData->nPreClose;	//���¼�
			int nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidPrice[i] = nPrice - iData;
				nPrice = pMarketData->nBidPrice[i];
			}
			nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskPrice[i] = nPrice - iData;
				nPrice = pMarketData->nAskPrice[i];
			}
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidVol[i] = iData;
			}
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskVol[i] = iData;
			}
			size += decompressData(iData, &p[size]);	pMarketData->nNumTrades = iData;			//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iVolume	= iData;		//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iTurnover	= iData;		//�ɽ��ܽ��
			size += decompressData(iData, &p[size]);	pMarketData->nTotalBidVol = iData;			//ί����������
			size += decompressData(iData, &p[size]);	pMarketData->nWeightedAvgBidPrice = iData;	//��Ȩƽ��ί��۸�
			size += decompressData(iData, &p[size]);	pMarketData->nTotalAskVol = iData;			//ί����������
			size += decompressData(iData, &p[size]);	pMarketData->nWeightedAvgAskPrice = iData;	//��Ȩƽ��ί���۸�
			size += decompressData(iData, &p[size]);	pMarketData->nIOPV = iData;					//IOPV��ֵ��ֵ
			size += decompressData(iData, &p[size]);	pMarketData->nYieldToMaturity = iData;		//����������
			size += decompressData(iData, &p[size]);	pMarketData->nHighLimited = pMarketData->nPreClose + iData;		//����������
			size += decompressData(iData, &p[size]);	pMarketData->nLowLimited = pMarketData->nPreClose + iData;		//����������
			memcpy(pMarketData->chPrefix, &p[size], 4);
			size += 4;
			//size += decompressData(iData, &p[size]);    pMarketData->nProcessTime = iData;
		}
		return size;
	}
	static int DecompressMarketData(MarketDataForTrade *pMarketData, char *p ,int &pIdnum)
	{
		int size = 0;
		__int64 iData;
		{	//��������ѹ��
			int i;
			size =  decompressData(iData, &p[size]);	pIdnum = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nStatus = iData;							//״̬
			size += decompressData(iData, &p[size]);	pMarketData->nTime	 = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nPreClose = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nOpen = iData + pMarketData->nPreClose;	//���̼�
			size += decompressData(iData, &p[size]);	pMarketData->nHigh = iData + pMarketData->nPreClose;	//��߼�
			size += decompressData(iData, &p[size]);	pMarketData->nLow  = iData + pMarketData->nPreClose;	//��ͼ�
			size += decompressData(iData, &p[size]);	pMarketData->nMatch= iData + pMarketData->nPreClose;	//���¼�
			int nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidPrice[i] = nPrice - iData;
				nPrice = pMarketData->nBidPrice[i];
			}
			nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskPrice[i] = nPrice - iData;
				nPrice = pMarketData->nAskPrice[i];
			}
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidVol[i] = iData;
			}
			for(i=0; i<10; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskVol[i] = iData;
			}
			size += decompressData(iData, &p[size]);	pMarketData->nNumTrades = iData;			//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iVolume	= iData;		//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iTurnover	= iData;		//�ɽ��ܽ��
			size += decompressData(iData, &p[size]);	pMarketData->nTotalBidVol = iData;			//ί����������
			size += decompressData(iData, &p[size]);	pMarketData->nWeightedAvgBidPrice = iData;	//��Ȩƽ��ί��۸�
			size += decompressData(iData, &p[size]);	pMarketData->nTotalAskVol = iData;			//ί����������
			size += decompressData(iData, &p[size]);	pMarketData->nWeightedAvgAskPrice = iData;	//��Ȩƽ��ί���۸�
			size += decompressData(iData, &p[size]);	pMarketData->nIOPV = iData;					//IOPV��ֵ��ֵ
			size += decompressData(iData, &p[size]);	pMarketData->nYieldToMaturity = iData;		//����������
			size += decompressData(iData, &p[size]);	pMarketData->nHighLimited = pMarketData->nPreClose + iData;		//����������
			size += decompressData(iData, &p[size]);	pMarketData->nLowLimited = pMarketData->nPreClose + iData;		//����������
			memcpy(pMarketData->chPrefix, &p[size], 4);
			size += 4;
			//size += decompressData(iData, &p[size]);    pMarketData->nProcessTime = iData;
		}
		return size;
	}
	
	static int DecompressMarketData_Futures(TDFDefine_MarketData_Futures *pMarketData, char *p)
	{
		int size = 0;
		__int64 iData;
		{	//��������ѹ��
			int i;
			size =  decompressData(iData, &p[size]);	pMarketData->nIdnum  = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nStatus = iData;							//״̬
			size += decompressData(iData, &p[size]);	pMarketData->nTime	 = iData;



			size += decompressData( iData,&p[size]);	pMarketData->iPreOpenInterest = iData;		//��ֲ�
			size += decompressData( iData,&p[size]);	pMarketData->nPreClose= iData;				//ǰ���̼�
			size += decompressData( iData,&p[size]);	pMarketData->nPreSettlePrice = iData;		//������


			size += decompressData(iData, &p[size]);	pMarketData->nOpen = iData + pMarketData->nPreClose;	//���̼�
			size += decompressData(iData, &p[size]);	pMarketData->nHigh = iData + pMarketData->nPreClose;	//��߼�
			size += decompressData(iData, &p[size]);	pMarketData->nLow  = iData + pMarketData->nPreClose;	//��ͼ�
			size += decompressData(iData, &p[size]);	pMarketData->nMatch= iData + pMarketData->nPreClose;	//���¼�


			size += decompressData(iData, &p[size]);	pMarketData->iVolume	= iData;		//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iTurnover	= iData;		//�ɽ��ܽ��


			size += decompressData( iData,&p[size]);	pMarketData->iOpenInterest = iData;		//�ֲ�
			size += decompressData( iData,&p[size]);	pMarketData->nClose= iData;				//���̼�
			size += decompressData( iData,&p[size]);	pMarketData->nSettlePrice = iData;		//�����




			size += decompressData( iData, &p[size]);	pMarketData->nHighLimited = iData;				//��ͣ��
			size += decompressData( iData, &p[size]);	pMarketData->nLowLimited = iData;                //��ͣ��

			size += decompressData( iData, &p[size]);	pMarketData->nPreDelta = iData;			//����ʵ��.
			size += decompressData( iData, &p[size]);	pMarketData->nCurrDelta = iData;	//����ʵ��.



			int nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidPrice[i] = nPrice - iData;
				nPrice = pMarketData->nBidPrice[i];
			}
			nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskPrice[i] = nPrice - iData;
				nPrice = pMarketData->nAskPrice[i];
			}
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidVol[i] = iData;
			}
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskVol[i] = iData;
			}
		}
		return size;
	}
	
	static int DecompressMarketData_Futures(MarketDataForTrade_Future *pMarketData, char *p)
	{
		int size = 0;
		__int64 iData;
		{	//��������ѹ��
			int i;
			size =  decompressData(iData, &p[size]);	pMarketData->nIndex  = iData;
			size += decompressData(iData, &p[size]);	pMarketData->nStatus = iData;							//״̬
			size += decompressData(iData, &p[size]);	pMarketData->nTime	 = iData;



			size += decompressData( iData,&p[size]);	pMarketData->iPreOpenInterest = iData;		//��ֲ�
			size += decompressData( iData,&p[size]);	pMarketData->nPreClose= iData;				//ǰ���̼�
			size += decompressData( iData,&p[size]);	pMarketData->nPreSettlePrice = iData;		//������


			size += decompressData(iData, &p[size]);	pMarketData->nOpen = iData + pMarketData->nPreClose;	//���̼�
			size += decompressData(iData, &p[size]);	pMarketData->nHigh = iData + pMarketData->nPreClose;	//��߼�
			size += decompressData(iData, &p[size]);	pMarketData->nLow  = iData + pMarketData->nPreClose;	//��ͼ�
			size += decompressData(iData, &p[size]);	pMarketData->nMatch= iData + pMarketData->nPreClose;	//���¼�


			size += decompressData(iData, &p[size]);	pMarketData->iVolume	= iData;		//�ɽ�����
			size += decompressData(iData, &p[size]);	pMarketData->iTurnover	= iData;		//�ɽ��ܽ��


			size += decompressData( iData,&p[size]);	pMarketData->iOpenInterest = iData;		//�ֲ�
			size += decompressData( iData,&p[size]);	pMarketData->nClose= iData;				//���̼�
			size += decompressData( iData,&p[size]);	pMarketData->nSettlePrice = iData;		//�����




			size += decompressData( iData, &p[size]);	pMarketData->nHighLimited = iData;				//��ͣ��
			size += decompressData( iData, &p[size]);	pMarketData->nLowLimited = iData;                //��ͣ��

			size += decompressData( iData, &p[size]);	pMarketData->nPreDelta = iData;			//����ʵ��.
			size += decompressData( iData, &p[size]);	pMarketData->nCurrDelta = iData;	//����ʵ��.



			int nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidPrice[i] = nPrice - iData;
				nPrice = pMarketData->nBidPrice[i];
			}
			nPrice = pMarketData->nMatch;
			if(nPrice==0)
				nPrice = pMarketData->nPreClose;
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskPrice[i] = nPrice - iData;
				nPrice = pMarketData->nAskPrice[i];
			}
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nBidVol[i] = iData;
			}
			for(i=0; i<5; i++)
			{
				size += decompressData(iData, &p[size]);
				pMarketData->nAskVol[i] = iData;
			}
		}
		return size;
	}

	static int DecompressIndexData(TDFDefine_IndexData *pMarketData, char *p)
	{
		int size = 0;
		__int64 iData;
		size =  decompressData(iData, &p[size]);	pMarketData->nIdnum = iData;															//���ձ��
		size += decompressData(iData, &p[size]);	pMarketData->nTime  = iData;
		size += decompressData(iData, &p[size]);	pMarketData->nOpenIndex = iData; 							//����ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nHighIndex = iData + pMarketData->nOpenIndex;	//���ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nLowIndex  = iData + pMarketData->nOpenIndex;	//���ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nLastIndex = iData + pMarketData->nOpenIndex;	//����ָ��
		size += decompressData(iData, &p[size]);	pMarketData->iTotalVolume = iData;	//���������Ӧָ���Ľ�������
		size += decompressData(iData, &p[size]);	pMarketData->iTurnover	  = iData;	//���������Ӧָ���ĳɽ����
		size += decompressData(iData, &p[size]);	pMarketData->nPreCloseIndex= iData + pMarketData->nOpenIndex;//ǰ��ָ��
		return size;
	}
	
	static int DecompressIndexData(AD_Index *pMarketData, char *p)
	{
		int size = 0;
		__int64 iData;
		size =  decompressData(iData, &p[size]);	pMarketData->nIndex = iData;															//���ձ��
		size += decompressData(iData, &p[size]);	pMarketData->nTime  = iData;
		size += decompressData(iData, &p[size]);	pMarketData->nOpenIndex = iData; 							//����ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nHighIndex = iData + pMarketData->nOpenIndex;	//���ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nLowIndex  = iData + pMarketData->nOpenIndex;	//���ָ��
		size += decompressData(iData, &p[size]);	pMarketData->nLastIndex = iData + pMarketData->nOpenIndex;	//����ָ��
		size += decompressData(iData, &p[size]);	pMarketData->iTotalVolume = iData;	//���������Ӧָ���Ľ�������
		size += decompressData(iData, &p[size]);	pMarketData->iTurnover	  = iData;	//���������Ӧָ���ĳɽ����
		size += decompressData(iData, &p[size]);	pMarketData->nPreCloseIndex= iData + pMarketData->nOpenIndex;//ǰ��ָ��
		return size;
	}
	static int DecompressTransactionData(TDFDefine_Transaction *pTransaction, char *p, int nItems)
	{
		int i;
		int nSize = 0;
		__int64 iData;
		int nPreTime = 0;
		int nPreIndex = 0;
		int nPrePrice = 0;
		for(i=0; i<nItems; i++)
		{
			nSize += decompressData(iData, &p[nSize]);			//�ɽ�ʱ��
			pTransaction[i].nTime = nPreTime + iData, 
				nPreTime = pTransaction[i].nTime;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nIndex = iData + nPreIndex;	//�ɽ����
			nPreIndex = pTransaction[i].nIndex;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nPrice = nPrePrice + iData;			//�ɽ��۸�
			nPrePrice = pTransaction[i].nPrice;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nVolume = iData;					//�ɽ�����

			pTransaction[i].nTurnover = pTransaction[i].nPrice/10000.0 * pTransaction[i].nVolume;
		}
		return nSize;
	}
	
	static int DecompressTransactionData(AD_Transaction *pTransaction, char *p, int nItems)
	{
		int i;
		int nSize = 0;
		__int64 iData;
		int nPreTime = 0;
		int nPreIndex = 0;
		int nPrePrice = 0;
		for(i=0; i<nItems; i++)
		{
			nSize += decompressData(iData, &p[nSize]);			//�ɽ�ʱ��
			pTransaction[i].nTime = nPreTime + iData, 
				nPreTime = pTransaction[i].nTime;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nIndex = iData + nPreIndex;	//�ɽ����
			nPreIndex = pTransaction[i].nIndex;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nPrice = nPrePrice + iData;			//�ɽ��۸�
			nPrePrice = pTransaction[i].nPrice;

			nSize += decompressData(iData, &p[nSize]);						
			pTransaction[i].nVolume = iData;					//�ɽ�����

			pTransaction[i].nTurnover = pTransaction[i].nPrice/10000.0 * pTransaction[i].nVolume;
		}
		return nSize;
	}

	static int DecompressOrderData(TDFDefine_Order *pOrder, char *p, int nItems)
	{
		int i;
		int nSize = 0;
		__int64 iData;
		int nPreTime = 0;
		int nPreIndex = 0;
		int nPrePrice = 0;
		for(i=0; i<nItems; i++)
		{
			nSize += decompressData(iData, &p[nSize]);	//ί��ʱ��
			pOrder[i].nTime = nPreTime + iData, 
				nPreTime = pOrder[i].nTime;

			nSize += decompressData(iData, &p[nSize]);						
			pOrder[i].nIndex = iData + nPreIndex;		//ί�����
			nPreIndex = pOrder[i].nIndex;

			nSize += decompressData(iData, &p[nSize]);						
			pOrder[i].nPrice = nPrePrice + iData;		//ί�м۸�
			nPrePrice = pOrder[i].nPrice;

			nSize += decompressData(iData, &p[nSize]);						
			pOrder[i].nVolume = iData;					//ί������

			pOrder[i].chType   = p[nSize++];
			pOrder[i].chBSFlag = p[nSize++];
		}
		return nSize;
	}

	static int DecompressOrderQueueData(TDFDefine_OrderQueue *pQueue, char *p, int nItems)
	{
		int i, k;
		int nSize = 0;
		__int64 iData;
		for(i=0; i<nItems; i++)
		{
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nIdnum = iData;	//���ձ��
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nTime = iData;	//�������
			pQueue[i].nSide = p[nSize++];											//��������(A:Ask, B:Bid)
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nPrice = iData;	//�����۸�
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nOrders = iData;	//��������
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nABItems = iData;	//���и���
			for(k=0; k<pQueue[i].nABItems; k++)
			{
				nSize += decompressData(iData, &p[nSize]);	
				pQueue[i].nABVolume[k] = iData;		//��������
			}
		}
		return nSize;
	}

	static int DecompressOrderQueueData(AD_OrderQueue *pQueue, char *p, int nItems,int *pIdnum)
	{
		int i, k;
		int nSize = 0;
		__int64 iData;
		for(i=0; i<nItems; i++)
		{
			nSize += decompressData(iData, &p[nSize]);	pIdnum[i] = iData;	//���ձ��
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nTime = iData;	//�������
			pQueue[i].nSide = p[nSize++];											//��������(A:Ask, B:Bid)
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nPrice = iData;	//�����۸�
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nOrders = iData;	//��������
			nSize += decompressData(iData, &p[nSize]);	pQueue[i].nABItems = iData;	//���и���
			for(k=0; k<pQueue[i].nABItems; k++)
			{
				nSize += decompressData(iData, &p[nSize]);	
				pQueue[i].nABVolume[k] = iData;		//��������
			}
		}
		return nSize;
	}

	static char *DecompressABQueue_FAST(char *p)
	{
		char *pData = new char[16*1024];
		memset(pData, 0, 16*1024);
		int offset = 0;
		int size = 0;
		int i, j;
		__int64 iDelta;
		TDFDefine_OrderQueue_FAST *pOrder = (TDFDefine_OrderQueue_FAST*)pData;
		offset += sizeof(TDFDefine_OrderQueue_FAST);
		size = decompressData(iDelta, &p[size]); pOrder->nIdnum = iDelta;
		size += decompressData(iDelta, &p[size]);	pOrder->nTime = iDelta;//����ʱ��
		pOrder->nSide = p[size++];						//��������'B':Bid 'A':Ask��
		pOrder->nImageStatus = p[size++];				//����״̬
		pOrder->nNoPriceLevel = p[size++];				//�����ļ�λ��
		TDFDefine_OrderQueue_FAST_Operate *pOper;
		TDFDefine_OrderQueue_FAST_OperateItem *pItem;
		for(i=0; i<pOrder->nNoPriceLevel; i++)
		{
			pOper = (TDFDefine_OrderQueue_FAST_Operate*)&pData[offset];
			offset += sizeof(TDFDefine_OrderQueue_FAST_Operate);
			pOper->nPriceOperate = p[size++];										// 1.add , 2 Update ,3 Delete  0. absent 
			size += decompressData(iDelta, &p[size]);	pOper->nPrice = iDelta;		//�۸�
			size += decompressData(iDelta, &p[size]);	pOper->nNumOrders = iDelta;	//ί�б���
			size += decompressData(iDelta, &p[size]);	pOper->nOqLevel = iDelta;	//��ǰ��λ��Ҫ�����Ķ�����
			for(j=0; j<pOper->nOqLevel; j++)
			{
				pItem = (TDFDefine_OrderQueue_FAST_OperateItem*)&pData[offset];
				offset += sizeof(TDFDefine_OrderQueue_FAST_OperateItem);
				pItem->nOperate = p[size++];											//������ʽ: 1 Add ,2 Update,3 Delete
				size += decompressData(iDelta, &p[size]);	pItem->nEntryID = iDelta;	//����ID:  Oreder Queue Operator Entry ID
				size += decompressData(iDelta, &p[size]);	pItem->nQty = iDelta;		//��������
			}
		}
		return pData;
	}

	////////////////////////////////////////////////////////////////////////////
	//�ӽ��ְ�ȫ�������ݣ����ȫ�����ͳɹ�������True�����򷵻�FALSE
	static bool SafeSendData(SOCKET sock,char* buff, int nLen,int nFlags = 0)
	{
		int nSend = 0;
		int i = 0;
		while(nSend < nLen)
		{
			i = send(sock,buff+nSend,nLen-nSend,nFlags);
			if(i>0)
			{
				nSend +=i;
			}else if(i==0)
			{
				// �Է��Ѿ��ر����׽��֡�
				return false;			
			}
			else
			{
				//����
				return false; //�������
			}
		}
		return true;
	}
	//���׽Ӱ�ȫ�������ݣ����ȫ�����ճɹ����򷵻�TRUE,���򷵻�False
	static bool SafeRecvData(SOCKET sock,char* buff,int nLen,int nFlags = 0)
	{
		int nRecv =0;
		int i =0;
		while(nRecv < nLen)
		{
			i = recv(sock,buff+nRecv,nLen-nRecv,nFlags);
			if(i>0)
			{
				nRecv +=i;
			}else if (i==0)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	static SOCKADDR_IN MakeSockAddr(LPTSTR lpIP,unsigned short nPort)
	{
		SOCKADDR_IN ret;
		memset(&ret,0,sizeof(ret));
		ret.sin_family = AF_INET;
		ret.sin_addr.S_un.S_addr = inet_addr(lpIP);
		ret.sin_port = htons(nPort);
		return ret;
	}

	CDecompressFuncitons(void);
	~CDecompressFuncitons(void);
};
