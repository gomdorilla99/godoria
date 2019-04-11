#include "StdAfx.h"
#include <list>
#include <algorithm>
#include <gdiplus.h>
#include "GroundSelect.h"
#include "resource.h"
#include "game.h"
#include "Card.h"
#include "resource.h"       // 주 기호입니다.
#include "math.h"

#define ZOOMMAX 1.2
using namespace Gdiplus;
using namespace std;




void rotateDDS(CDC *pS_DC, CDC *pD_DC,int sx, int sy, int dx , int dy, int swidth,
		                          int sheight, float theta)
{	
	unsigned char * data;
	unsigned int * ptr;
	float radian;
	radian = theta * 3.1415926535 / 180.;
		
	
	for (int j = 0; j < sheight; j++) 
	{
	
		for (int i = 0; i < swidth; i++) 
		{
			
			unsigned int bRGB, dRGB;
			unsigned char bR, bG, bB, dR, dG, dB, dA;
			float cx, cy, x, y;
			int xp, yp;
			
			cx = swidth * 0.5f;
			cy = sheight * 0.5f;
			x = i - cx;
			y = j - cy;
				
			//역 회전 
			xp = (int)floor(x*cos(radian) - y * sin(radian) + cx + 0.5);
			yp = (int)floor(x*sin(radian) + y * cos(radian) + cy + 0.5);
			
			//보간
			bRGB = pS_DC->GetPixel(sx + xp, sy + yp);
			bR = GetRValue(bRGB);
			bG = GetGValue(bRGB);
			bB = GetBValue(bRGB);
			//dR = GetRValue(dRGB);
			//dG = GetGValue(dRGB);
			//dB = GetBValue(dRGB);
			//dR = (unsigned char)((dA*dR + (255.0 - dA)*bR) / 255.0);
			//dG = (unsigned char)((dA*dG + (255.0 - dA)*bG) / 255.0);
			//dB = (unsigned char)((dA*dB + (255.0 - dA)*bB) / 255.0);
			pD_DC->SetPixel(dx + xp, dy + yp, RGB(bR, bG, bB));
		}
	}
	
}




CGroundSelect::CGroundSelect(void)
{
	int i;
	mActiveStage = 0;
	nItem = 19;
	mMousePoint.x=0;
	mMousePoint.y=0;
	CenterTowerOpen = 0;
	mpDiler = &(CDiler::GetInstance(ID_DILLER));
	if (mpDiler->mOnFloor.GetCount() > 0)
	{
		for_each(mpDiler->mOnFloor.Begin(), mpDiler->mOnFloor.End(), [&](CCard *a) {
			a->mPhysic.mVelocity = CPoint(0, 0);
			a->mPhysic.cont_x.current_val = 0;
			a->mPhysic.cont_y.current_val = 0;
			return a;
		});
	}
	nGo=0;
	nNG=0;
	nPoint=0;

	BackBrush.CreateSolidBrush(RGB(244,194,194));
	BackShadowBrush.CreateSolidBrush(RGB(15,55,18));

	NULLPen.CreateStockObject(NULL_PEN);
	
	CardBack.LoadBitmap(IDB_CARDBACK);
	curBitmap.LoadBitmap(IDB_CARDFACE); // 비트맵 객체에 리소스 연결
	
	BackGroundImage.LoadBitmap(IDB_BACKGROUND);

	RedCard.CreatePatternBrush(&CardBack);
	BackGround.CreatePatternBrush(&BackGroundImage);
	

}

CGroundSelect::~CGroundSelect(void)
{
	curBitmap.DeleteObject();
}

CPoint CGroundSelect::Gravity(CPoint p1, CPoint p2,int m1, int m2)
{
	CPoint temp;
	double rsq = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)+40;
	double r=sqrt((double)rsq);
	if(r>0){
		temp.x= GForce*(m1*m2)*(p1.x -p2.x)/r/rsq; 
		temp.y= GForce*(m1*m2)*(p1.y -p2.y)/r/rsq;
	}else
	{
		temp.x=temp.y=0;
	}
	return temp;
}

int CGroundSelect::GravityX(CPoint p1, int X,int m1, int m2)
{
	int temp;
	double rsq = (p1.x - X)*(p1.x - X)+40;
	double r=sqrt((double)rsq);
	temp =GForce*(m1*m2)*(p1.x - X )/rsq; 
	if(p1.x>mRegion.left && p1.x<mRegion.right)
	{
		temp;
	}
	else
	{
		temp = 0;//-temp;//
	}
	return temp;  
}

int CGroundSelect::GravityY(CPoint p1, int Y,int m1, int m2)
{
	int temp;
	double rsq = (p1.y - Y)*(p1.y - Y)+40;
	double r=sqrt((double)rsq);
	temp = GForce*(m1*m2)*( p1.y-Y)/rsq; 
	if(p1.y>mRegion.top && p1.y<mRegion.bottom)
	{
		temp;//0;
	}
	else
	{
		temp = 0;//-temp;//
	}
	return temp; 
}




double PIDControl(control_t& Cont, double target)
{
	double err;
	double D_err;					// 오차적분. 오차미분 
	double Kp_term, Ki_term, Kd_term;		// p항, i항, d항 

	err = target - Cont.current_val;		// 오차 = 목표치-현재값 
	Kp_term = Kp * err;						// p항 = Kp*오차 

	Cont.I_err += err * dt;						// 오차적분 += 오차*dt 
	Ki_term = Ki * Cont.I_err;					// i항 = Ki*오차적분 

	D_err = (err - Cont.prev_err) / dt;		// 오차미분 = (현재오차-이전오차)/dt 
	Kd_term = Kd * D_err;					// d항 = Kd*오차미분 

											/*	if (Kd_term > 3)
											Kd_term = 3;
											if (Kd_term < -3)
											Kd_term = -3;*/

	Cont.prev_err = err;					// 현재오차를 이전오차로 

	Cont.control = Kp_term + Ki_term + Kd_term;  // 제어량 = p항+i항+d항 

												 /*
												 if (Cont.control > 10)
												 Cont.control = 10;
												 if (Cont.control < -10)
												 Cont.control = -10;
												 */
	return Cont.control;
}

void CGroundSelect::FligtTo(int index,long int& x, long int& y, int xx, int yy)
{
	double direct_x = (xx - x) / 90.;
	double direct_y = (yy - y) / 90.;
	double target_x = xx;
	double target_y = yy;
	CCard *pCard = mpDiler->GetFloor()->GetItem(index);
	if (pCard)
	{
		PIDControl(pCard->mPhysic.cont_x, target_x);
		PIDControl(pCard->mPhysic.cont_y, target_y);
		pCard->mPhysic.cont_x.current_val += pCard->mPhysic.cont_x.control;
		pCard->mPhysic.cont_y.current_val += pCard->mPhysic.cont_y.control;
		x = pCard->mPhysic.cont_x.current_val;
		y = pCard->mPhysic.cont_y.current_val;
	}
}


///
///   1     4     7    10
///
///      2     5     8     11
///
///    3    6     9   12
int CGroundSelect::DrawCard(CDC *pDC, int index)
{
	CBitmap *oldBitmap;
	CPoint *refPoint;
	CPoint Target;
	CPoint *velocity;
	
	unsigned int distance;
	double factor;
	CPoint reactPoint;
	CCard *pCard;
	int nCard = 0;
	int month;
	int nCount = mpDiler->GetFloor()->GetCount();
	if (nCount > index)
	{
	
		pCard = mpDiler->GetFloor()->GetItem(index);
		if (!pCard)
			return 0;
		nCard = pCard->mCardID;
		month = pCard->mCharic.mTheMonth;
		refPoint = &pCard->mPhysic.mPosition;
	}
	else
	{
		return 0;
	}

	unsigned int step = mRegion.Width() / 10;
	if (step > 88) step = 88;
	CRect temp;
	oldBitmap = MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택

	int i;


	Target.x = 100 + (month / 3) * mRegion.Width() / 6 + (nCard % 4) * 10;
	Target.y = mRegion.top + 80 + (month % 3) * (mRegion.Height()-50) / 3 + (nCard % 4) * 10;
	int count;
	if (mActiveStage == ID_PLAYER1)
	{
		auto & Player = CPlayer::GetInstance(ID_PLAYER1);
		count = Player.mOnFloor.GetCount();
		for_each(Player.mOnFloor.Begin(), Player.mOnFloor.End(), [&](CCard * a) {
			if (pCard == a)
			{
				Target.x = 100 + (month / 3) * mRegion.Width() / 6 + (nCard % 4) * 10;
				Target.y = mRegion.bottom+100;
			}
		});
	}
	else if (mActiveStage == ID_PLAYER2)
	{
		auto & Player = CPlayer::GetInstance(ID_PLAYER2);
		count = Player.mOnFloor.GetCount();
		for_each(Player.mOnFloor.Begin(), Player.mOnFloor.End(), [&](CCard * a) {
			if (pCard == a)
			{
				Target.x = 100 + (month / 3) * mRegion.Width() / 6 + (nCard % 4) * 10;
				Target.y = mRegion.top-100;
			}
		});
	}

	if(CenterTowerOpen==0)
	{
		FligtTo(index, refPoint->x, refPoint->y, Target.x, Target.y);
	}
//	else
//	{
//		refPoint->x = (refPoint->x * 4 + Target.x) / 5;
//		refPoint->y = (refPoint->y * 4 + Target.y) / 5;
//	}

	if (CenterTowerOpen == 0 )
	{
		factor = 1.;
		distance = (refPoint->x - mMousePoint.x)*(refPoint->x - mMousePoint.x) +
			(refPoint->y - mMousePoint.y)*(refPoint->y - mMousePoint.y);
		if (sqrt(distance + 4000.) < 100)
		{
			factor = 100 / sqrt(distance + 4000.);
		}
		else
		{
			factor = 0.9;
		}

		reactPoint = *refPoint;
		temp.left = reactPoint.x - (step / 2)*factor;
		temp.top = reactPoint.y - (step / 2) * 140 / 88 * factor;
		temp.right = reactPoint.x + (step / 2)*factor;
		temp.bottom = reactPoint.y + (step / 2) * 140 / 88 * factor;
	}
	else
	{
		if (index == nItem-1)
		{
			distance = CenterTowerOpen * CenterTowerOpen * 100;
			if (sqrt(distance + 4000.) < 100)
			{
				factor = 100 / sqrt(distance + 4000.);
			}
			else
			{
				factor = 0.9;
			}

			reactPoint = *refPoint;
			temp.left = reactPoint.x - (step / 2)*factor* (5-CenterTowerOpen)*2/5;
			temp.top = reactPoint.y - (step / 2) * 140 / 88 * factor;
			temp.right = reactPoint.x + (step / 2)*factor;
			temp.bottom = reactPoint.y + (step / 2) * 140 / 88 * factor;
		}
		else
		{
			distance = 10000;
			if (sqrt(distance + 4000.) < 100)
			{
				factor = 100 / sqrt(distance + 4000.);
			}
			else
			{
				factor = 0.9;
			}
			reactPoint = *refPoint;
			temp.left = reactPoint.x - (step / 2)*factor;
			temp.top = reactPoint.y - (step / 2) * 140 / 88 * factor;
			temp.right = reactPoint.x + (step / 2)*factor;
			temp.bottom = reactPoint.y + (step / 2) * 140 / 88 * factor;
		}
		
	}
	


	
	BLENDFUNCTION bf;      // structure for alpha blending
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_OVER;  // use source alpha 
	bf.SourceConstantAlpha = 220;  // opaque (disable constant alpha)

	//pDC->Rectangle(temp.left+ factor*30, temp.top + factor * 30, temp.right + factor * 30, temp.bottom + factor * 30);

	pDC->AlphaBlend(temp.left, temp.top, temp.Width(), temp.Height(), &MemoryDC,
		((nCard) % 8) * 97, ((nCard) / 8) * 145, 88, 140, bf);

	pDC->MoveTo(temp.right, temp.top);
	pDC->LineTo(temp.right, temp.bottom);
	pDC->LineTo(temp.left, temp.bottom);
	MemoryDC.SelectObject(oldBitmap);
	return 0;
}

void CGroundSelect::DrawPopText(int x,int y,CDC *pDC, int oposit,CString Message)	
{
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOut(x,y-1,Message);
	pDC->TextOut(x,y+1,Message);
	pDC->TextOut(x-1,y,Message);
	pDC->TextOut(x+1,y,Message);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(x,y,Message);
}

void CGroundSelect::DrawToast(int x, int y, CDC *pDC, int oposit, CString Message)
{
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&mBigFont);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(x-1, y, Message);
	pDC->TextOutW(x, y-1, Message);
	pDC->TextOutW(x, y+1, Message);
	pDC->TextOutW(x+1, y, Message);
	pDC->SetTextColor(RGB(255, 255, 190));
	pDC->TextOutW(x , y, Message);

	pDC->SelectObject(pOldFont);
}
void CGroundSelect::DrawCeterTower(CDC *pDC)
{
	int i;
	int RemainCard=nItem;
	CRect ViewRect = mRegion;
	CRect rctCenterCard;
	
	
	CBrush *pOldBrush;
	CPen *pOldPen;
	for(int s=1;s<2;s++)
	{
		for(i=0;i<RemainCard;i+=2)
		{
			if(s==1){
				
				pOldBrush  = pDC->SelectObject(&RedCard);
				//rctCenterCard.left = ViewRect.CenterPoint().x-30-i/5;
				rctCenterCard.left = 20- 30 - i / 5;
				rctCenterCard.top = ViewRect.CenterPoint().y-40-i/5;
				rctCenterCard.right = 20 +30-i/5;

				//rctCenterCard.right = ViewRect.CenterPoint().x+30-i/5;
				rctCenterCard.bottom = ViewRect.CenterPoint().y+40-i/5;
				pOldPen = nullptr;
			}else
			{
				pOldPen = pDC->SelectObject(&NULLPen);
				pOldBrush  = pDC->SelectObject(&BackShadowBrush);
				//rctCenterCard.left = ViewRect.CenterPoint().x-30+i;
				rctCenterCard.left = 20 - 30 + i;
				rctCenterCard.top = ViewRect.CenterPoint().y-40+i;
				rctCenterCard.right = 20+30+i;

				//rctCenterCard.right = ViewRect.CenterPoint().x+30+i;
				rctCenterCard.bottom = ViewRect.CenterPoint().y+40+i;
			}
			pDC->RoundRect(rctCenterCard.left,rctCenterCard.top,rctCenterCard.right,rctCenterCard.bottom,5,5);
			pDC->SelectObject(pOldBrush);
			if(s==0) pDC->SelectObject(pOldPen);
		}
	}
	if (CenterTowerOpen > 0)
		CenterTowerOpen--;
	RemainCard--;



}

int CGroundSelect::SetLatestCardPosition(CPoint newPoint)
{

	int nItem = mpDiler->GetFloor()->GetCount();
	if (nItem)
	{
		mpDiler->GetFloor()->Back()->mPhysic.mPosition = newPoint;
		mpDiler->GetFloor()->Back()->mPhysic.cont_x.current_val = newPoint.x;
		mpDiler->GetFloor()->Back()->mPhysic.cont_y.current_val = newPoint.y;
		mpDiler->GetFloor()->Back()->mPhysic.mVelocity = CPoint(0, 0);
	}
	
	return 0;
}

extern CCard staticCardList[54];
int CGroundSelect::AddMessege(CString &StrMessage, int Count, int CardNumber)
{
	CMessege_t *pNewMessage = new CMessege_t;

	pNewMessage->cont_x.current_val = staticCardList[CardNumber].mPhysic.mPosition.x;
	pNewMessage->cont_y.current_val = staticCardList[CardNumber].mPhysic.mPosition.y;
	pNewMessage->mMessege = StrMessage;
	pNewMessage->mRemainCount = Count;
	mMessageQ.push(pNewMessage);
	return 0;
}



int CGroundSelect::DrawFace(CDC *pDC)
{
	int i;
	int x,y;
	int index;
	CPoint refPoint;
	CPoint reactPoint;
	CBrush *pOldBrush;
	unsigned int distance;
	double factor;

	auto& diller = CDiler::GetInstance(ID_DILLER);
	
	nItem = mpDiler->GetFloor()->GetCount();
		
	///////
	BackDC.SetBkMode(TRANSPARENT);
	BackDC.SetTextColor(RGB(0,0,0));
	unsigned int step;

	CString Num;
	CPen *pOldPen;
	
	
	step = mRegion.Width() / 12;
	if (step > 50) step= 50;

	CRect temp;

	if(mMousePoint.x ==0)
		index = 0;
	else
		index = (mMousePoint.x - mRegion.left) / step;
	
	if (index > nItem-1)index = nItem-1;
	if (index < 0) index = 0;

	
	//pOldBrush = BackDC.SelectObject(&BackGround);
	pOldBrush = BackDC.SelectObject(&BackBrush);
	pOldPen = BackDC.SelectObject(&NULLPen);
	BackDC.Rectangle(mRegion.left,mRegion.top,mRegion.right+1,mRegion.bottom+1);
	BackDC.SelectObject(pOldBrush);
	BackDC.SelectObject(pOldPen);
	BackDC.SetStretchBltMode(HALFTONE);
	BackDC.StretchBlt(mRegion.right - 314, mRegion.top, 314, 246- mRegion.top, &OctopusDC,
		0, mRegion.top,314 , 246- mRegion.top, SRCCOPY);
	
	DrawCeterTower(&BackDC);
	GForce--;
	if (GForce <10) GForce =10;
	if (nItem !=0)
	
	for(i=0;i<nItem;i++)
	{
		DrawCard(&BackDC,i);
	}

	CMessege_t *pMessage;
	if (mMessageQ.size() > 0)
	{
		pMessage = mMessageQ.front();
		int x, y;
		x = pMessage->cont_x.current_val;
		y = pMessage->cont_y.current_val;
		double target_x = pMessage->cont_x.current_val;
		double target_y = pMessage->cont_y.current_val;
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

	//DrawToast(mRegion.CenterPoint().x, mRegion.bottom - 50,&BackDC, 1, CString(_T("아싸라삐야!")));
	
	//pDC->AlphaBlend(mRegion.left-30,mRegion.top,mRegion.Width(),mRegion.Height(),&BackDC,
	//	mRegion.left-30,mRegion.top,SRCCOPY);
	//nGo,nNG,nPoint


	
	return 0;
}

int CGroundSelect::DCUpdate(CDC *pDC)
{
	pDC->BitBlt(mRegion.left, mRegion.top, mRegion.Width(), mRegion.Height(), &BackDC, mRegion.left, mRegion.top, SRCCOPY);
	return 0;
}

extern CCard staticCardList[54];
int CGroundSelect::SetRegion(CRect rect)
{
		mRegion = rect;
		unsigned int step = mRegion.Width() / 12;

		CenterPoint = mRegion.CenterPoint();
		if (step > 50) step= 50;
		int index;
		GForce =100;
		for(index=0;index<MAX_CARD;index++)
		{
			CRect temp;
			
			staticCardList[index].mPhysic.mPosition = 
				CPoint(mRegion.left+(step*(index*2+1))/2+(rand() % mRegion.Width()-step)-mRegion.Width()/2 , 
					(mRegion.top + 2 * mRegion.bottom) / 3 + (rand() % mRegion.Height() - step) - mRegion.Height() / 2);
			staticCardList[index].mPhysic.mVelocity = CPoint(4,4);
		}
		return 0;
}

int CGroundSelect::SetMousePoint(CPoint point)
{
		mMousePoint = point;
		return 0;
}
#if 0
int CGroundSelect::SetRect(CRect *crect)
{
	
	return 0;
}
#endif 
void CGroundSelect::SetTurn(int turn)
{
	mTurn = turn;

}
int CGroundSelect::InitSelectBar(CDC *pDC,CRect *crect)
{
	mpDC = pDC;
	MemoryDC.CreateCompatibleDC(pDC);
	BackDC.CreateCompatibleDC(pDC);
	
	BuffBMP.CreateCompatibleBitmap(pDC, crect->Width(), crect->Height());  
	BackDC.SelectObject(&BuffBMP);

	Octopus.LoadBitmapW(IDB_OCTOPUS);
	OctopusDC.CreateCompatibleDC(pDC);
	OctopusDC.SelectObject(&Octopus);


	mBaseFont.CreatePointFont(120,_T("-흥수")); 
	mBigFont.CreatePointFont(200, _T("휴먼매직체"));

	return 0;
}