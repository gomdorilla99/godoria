#include "stdafx.h"
#include "CardPorket.h"

int CCardPorket::CalculatePoint()
{
	int FinalPoint = 0;
	int PointOfGuang = 0;
	int ThPointOfGuang = 2;
	int PointOfP = 0;
	int ThPointOfP = 9;
	int PointOfGodori = 0;	//Godory = 5점
	int ThGodori = 5; // 새 5마리 이상
	int PointOfYoel = 0;
	int PointOfBand = 0;
	int ThBand = 3;		// 3개 이상
	int ThTotalBand = 4;
	int NumOfBiGuang = 0; 
	mtx.lock();
	list<CCard *>::iterator iter = mList.begin();
	while (iter != End())
	{
		PointOfP += (*iter)->mCharic.mPValue;
		PointOfGuang += (*iter)->mCharic.mGuang;
		PointOfBand += (*iter)->mCharic.mTheBand;
		PointOfGodori += (*iter)->mCharic.mAnimalCharic;
		if ((*iter)->mCardID == 44)
			NumOfBiGuang = 1;
		iter++;
	}
	mtx.unlock();
	
	
	TRACE("카드 개수 : %d \r\n", GetCount());

	//열 점수 
	if ((PointOfGuang>>16) > 4)
	{
		PointOfYoel = (PointOfGuang >> 16) - 4;
		TRACE("Yeol : %d\r\n", PointOfYoel);
	}
	FinalPoint += PointOfYoel;
	
	// 10장 1점, 1점씩
	if ((PointOfP & 0xFFFF) > ThPointOfP)
	{
		PointOfP = (PointOfP & 0xFFFF)  - ThPointOfP;
		TRACE("P : %d\r\n", PointOfP);
	}
	else
		PointOfP = 0;

	FinalPoint += PointOfP;

	// 3광 3점
	if ((PointOfGuang & 0xFFFF) > ThPointOfGuang)
	{
		if((PointOfGuang  & 0xFFFF) == 5)
		{
			PointOfGuang = 15; //5광 15점
		}
		else if (PointOfGuang == 4)
		{
			PointOfGuang = PointOfGuang;
		}
		else // 3광일때
		{
			PointOfGuang = PointOfGuang - NumOfBiGuang; // 3광 3점, 4광 4점
		}
		TRACE("Guang : %d\r\n", PointOfGuang);
	}
	else
	{
		PointOfGuang = 0;
	}
	FinalPoint += PointOfGuang;

	if ((PointOfGodori >> 16) == 5)
	{
		//고도리
		FinalPoint += 5;
		TRACE("Godori : %d\r\n", 5);
	}

	//
	int totalBand = NUMOF_BAND_RED(PointOfBand) + NUMOF_BAND_BLUE(PointOfBand) + NUMOF_BAND_GLASS(PointOfBand);
	int BandPoint=0;
	if (NUMOF_BAND_RED(PointOfBand) == 3)
	{
		BandPoint += 3;
		TRACE("HongDan : %d\r\n", BandPoint);
	}
	if (NUMOF_BAND_BLUE(PointOfBand) == 3)
	{
		BandPoint += 3;
		TRACE("ChungDan : %d\r\n", BandPoint);
	}
	if (NUMOF_BAND_GLASS(PointOfBand) == 3)
	{
		BandPoint += 3;
		TRACE("ChoiDan: %d\r\n", BandPoint);
	}
	if (totalBand > 4)
	{
		BandPoint += totalBand - 4;
		TRACE("Band: %d\r\n", totalBand - 4);
	}
	FinalPoint += BandPoint;

	int times = 1;
	//피박 - 상대의 피가 5개 이하일 때ㅐ
	//광박 - 광의 점수가 있을때 상대방이 광이 없다면
	//고박 - 고를 불렀으나 졌을떄
	//흔들기 - 흔들기 선언후 
	TRACE("x %d\r\n", times);
	TRACE("Total point : %d\r\n", FinalPoint * times);
	//Card List 합계	
	return FinalPoint;
}


void CCardPorket::SimpleList()
{
	//PrintCard();
}

void CCardPorket::PrintCardListSort()
{
	// 카드 리스트 열거
}

CCardPorket::CCardPorket()
{

}

void CCardPorket::Remove(CCard *pCard)
{
	mtx.lock();
	mList.remove(pCard);
	mtx.unlock();
}

void CCardPorket::RemoveIf(CCard *pCard)
{
	mtx.lock();
	mList.remove_if([&](CCard * a) {
		return a->mCharic.mTheMonth == pCard->mCharic.mTheMonth;
	});
	mtx.unlock();
}

void CCardPorket::Clear()
{
	//Lock
	mtx.lock();
	if(mList.size()>0)
		mList.clear();
	//unLock
	mtx.unlock();
}
