#include "stdafx.h"
#include "Card.h"
#include <stdlib.h>
#include <string.h>



CCard::CCard(int ID, int P, int Ani, int theMonth, int theBand, int Guang, wchar_t *Name)
{
	mCardID = ID;
	mCharic.mPValue = P;
	mCharic.mAnimalCharic = Ani;
	mCharic.mTheMonth = theMonth;
	mCharic.mTheBand = theBand;
	mCharic.mGuang = Guang;
	wcscpy_s(mCharic.mName,30, Name);
}

CCard::CCard()
{
}
