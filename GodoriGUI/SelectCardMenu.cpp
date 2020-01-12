#include "stdafx.h"
#include "SelectCardMenu.h"
#include <list>
#include <algorithm>
#include "Player.h"
#include "GroundSelect.h"
#include "resource.h"
#include "game.h"
#include "resource.h"       // 주 기호입니다.
#include "math.h"

#define ZOOMMAX 1.5

using namespace std;


 CMenuPresetting SelectMenuPresetting[NUM_OF_SELECT_MENU] = {
	{SELECT_MENU_1xGO				,_T("고 하시겠습니까?")		,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_2xGO				,_T("2고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_3xGO				,_T("3고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_4xGO				,_T("4고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_5xGO				,_T("5고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_6xGO				,_T("6고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_7xGO				,_T("7고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_8xGO				,_T("8고 하시겠습니까?")	,2,SELECT_RESULT_GO		,SELECT_RESULT_STOP},
	{SELECT_MENU_USE_IT_DOUBLE_P	,_T("쌍피로 사용할까요?")	,2,SELECT_RESULT_YEOL	,SELECT_RESULT_DUOUBLE_P},
	{SELECT_MENU_CHOOSE_ONE_OF_TWO	,_T("패를 선택하세요")		,2,SELECT_RESULT_LEFT	,SELECT_RESULT_RIGHT},
	{SELECT_MENU_CHONG_TONG			,_T("총통입니다.")			,2,SELECT_RESULT_CONTINUE,SELECT_RESULT_GAME_STOP},
	{SELECT_MENU_FIRST_TRIPLE_FAULT	,_T("첫뻑입니다.")			,1,SELECT_RESULT_OK		,-1},
	{SELECT_MENU_THIRED_TRIPLE_FAULT,_T("쓰리뻑입니다.")		,1,SELECT_RESULT_OK		,-1},
	{SELECT_MENU_SHAKE				,_T("흔들까요?")			,1,SELECT_RESULT_OK		,-1},
	{SELECT_MENU_WIN				,_T("아싸! 1승")			,1,SELECT_RESULT_OK		,-1},
	{SELECT_MENU_BANKOUT			,_T("상대 파산")			,1,SELECT_RESULT_OK		,-1},
	{SELECT_MENU_LOST				,_T("안타깝습니다")			,1,SELECT_RESULT_OK		,-1},
};


SelectCardMenu::SelectCardMenu()
{
	
	WhitePen[0].CreatePen(PS_SOLID, 4, RGB(255, 220, 220));
	WhitePen[1].CreatePen(PS_SOLID, 3, RGB(255, 210, 210));
	WhitePen[2].CreatePen(PS_SOLID, 2, RGB(255, 200, 200));
	WhitePen[3].CreatePen(PS_SOLID, 1, RGB(255, 190, 190));
	WhitePen[4].CreatePen(PS_SOLID, 2, RGB(255, 180, 180));

	NULLPen.CreateStockObject(NULL_PEN);
	curBitmap.LoadBitmap(IDB_CARDFACE); // 비트맵 객체에 리소스 연결
	mLock = 0;
}


SelectCardMenu::~SelectCardMenu()
{
	curBitmap.DeleteObject();
	BuffBMP.DeleteObject();
	mLock = 0;
}


int SelectCardMenu::SetMenuSetting(int ID) //국진피 쌍피, 고 or 스톱, 승리, 패배
{
	mpPresetting = &SelectMenuPresetting[ID];
	switch (ID)
	{
	case SELECT_MENU_1xGO:
		mStrButton1 = _T("고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_2xGO:
		mStrButton1 = _T("투고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_3xGO:
		mStrButton1 = _T("쓰리고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_4xGO:
		mStrButton1 = _T("4고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_5xGO:
		mStrButton1 = _T("5고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_6xGO:
		mStrButton1 = _T("6고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_7xGO:
		mStrButton1 = _T("7고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_8xGO:
		mStrButton1 = _T("8고");
		mStrButton2 = _T("스톱");
		result1 = SELECT_RESULT_GO;
		result2 = SELECT_RESULT_STOP;
		break;
	case SELECT_MENU_USE_IT_DOUBLE_P:
		mStrButton1 = _T("열끗");
		mStrButton2 = _T("쌍피");
		result1 = SELECT_RESULT_YEOL;
		result2 = SELECT_RESULT_DUOUBLE_P;
		break;
	case SELECT_MENU_WIN:
		mStrButton1 = _T("계속");
		result1 = SELECT_RESULT_OK;
		break;
	case SELECT_MENU_BANKOUT:
		mStrButton1 = _T("계속");
		result1 = SELECT_RESULT_OK;
		break;
	case SELECT_MENU_LOST:
		mStrButton1 = _T("계속");
		result1 = SELECT_RESULT_OK;
		break;	
	default:
		break;
	}
	return 1;
}
int SelectCardMenu::SetMenuSetting(int ID, CCard *pLeft, CCard *pRight) // 패를 선택
{
	mpPresetting = &SelectMenuPresetting[ID];
	if (pLeft == pRight)
		return -1; 

	switch (ID)
	{
	case SELECT_MENU_CHOOSE_ONE_OF_TWO:
		mStrButton1.Format(_T("%s"),pLeft->mCharic.mName);
		mStrButton2.Format(_T("%s"), pRight->mCharic.mName);
		result1 = SELECT_RESULT_LEFT;
		result2 = SELECT_RESULT_RIGHT;
		break;
	default:
		break;
	}
	return 1;
}
int SelectCardMenu::SetMenuSetting(int ID, list<CCard *> &CardList)	//총통, 흔들기, 흔들기, 첫뻑, 쓰리뻑 
{
	mpPresetting = &SelectMenuPresetting[ID];
	switch (ID)
	{
	case	SELECT_MENU_CHONG_TONG:
		mStrButton1 = _T("이번판 승리");
		mStrButton2 = _T("계속 하기");
		result1 = SELECT_RESULT_OK;
		result2 = SELECT_RESULT_CONTINUE;
		break;
	case	SELECT_MENU_FIRST_TRIPLE_FAULT:
		mStrButton1 = _T("계속");
		result1 = SELECT_RESULT_CONTINUE;
		break;
	case	SELECT_MENU_THIRED_TRIPLE_FAULT:
		mStrButton1 = _T("쓰리뻑 승리");
		result1 = SELECT_RESULT_OK;
		break;
	case	SELECT_MENU_SHAKE:
		mStrButton1 = _T("흔들기");
		mStrButton2 = _T("그냥치기");
		result1 = SELECT_RESULT_SHAKE;
		result2 = SELECT_RESULT_CONTINUE;
		break;
	default:
		break;
	}
	return 1;
}


CMenuPresetting::CMenuPresetting(int menu_id, wchar_t *message, int number_of_select, int result1, int result2)
{
	mMENU_ID = menu_id;
	mMessage = message;
	mNumberOfSelect = number_of_select;
	mResult1 = result1;
	mResult2 = result2;
}

int SelectCardMenu::SetRegion(CRect rect)
{
	mRegion = rect;
	return 0;
}

int SelectCardMenu::SetMousePoint(CPoint point)
{
	mMousePoint = point;
	return 0;
}


CPlayer *SelectCardMenu::GetPlayer()
{
	auto & Player1 = CPlayer::GetInstance(ID_PLAYER1);
	CPlayer *pPlayer;
	pPlayer = (CPlayer *)&Player1;
	return pPlayer;
}

extern CCard staticCardList[48];
int SelectCardMenu::DrawFace(CDC *pDC)
{
	CBrush BackBrush;
	int i;
	int x, y;
	int index;
	CBrush *pOldBrush;
	CPen *pOldPen;
	unsigned int distance;
	double factor;
	CFont *pOldFont;

	
	auto & Diller = CDiler::GetInstance(ID_PLAYER2);
	CPlayer *pPlayer = GetPlayer();

	BackDC.SetBkMode(TRANSPARENT);
	BackDC.SetTextColor(RGB(0, 0, 0));
	unsigned int step;

	CString Num;
	
	step = mRegion.Width() / (nItem + 1);
	if (step > 50) step = 50;
	if (step < 2) step = 2;
	CRect temp;

	if (mMousePoint.x == 0)
		index = 0;
	else
		index = (mMousePoint.x - mRegion.left-75) / step;

	if (index > nItem - 1)index = nItem - 1;
	if (index < 0) index = 0;
	pOldFont = BackDC.SelectObject(&mBaseFont);
	

	BackDC.BitBlt(mRegion.left, mRegion.top, mRegion.Width(), mRegion.Height(), pDC,
		mRegion.left, mRegion.top, SRCCOPY);
	pOldPen = BackDC.SelectObject(&NULLPen);
	BackBrush.CreateSolidBrush(RGB((200+mMousePoint.x>>2) % 255, (150+mMousePoint.y >>2)% 255, (180+mMousePoint.y>>2) % 255));
	///GradateBack.CreateSolidBrush(RGB(30,30,30));
	pOldBrush = BackDC.SelectObject(&BackBrush);
	//BackDC.RoundRect(mRegion.left, (mRegion.top + mRegion.bottom * 2) / 3, mRegion.right, mRegion.bottom, 16, 16);
	BackDC.Ellipse(mRegion.left, (mRegion.top + mRegion.bottom * 5) / 6, mRegion.right, mRegion.bottom);
	BackDC.SelectObject(pOldPen);
	
	//BackDC.Rectangle(mRegion.left, mRegion.top, mRegion.right, mRegion.bottom);
	
	BackDC.SelectObject(pOldBrush);
	
	int nFlower = 100;

	for (int i = 0; i < nFlower; i++)
	{
		CPen *pOldPen = BackDC.SelectObject(&WhitePen[i % 5]);
		//	Flower[i].y = mRegion.CenterPoint().y - rand() % 4 + 1;

		BackDC.MoveTo(Flower[i].x, Flower[i].y);

		FlowerVelocity[i].y -= rand() % 5 - 2;
		FlowerVelocity[i].x += rand() % 5 - 2;
		/*
		if (stage % 2) {
			FlowerVelocity[i].x = (Flower[i].x < FlowerDest[i].x) ? FlowerVelocity[i].x + 2 : FlowerVelocity[i].x - 2;
			FlowerVelocity[i].y = (Flower[i].y < FlowerDest[i].y) ? FlowerVelocity[i].y + 2 : FlowerVelocity[i].y - 2;
		}
		*/

		if (FlowerVelocity[i].y > 4) FlowerVelocity[i].y = 4;
		if (FlowerVelocity[i].x > 4) FlowerVelocity[i].x = 4;

		if (FlowerVelocity[i].y < -4) FlowerVelocity[i].y = -4;
		if (FlowerVelocity[i].x < -4) FlowerVelocity[i].x = -4;


		Flower[i].y += FlowerVelocity[i].y;
		Flower[i].x += FlowerVelocity[i].x;


		if (Flower[i].y < mRegion.top)Flower[i].y = mRegion.top+10;
		else if (Flower[i].y > mRegion.bottom)Flower[i].y = mRegion.bottom-10;
		else if (Flower[i].x < mRegion.left)Flower[i].x = mRegion.left+10;
		else if (Flower[i].x > mRegion.right)Flower[i].x = mRegion.right-10;
		else
			BackDC.LineTo(Flower[i].x, Flower[i].y);
		BackDC.SelectObject(pOldPen);
	}

	mStrComment = strMenuTitle;
	DrawPopText1(mRegion.left + 50, mRegion.top + 40,&BackDC, mStrComment);
	
	CCard *pCard;
	 

	if (nItem != 0)
	{
		for (i = 0; i < nItem; i++)
		{
			pCard = &staticCardList[CardArray[i]];
			DrawCard(&BackDC, i, nItem,pCard);
		}
	}

	pDC->BitBlt(mRegion.left, mRegion.top, mRegion.Width(), mRegion.Height(), &BackDC,
		mRegion.left, mRegion.top, SRCCOPY);
	BackDC.SelectObject(pOldFont);

	return index;
}

unsigned int SelectCardMenu::GetCardLocation(int index, long int &x, long int &y)
{
	unsigned int step = (mRegion.Width()-150) / (nItem );
	x = mRegion.left + (step*(index * 2 + 1)) / 2 + 75;
	y = (mRegion.top + 2 * mRegion.bottom) / 3;
	return step;
}

int SelectCardMenu::DrawCard(CDC *pDC, int index,int nCard, CCard *pCard)	
{
	//CPoint refPoint;
	CPoint reactPoint;
	CBrush *pOldBrush;
	CPen *pOldPen;
	CBitmap *oldBitmap;
	unsigned int distance;
	double factor;
	unsigned int step;

	

	CRect temp;
	oldBitmap = MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택


	step = GetCardLocation(index, refPoint[index].x, refPoint[index].y);
	

		//BackDC.SetPixel(Flower[i].x,Flower[i].y,RGB(255,255,255));
		//BackDC.SelectObject(pOldPen);

	//int j;
	//for( i=0; i<1000;i++)
	//{
	//	BackDC.SetPixel(FlowerDest[i].x,FlowerDest[i].y,RGB(255,255,254));
	//}

	CString strTxt;

	factor = 1.;
	distance = (refPoint[index].x - mMousePoint.x)*(refPoint[index].x - mMousePoint.x) +
		(refPoint[index].y - mMousePoint.y)*(refPoint[index].y - mMousePoint.y);

	for (int s = 0; s < 2; s++)	// 그림자 그리기 위해 두번 실행 
	{
			if (sqrt(distance + 20.) < 100)
			{
				if (distance < 2000)
				{
					factor = ZOOMMAX;
				}
				else
				{
					factor = 100. / sqrt(distance + 20.);
				}

				if (factor > ZOOMMAX) factor = ZOOMMAX;
				if (refPoint[index].x > mMousePoint.x)
					reactPoint.x = refPoint[index].x + sqrt((double)distance) / 1000.;
				else
					reactPoint.x = refPoint[index].x - sqrt((double)distance) / 1000.;
				reactPoint.y = refPoint[index].y - 2000. / (sqrt((double)distance) + 30);
			}
			else
			{
				factor = 1.;
				reactPoint.x = refPoint[index].x;
				reactPoint.y = refPoint[index].y - 2000. / sqrt((double)distance + 30);
			}
			if (s == 1) {

				temp.top = reactPoint.y - (88 / 2) * 140/88 * factor;
				temp.bottom = reactPoint.y + (88 / 2) * 140/88 * factor;
				temp.left = reactPoint.x - (88 / 2)*factor;
				temp.right = reactPoint.x + (88 / 2)*factor;

				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
				bf.SourceConstantAlpha = 70;  // opaque (disable constant alpha)
				

				//pDC->RoundRect(temp.left - 2, temp.top - 2, temp.right + 2, temp.bottom + 2, 16, 16);

				//	pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
				//		(pCard->mCardID % 8)*73,(pCard->mCardID / 8)*120,73,120,bf);  	
				pDC->StretchBlt(temp.left + 1, temp.top + 1, temp.Width() , temp.Height() , &MemoryDC,
					((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, SRCCOPY);// , bf);
				
				strTxt.Format(_T("%s"), pCard->mCharic.mName);
				pDC->MoveTo(temp.right, temp.top);
				pDC->LineTo(temp.right, temp.bottom);
				pDC->LineTo(temp.left, temp.bottom);
				DrawPopText(temp.left, temp.bottom+3, pDC, strTxt);
				//pDC->AlphaBlend(temp.left + 1, temp.top + 1, temp.Width() - 2, temp.Height() - 2, &MemoryDC,
				//	((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, bf);

			}		
			else /* 뒷면을 그린다. */
			{
				reactPoint.y = refPoint[index].y + 2000. / sqrt((double)distance + 30);
				temp.left = reactPoint.x - (88 / 2)*factor;
				temp.top = reactPoint.y - (88 / 2) * 140 / 88 * factor;
				temp.right = reactPoint.x + (88 / 2)*factor;
				temp.bottom = reactPoint.y + (88 / 2) * 140 / 88 * factor;

				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
				bf.SourceConstantAlpha = 20;  // opaque (disable constant alpha)

				//pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
				//	(pCard->mCardID % 8)*73,(pCard->mCardID / 8)*120,73,120,bf); 

				//pDC->StretchBlt(temp.left + 1, temp.top + 1, temp.Width() - 2, temp.Height() - 2, &MemoryDC,
				//	((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, SRCCOPY);

				pDC->AlphaBlend(temp.left + 1, temp.top + 1, temp.Width() , temp.Height() , &MemoryDC,
					((pCard->mCardID) % 8) * 97, ((pCard->mCardID) / 8) * 145, 88, 140, bf);

				
			}
	}
	MemoryDC.SelectObject(oldBitmap);
	return 0;
}


void SelectCardMenu::DrawPopText(int x, int y, CDC *pDC, CString Message)
{
	pDC->SetTextColor(RGB(25, 25, 20));
	pDC->TextOut(x, y, Message);
}


void SelectCardMenu::DrawPopText1(int x, int y, CDC *pDC, CString Message)
{

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(x, y - 1, Message);
	pDC->TextOut(x, y + 1, Message);
	pDC->TextOut(x - 1, y, Message);
	pDC->TextOut(x + 1, y, Message);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOut(x, y, Message);
}

int SelectCardMenu::InitSelectCardMenu(CDC *pDC, CRect *crect)
{
	mpDC = pDC;
	MemoryDC.CreateCompatibleDC(pDC);
	BackDC.CreateCompatibleDC(pDC);
	
	BuffBMP.CreateCompatibleBitmap(pDC, crect->Width(), crect->Height());
	BackDC.SelectObject(&BuffBMP);

	mBaseFont.CreatePointFont(250, _T("휴먼매직체"));
	BackDC.SelectObject(&mBaseFont);


	return 0;
}