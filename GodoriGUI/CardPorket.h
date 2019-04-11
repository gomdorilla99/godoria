#pragma once
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <list>
#include "Card.h"

using namespace std;



struct SampleSort
{

public:
	SampleSort() {};
	bool operator ()  (const CCard *pAarg1, const CCard* pArg2)
	{
		return pAarg1->mCardID < pArg2->mCardID;
	};
};

class CCardPorket
{
private:
	mutex mtx;
	list<CCard*>mList;	//private로 보호하고, Mutex로 싸야한다. 
public:
	void Remove(CCard *pCard);
	void RemoveIf(CCard * pCard);
	void Clear();
	void PrintCardListSort();	// 점수와 함께 카드 리스트 보여주기 
	int GetCardNumber(){		return mList.size();}
	void Lock() { mtx.lock(); };
	void UnLock() { mtx.unlock(); };
	CCard * Back()
	{
		CCard *pRetCard;
		//mtx.lock();
		pRetCard = mList.back();
		//mtx.unlock();
		return pRetCard;  
	}
	list<CCard *>::iterator Begin()
	{
		list<CCard *>::iterator pRetCard;
		//mtx.lock();
		pRetCard = mList.begin();
		//mtx.unlock();
		return pRetCard;
	};

	list<CCard *>::iterator End()
	{
		list<CCard *>::iterator pRetCard;
		//mtx.lock();
		pRetCard = mList.end();
		//mtx.unlock();
		return pRetCard;
	};

	CCardPorket& operator= (CCardPorket & rhs)
	{
		mtx.lock();
		mList.clear();
		for_each(rhs.mList.begin(), rhs.mList.end(), [&](CCard *a){ mList.push_back(a); });
		mtx.unlock();
		return *this;
	};

	void Sort()
	{		
		mList.sort(SampleSort());
	};

	
	int GetCount()
	{
		try {

			int retsize;
		//	mtx.lock();
			retsize = mList.size();
		//	mtx.unlock();
			return retsize;
		}
		catch (std::system_error) {
		//	mtx.unlock();
			return 0;
		}
	}
	CCard *GetItem(unsigned int index)
	{
		try {
		//	mtx.lock();
			if (mList.size() < index + 1)
				return nullptr;
			list<CCard *>::iterator iter = mList.begin();
			advance(iter, index);
		//	mtx.unlock();
			return *iter;
		}
		catch (std::system_error) {
		//	mtx.unlock();
			return nullptr;
		}
	}

	int InsertHeader(CCard *pCard)
	{
		mtx.lock();
		mList.push_front(pCard);
		mtx.unlock();
		return 0;
	};

	int InsertTail(CCard *pCard)
	{
		mtx.lock();
		mList.push_back(pCard);
		mtx.unlock();
		return 0;
	};
	
	CCardPorket();
	~CCardPorket()
	{
		mtx.lock();
		mList.clear();
		mtx.unlock();
	};

	int CalculatePoint();
	void SimpleList();
	int Push(CCard *pCard)
	{
		mtx.lock();
		if (pCard->mCardID < 55)
		{
			mList.push_back(pCard);
		}
		mtx.unlock();
		return 0;
	};
};

