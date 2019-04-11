#include "stdafx.h"
#include "CardPorket.h"

int CCardPorket::CalculatePoint()
{
	int FinalPoint = 0;
	int PointOfGuang = 0;
	int ThPointOfGuang = 2;
	int PointOfP = 0;
	int ThPointOfP = 9;
	int PointOfGodori = 0;	//Godory = 5��
	int ThGodori = 5; // �� 5���� �̻�
	int PointOfYoel = 0;
	int PointOfBand = 0;
	int ThBand = 3;		// 3�� �̻�
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
	
	
	TRACE("ī�� ���� : %d \r\n", GetCount());

	//�� ���� 
	if ((PointOfGuang>>16) > 4)
	{
		PointOfYoel = (PointOfGuang >> 16) - 4;
		TRACE("Yeol : %d\r\n", PointOfYoel);
	}
	FinalPoint += PointOfYoel;
	
	// 10�� 1��, 1����
	if ((PointOfP & 0xFFFF) > ThPointOfP)
	{
		PointOfP = (PointOfP & 0xFFFF)  - ThPointOfP;
		TRACE("P : %d\r\n", PointOfP);
	}
	else
		PointOfP = 0;

	FinalPoint += PointOfP;

	// 3�� 3��
	if ((PointOfGuang & 0xFFFF) > ThPointOfGuang)
	{
		if((PointOfGuang  & 0xFFFF) == 5)
		{
			PointOfGuang = 15; //5�� 15��
		}
		else if (PointOfGuang == 4)
		{
			PointOfGuang = PointOfGuang;
		}
		else // 3���϶�
		{
			PointOfGuang = PointOfGuang - NumOfBiGuang; // 3�� 3��, 4�� 4��
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
		//����
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
	//�ǹ� - ����� �ǰ� 5�� ������ ����
	//���� - ���� ������ ������ ������ ���� ���ٸ�
	//��� - �� �ҷ����� ������
	//���� - ���� ������ 
	TRACE("x %d\r\n", times);
	TRACE("Total point : %d\r\n", FinalPoint * times);
	//Card List �հ�	
	return FinalPoint;
}


void CCardPorket::SimpleList()
{
	//PrintCard();
}

void CCardPorket::PrintCardListSort()
{
	// ī�� ����Ʈ ����
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
