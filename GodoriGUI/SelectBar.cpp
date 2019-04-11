#include "StdAfx.h"
#include "SelectBar.h"
#include "resource.h"       // 주 기호입니다.
#include "math.h"
#include "game.h"
#define ZOOMMAX 1.5
#define ALLGROUP 4

using namespace std;

CSelectBar::CSelectBar(void)
{
	nItem = 12;
	mMousePoint.x=0;
	mMousePoint.y=0;
	BackBrush.CreateSolidBrush(RGB(244,194,194));
	BackShadowBrush.CreateSolidBrush(RGB(15,55,18));

	NULLPen.CreateStockObject(NULL_PEN);
	YELLOPen.CreatePen(PS_SOLID,2, RGB(255, 80,80));
	GREENPen.CreatePen(PS_SOLID, 2, RGB(244, 194,194));
	curBitmap.LoadBitmap(IDB_CARDFACE); // 비트맵 객체에 리소스 연결
	CardBack.LoadBitmap(IDB_CARDBACK);
	Octopus.LoadBitmapW(IDB_OCTOPUS);
	RedCard.CreatePatternBrush(&CardBack);
	OCTOPUS.CreatePatternBrush(&Octopus);

	CString greating;
	
}

CSelectBar::~CSelectBar(void)
{
	curBitmap.DeleteObject();
}
void CSelectBar::SetOpsit()
{
	option_opsit=1;
	mPlayerId = ID_PLAYER2;

	CString greating;
	
	greating = _T("안녕하세요!");
	
	AddMessege(greating, 100);

}
int CSelectBar::SetPlayerTurn(CDC *pDC, bool bMyTurn)
{
	CPen *pOldPen;
	
	if (bMyTurn)
	{
		isMyTurn = true;
		pOldPen = pDC->SelectObject(&YELLOPen);
		pDC->MoveTo(mRegion.left + 1, mRegion.top + 1);
		pDC->LineTo(mRegion.right - 1, mRegion.top + 1);
		pDC->LineTo(mRegion.right - 1, mRegion.bottom - 1);
		pDC->LineTo(mRegion.left + 1, mRegion.bottom - 1);
		pDC->LineTo(mRegion.left + 1, mRegion.top + 1);
		pDC->SelectObject(pOldPen);
	}
	else
	{
		isMyTurn = false;
	/*
		pOldPen = pDC->SelectObject(&GREENPen);
		pDC->MoveTo(mRegion.left + 1, mRegion.top + 1);
		pDC->LineTo(mRegion.right - 1, mRegion.top + 1);
		pDC->LineTo(mRegion.right - 1, mRegion.bottom - 1);
		pDC->LineTo(mRegion.left + 1, mRegion.bottom - 1);
		pDC->LineTo(mRegion.left + 1, mRegion.top + 1);
		pDC->SelectObject(pOldPen);*/
	}
	
	return 0;
}

void CSelectBar::DrawToast(int x, int y, CDC *pDC, int oposit, CString Message)
{
	CFont *pOldFont;


	pOldFont = pDC->SelectObject(&mBigFont);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(x - 1, y, Message);
	pDC->TextOutW(x, y - 1, Message);
	pDC->TextOutW(x, y + 1, Message);
	pDC->TextOutW(x + 1, y, Message);
	pDC->SetTextColor(RGB(255, 255, 220));
	pDC->TextOutW(x, y, Message);

	pDC->SelectObject(pOldFont);
}

int CSelectBar::DrawBaseCard(CDC *pDC,int group,int index)
{	
	//CPoint refPoint;
	CPoint reactPoint;
	CBitmap *oldBitmap;
	CBrush *pOldBrush;
	CPen *pOldPen;
	CPlayer *mpPlayer = GetPlayer();

	unsigned int distance;
	double factor;
	int nCard;

	unsigned int step=35;
	if(group==3){
		if(nItem>12)
			step = 35*12/nItem;
	}
	else
	{
		if(nItem>5)
			step = 35*12/nItem;
	}
	
	CCard *pCard = nullptr;
	switch(group)
	{
	case 0:
		pCard =mpPlayer->mPrime.GetItem(index);
		nCard =mpPlayer->mPrime.GetCount();
		break;
	case 1:
		pCard =mpPlayer->mCharictor.GetItem(index);
		nCard =mpPlayer->mCharictor.GetCount();
		break;
	case 2:
		pCard =mpPlayer->mBand.GetItem(index);
		nCard =mpPlayer->mBand.GetCount();
		break;
	case 3:
		pCard =mpPlayer->mPi.GetItem(index);
		nCard =mpPlayer->mPi.GetCount();
		break;
	case 4:
		pCard = mpPlayer->mOnFloor.GetItem(index);
		nCard = mpPlayer->mOnFloor.GetCount();
		break;
	}
	CRect temp;
	oldBitmap=MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택
	
	refPoint[index].x = mRegion.left+(step*((index % 5)*2+1))/4 + group*mRegion.Width()/5;
	refPoint[index].x += (index / 5) * 5;
	if(option_opsit){
		refPoint[index].y = mRegion.top+45;
		refPoint[index].y += (index / 5) * (60/ (1+((nCard-1) / 5)));
	}else
	{
		refPoint[index].y = mRegion.top+57;
		refPoint[index].y += (index / 5) *  (60 / (1+((nCard-1) / 5)));
	}
	factor = 1.;
	distance = (refPoint[index].x - mMousePoint.x)*(refPoint[index].x - mMousePoint.x) +
				   (refPoint[index].y - mMousePoint.y)*(refPoint[index].y - mMousePoint.y);

	

	if(sqrt(distance+20.) < 10)
	{
		reactPoint.x = refPoint[index].x- 300./(sqrt((double)distance)+30);;
		reactPoint.y = refPoint[index].y;
	}else
	{
		factor = 1.;
		reactPoint.x = refPoint[index].x;
		reactPoint.y = refPoint[index].y;
	}
	
	temp.left = reactPoint.x;
	temp.top = reactPoint.y-(50/2)*140/88;
	temp.right = reactPoint.x+(50);
	temp.bottom = reactPoint.y+(50/2)*140/88;

	//pDC->RoundRect(temp.left-1,temp.top-1,temp.right+1,temp.bottom+1,2,2);	
	pDC->StretchBlt(temp.left+1,temp.top+1,temp.Width(),temp.Height(),&MemoryDC,
		(pCard->mCardID % 8)*97,(pCard->mCardID / 8)*145,88,140,SRCCOPY);

	pDC->MoveTo(temp.right, temp.top);
	pDC->LineTo(temp.right, temp.bottom);
	pDC->LineTo(temp.left, temp.bottom);

	/*
	if(index==0){
		CString txt;
		txt.Format(_T("%d"),nCard);
		DrawPopText(temp.left-14,temp.top-2,pDC, option_opsit,txt)	;
	}
	*/

	MemoryDC.SelectObject(oldBitmap);
	return 0;
}

void CSelectBar::DrawFlatText(int x, int y, CDC *pDC, int oposit, CString Message)
{
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(x, y, Message);
}
void CSelectBar::DrawPopText(int x,int y,CDC *pDC, int oposit,CString Message)	
{

	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOut(x,y-1,Message);
	pDC->TextOut(x,y+1,Message);
	pDC->TextOut(x-1,y,Message);
	pDC->TextOut(x+1,y,Message);
	//pDC->SetTextColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(x,y,Message);
}

CPlayer *CSelectBar::GetPlayer()
{
	auto & Player1 = CPlayer::GetInstance(ID_PLAYER1);
	auto & Player2 = CPlayer::GetInstance(ID_PLAYER2);
	
	CPlayer *pPlayer;
	if (mPlayerId == ID_PLAYER1)
	{
		pPlayer = (CPlayer *)&Player1;
		option_opsit = 0;
	}
	else if (mPlayerId == ID_PLAYER2)
	{
		pPlayer = (CPlayer *)&Player2;
		option_opsit = 1;
	}
	else
	{
		pPlayer = nullptr;
	}
	return pPlayer;
}

unsigned int CSelectBar::GetCardLocation(int index, long int &x, long int &y)
{
	unsigned int step = mRegion.Width() / (nItem + 1);
	if (step > 88) step = 88;
	x = mRegion.left + (step*(index * 2 + 1)) / 2;
	y = (mRegion.top + 2 * mRegion.bottom) / 3;
	return step;
}

int CSelectBar::DrawCard(CDC *pDC,int index)
{	
	//CPoint refPoint;
	CPoint reactPoint;
	CBitmap *oldBitmap;
	CBrush *pOldBrush;
	CPen *pOldPen;
	unsigned int distance;
	double factor;
	unsigned int step;

	CPlayer *pPlayer = GetPlayer();
	auto & Diller = CPlayer::GetInstance(ID_DILLER);

	CCard *pCard;
	if(index > pPlayer->mInHand.GetCount())
	{
		return -1;
	}
	
	pCard = pPlayer->mInHand.GetItem(index);
	if (!pCard)
	{
		return -1;
	}

	step = GetCardLocation(index, refPoint[index].x, refPoint[index].y);
	CRect temp;
	oldBitmap=MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택
	factor = 1.;
	distance = (refPoint[index].x - mMousePoint.x)*(refPoint[index].x - mMousePoint.x) +
				   (refPoint[index].y - mMousePoint.y)*(refPoint[index].y - mMousePoint.y);

	for(int s=0;s<2;s++)	// 그림자 그리기 위해 두번 실행 
	{
		if(!option_opsit)	
		{
			
			if(sqrt(distance+20.) < 100)
			{
				if(distance <2000)
				{
					factor = ZOOMMAX;
				}
				else
				{
					factor = 100./sqrt(distance+20.);
				}

				if(factor>ZOOMMAX) factor = ZOOMMAX;
				if(refPoint[index].x > mMousePoint.x )
					reactPoint.x = refPoint[index].x + sqrt((double)distance)/1000.;
				else
					reactPoint.x = refPoint[index].x - sqrt((double)distance)/1000.;
				reactPoint.y = refPoint[index].y - 2000./(sqrt((double)distance)+30);
			}
			else
			{
				factor = 1.;
				reactPoint.x = refPoint[index].x;
				reactPoint.y = refPoint[index].y - 2000./sqrt((double)distance+30);
			}
			if(s==1){	
				//tempList = CDiler::GetInstance()->mList->Matching(pPlayer->mInHand.GetItem(index));
				
				temp.top = reactPoint.y-(step/2)*140/88*factor;
				temp.bottom = reactPoint.y+(step/2)*140/88*factor;
				temp.left = reactPoint.x-(step/2)*factor;
				temp.right = reactPoint.x+(step/2)*factor;
								
				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				
				if (!pCard->bomb)
				{
					if (pCard->Value == ID_PLAYER1)
					{
						bf.AlphaFormat = AC_SRC_OVER;  // use source alpha 
						CString Msg(_T("▼"));
						//Msg.Format(_T("▼"), pCard->Value);
						bf.SourceConstantAlpha = 100 * factor + 100;  // opaque (disable constant alpha)
						DrawPopText(temp.left, temp.top - 15, pDC, option_opsit, Msg);
					}
					else
					{
						bf.AlphaFormat = AC_SRC_OVER;  // use source alpha 
						bf.SourceConstantAlpha = 80;  // opaque (disable constant alpha)
					}
				}
				else if(pCard->bomb == 3)
				{
					bf.AlphaFormat = AC_SRC_OVER;  // use source alpha 
					CString Msg;
					if (pCard->Value == ID_PLAYER1)
					{
						Msg = _T("폭탄");
					}
					else
					{

						temp.top = reactPoint.y - (step / 2) * 140 / 88 * factor + shake_y;
						temp.bottom = reactPoint.y + (step / 2) * 140 / 88 * factor + shake_y;
						temp.left = reactPoint.x - (step / 2)*factor + shake_x;
						temp.right = reactPoint.x + (step / 2)*factor + shake_x;

						Msg = _T("흔들기");
					}
					bf.SourceConstantAlpha = 100 * factor + 100;  // opaque (disable constant alpha)
					DrawPopText(temp.left, temp.top - 15, pDC, option_opsit, Msg);
				}
				//pDC->RoundRect(temp.left-2,temp.top-2,temp.right+2,temp.bottom+2,8,8);	
				
			//	pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
			//		(pCard->mCardID % 8)*73,(pCard->mCardID / 8)*120,73,120,bf);  	
				pDC->AlphaBlend(temp.left + 1, temp.top + 1, temp.Width() , temp.Height() , &MemoryDC,
					((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, bf);
				pDC->MoveTo(temp.right, temp.top);
				pDC->LineTo(temp.right, temp.bottom);
				pDC->LineTo(temp.left, temp.bottom);
			
			}
			else /* 아래 비친 음영 그린다. */
			{
				//pOldPen = pDC->SelectObject(&NULLPen);
				pOldBrush  = pDC->SelectObject(&BackShadowBrush);
				reactPoint.y = refPoint[index].y + 2000./sqrt((double)distance+30);
				temp.left = reactPoint.x-(step/2)*factor;
				temp.top = reactPoint.y-(step/2)*140/88*factor;
				temp.right = reactPoint.x+(step/2)*factor;
				temp.bottom = reactPoint.y+(step/2)*140/88*factor;
				
				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
				bf.SourceConstantAlpha = 30;  // opaque (disable constant alpha)

				//pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
				//	(pCard->mCardID % 8)*73,(pCard->mCardID / 8)*120,73,120,bf); 

				pDC->AlphaBlend(temp.left + 1, temp.top + 1, temp.Width() , temp.Height() , &MemoryDC,
					((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, bf);
				
			}
		}else // 상대 플레이어면 
		{
			if(sqrt(distance+20.) < 100)
			{
				
				if(refPoint[index].x > mMousePoint.x )
					reactPoint.x = refPoint[index].x + (int)sqrt((double)distance)/1000.;
				else
					reactPoint.x = refPoint[index].x - (int)sqrt((double)distance)/1000.;

				reactPoint.y = refPoint[index].y + (int)2000./(sqrt((double)distance)+30);
			}else
			{
				factor = 1.;
				reactPoint.x = refPoint[index].x;
				reactPoint.y = refPoint[index].y + (int)2000./sqrt((double)distance+30);
			}
			if(s==1){ /* 상대 카드 뒷면 */
				pOldBrush  = pDC->SelectObject(&RedCard);
				temp.left = reactPoint.x- (int)(step/2)*factor;
				temp.top = reactPoint.y- (int)(step/10)*140/88*factor + 35;
				temp.right = reactPoint.x+ (int)(step/2)*factor;
				temp.bottom = reactPoint.y+ (int)(step/10)*140/88*factor + 35;
			//	pOldPen = nullptr;
			//	pDC->RoundRect(temp.left - 2, temp.top - 2, temp.right + 2, temp.bottom + 2, 8, 8);
				CString Msg;
				Msg.Format(_T("%d"), index+1);
				pDC->Ellipse(temp.left + 2, temp.top - 2, temp.left + 22, temp.top - 2 + 20);
				pDC->TextOutW(temp.left + 4, temp.top, Msg);
				pDC->SelectObject(pOldBrush);

			}
		}
	}
	
	MemoryDC.SelectObject(oldBitmap);
	return 0;
}

extern double PIDControl(control_t& Cont, double target);

int CSelectBar::DrawFace(CDC *pDC)
{
	int i;
	int x,y;
	int index;
	CBrush *pOldBrush;
	unsigned int distance;
	double factor;

	auto & Diller = CDiler::GetInstance(ID_PLAYER2);
	CPlayer *pPlayer = GetPlayer();
			
    BackDC.SetBkMode(TRANSPARENT);
	BackDC.SetStretchBltMode(HALFTONE);
	BackDC.SetTextColor(RGB(0,0,0));
	unsigned int step;

	CString Num;
	
	step = mRegion.Width() / (nItem+1);
	if (step > 50) step= 50;
	if (step < 2) step = 2;
	CRect temp;

	if(mMousePoint.x ==0)
		index = 0;
	else
		index = (mMousePoint.x - mRegion.left) / step;
	
	if (index > nItem-1)index = nItem-1;
	if (index < 0) index = 0;
	pOldBrush = BackDC.SelectObject(&BackBrush);
	BackDC.Rectangle(mRegion.left-1,mRegion.top-1,mRegion.right+1,mRegion.bottom+1);
	BackDC.SelectObject(pOldBrush);
	
	if (option_opsit)
	{
		BackDC.StretchBlt(mRegion.right - 314, mRegion.top, 314, mRegion.Height() , &OctopusDC,
			0, 0, 314, mRegion.Height(), SRCCOPY);
	}
	

	for (int j = 0; j < ALLGROUP; j++)
	{
		int nCard;
		switch (j) {
		case 0:
			nCard = pPlayer->mPrime.GetCount();
			break;
		case 1:
			nCard = pPlayer->mCharictor.GetCount();
			break;
		case 2:
			nCard = pPlayer->mBand.GetCount();
			break;
		case 3:
			nCard = pPlayer->mPi.GetCount();
			break;
		case 4:
			nCard = pPlayer->mOnFloor.GetCount();
			break;
		}
		CString StrMsg;
		CString SubStrMsg;
		
		switch (j)
		{
		case 0:
			if (pPlayer->mScore.PointOfGuang > 0)
			{
				StrMsg.Format(_T("광%d점"), pPlayer->mScore.PointOfGuang);
			}
			else
			{
				if (pPlayer->mStatic.NumOfGuang == 0)
					StrMsg = _T("광박");
				else
					StrMsg.Format(_T("광%d개"), pPlayer->mStatic.NumOfGuang);

			}
			break;
		case 1:
			if (pPlayer->mScore.PointOfYoel > 0)
			{
				StrMsg.Format(_T("열%d점"), pPlayer->mScore.PointOfYoel);
			}
			else
			{
				if(pPlayer->mStatic.NumOfYeol>0)
					StrMsg.Format(_T("열%d개"), pPlayer->mStatic.NumOfYeol);
			}

			if (pPlayer->mScore.Godori > 0)
			{
				if (mStatus.GoDori == 0)
				{
					mStatus.GoDori = 1;
					SubStrMsg = _T("아싸 고도리");
					AddMessege(SubStrMsg, 40);
				}
			}


			break;
		case 2:
			if (pPlayer->mScore.PointOfBand > 0)
			{
				StrMsg = _T("");
				if (pPlayer->mScore.PointHongDan)
				{
					SubStrMsg.Format(_T("홍단"), pPlayer->mScore.PointOfBand);
					StrMsg += SubStrMsg;
					if (pPlayer->mStatus.HongDan != mStatus.HongDan)
					{
						mStatus.HongDan = pPlayer->mStatus.HongDan ;
						SubStrMsg = _T("홍단");
						AddMessege(SubStrMsg, 40);
					}
				}
				if (pPlayer->mScore.PointChungDan)
				{
					SubStrMsg.Format(_T("청단"), pPlayer->mScore.PointOfBand);
					StrMsg += SubStrMsg;
					
					if (pPlayer->mStatus.ChungDan != mStatus.ChungDan )
					{
						mStatus.ChungDan = pPlayer->mStatus.ChungDan;
						SubStrMsg = _T("아싸 청단!;청단했지렁");
						AddMessege(SubStrMsg, 40);
					}
				}
				if (pPlayer->mScore.PointChoDan)
				{
					SubStrMsg.Format(_T("초단"), pPlayer->mScore.PointOfBand);
					StrMsg += SubStrMsg;
					if (pPlayer->mStatus.ChoDan != mStatus.ChoDan)
					{
						mStatus.ChoDan = pPlayer->mStatus.ChoDan;
						SubStrMsg = _T("아싸 초단;초단했지렁");
						AddMessege(SubStrMsg, 40);
					}
				}
				StrMsg.Format(_T("띠%d점"), pPlayer->mScore.PointOfBand);
				
			}
			else
			{
				if (pPlayer->mStatic.NumOfYeol > 0)
					StrMsg.Format(_T("띠%d개"), pPlayer->mStatic.NumOfBand);
			}
			break;
		case 3:
			if (pPlayer->mScore.PointOfP > 0)
			{
				StrMsg.Format(_T("피%d점"), pPlayer->mScore.PointOfP);
			}
			else
			{
				if (pPlayer->mStatic.NumOfP > 4)
					StrMsg.Format(_T("피%d개"), pPlayer->mStatic.NumOfP);
				else
					StrMsg = _T("피박");
			}
			break;
		default:
			break;
		}

		if (nCard > 0)
		{
			for (i = 0; i < nCard; i++)
			{
				DrawBaseCard(&BackDC, j, i);
			}

			if (!option_opsit)
			{
				if (StrMsg.GetLength() > 0)
					DrawFlatText(refPoint[0].x, refPoint[0].y - 50, &BackDC, 0, StrMsg);
				//else
				//	DrawPopText(refPoint[0].x, refPoint[0].y - 50, &BackDC, 0, _T("테스트문자"));
			}
			else
			{
				if (StrMsg.GetLength() > 0)
					DrawFlatText(refPoint[0].x, refPoint[0].y + 40, &BackDC, 0, StrMsg);
				//else
				//	DrawPopText(refPoint[0].x, refPoint[0].y + 40, &BackDC, 0, _T("테스트문자"));
			}
		}

	}

	shake_x = rand() % 2 - 1;
	shake_y = 0;// rand() % 2 - 1;
	
	for(i=0;i<index;i++)
	{

		DrawCard(&BackDC,i);
	}
	for(i=nItem-1;i>=index;i--)
	{
		DrawCard(&BackDC,i);
	}
	
	CString txt;

	CFont *pOldFont = BackDC.SelectObject(&mBaseFont);
	// 현황
//	txt.Format(_T("%0d고 %0d뻑 %0d점"), nGo, nNG, nPoint);
//	DrawPopText(mRegion.right - 150, mRegion.top, &BackDC, 0, txt);
//	DrawPopText(mRegion.right - 150, mRegion.bottom - 20, &BackDC, 0, txt);

	// 선
	txt.Format(_T("Total : %d점" ),pPlayer->mScore.SemiFinalPoint);
	if (option_opsit)
	{
		DrawPopText(mRegion.left + 10, mRegion.bottom - 20, &BackDC, 0, txt);
	}
	else
	{
		DrawPopText(mRegion.left + 10, mRegion.top, &BackDC, 0, txt);
	}
	

	//점수
//	txt.Format(_T("사용자 : 100만 3200원"), nGo, nNG, nPoint);
//	DrawPopText(mRegion.left + 250, mRegion.top, &BackDC, 0, txt);
//	DrawPopText(mRegion.left + 250, mRegion.bottom - 20, &BackDC, 0, txt);

	/*if (mTurn == ID_PLAYER1)
	{
		txt.Format(_T("****"));
		DrawPopText(mRegion.left + 50, mRegion.top, &BackDC, 0, txt);
	}
	else
	{
		txt.Format(_T("****"));
		DrawPopText(mRegion.left + 50, mRegion.bottom - 20, &BackDC, 0, txt);
	}*/

	BackDC.SelectObject(pOldFont);

	CMessege_t *pMessage;
	if (mMessageQ.size() > 0)
	{
		pMessage = mMessageQ.front();
		int x, y;
		x = pMessage->cont_x.current_val;
		y = pMessage->cont_y.current_val;
		double target_x = mRegion.CenterPoint().x + 100;
		double target_y = mRegion.bottom - 50;
		double direct_x = (target_x - x) / 90.;
		double direct_y = (target_y - y) / 90.;

		
		PIDControl(pMessage->cont_x, target_x);
		PIDControl(pMessage->cont_y, target_y);
		pMessage->cont_x.current_val += pMessage->cont_x.control;
		pMessage->cont_y.current_val += pMessage->cont_y.control;
		x = pMessage->cont_x.current_val;
		y = pMessage->cont_y.current_val;
		


		DrawToast(x, y, &BackDC, 1, pMessage->mMessege);
		pMessage->mRemainCount--;
		if (!pMessage->mRemainCount)
		{
			delete 	pMessage;
			mMessageQ.pop();
		}
	}
	SetPlayerTurn(&BackDC, isMyTurn);

	pDC->BitBlt(mRegion.left,mRegion.top,mRegion.Width(),mRegion.Height(),&BackDC,
		mRegion.left,mRegion.top,SRCCOPY); 



	return index;
}


int CSelectBar::AddMessege(CString &StrMessage, int Count)
{
	CMessege_t *pNewMessage = new CMessege_t;

	pNewMessage->cont_x.current_val = mRegion.CenterPoint().x;
	pNewMessage->cont_y.current_val = mRegion.CenterPoint().y;
	pNewMessage->mMessege = StrMessage;
	pNewMessage->mRemainCount = Count;
	mMessageQ.push(pNewMessage);
	return 0;
}
int CSelectBar::SetRegion(CRect rect)
{
		mRegion = rect;
		return 0;
}

int CSelectBar::SetMousePoint(CPoint point)
{
		mMousePoint = point;
		return 0;
}


int CSelectBar::InitSelectBar(CDC *pDC,CRect *crect)
{
	mpDC = pDC;
	MemoryDC.CreateCompatibleDC(pDC);
	BackDC.CreateCompatibleDC(pDC);
	OctopusDC.CreateCompatibleDC(pDC);
	OctopusDC.SelectObject(&Octopus);
	BuffBMP.CreateCompatibleBitmap(pDC, crect->Width(), crect->Height());  
	BackDC.SelectObject(&BuffBMP);

	mBaseFont.CreatePointFont(100,_T("-흥수")); 
	BackDC.SelectObject(&mBaseFont);

	mBigFont.CreatePointFont(300, _T("휴먼매직체"));

	
	mStatus.NumOfGo = 0;		//고를 부른 횟수 1G +1점, 2Go + 2점  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
	mStatus.NumOfBuck = 0;
	mStatus.GoDori = 0;
	mStatus.ChungDan = 0;
	mStatus.HongDan = 0;		//Godory = 5점
	mStatus.ChoDan = 0;
	mStatus.Shake = 0;
	mStatus.Mungtungguri = 0;	// 동물 7개 이상일 경우 ,총점수 x2 

	
	CString greating;
	if (option_opsit)
	{
		greating = _T("즐겜 해요!");
	}
	else
	{
		greating = _T("방가 방가!");
	}
	AddMessege(greating, 100);


	return 0;
}