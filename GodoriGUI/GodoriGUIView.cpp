
// GodoriGUIView.cpp: CGodoriGUIView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GodoriGUI.h"
#endif

#include <queue>
#include "GodoriGUIDoc.h"
#include "GodoriGUIView.h"
#include "SelectCardMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
CGodoriGUIView *G_Gostop;

queue <CAnimationItem *> AnimationQueue;

// CGodoriGUIView

IMPLEMENT_DYNCREATE(CGodoriGUIView, CView)

BEGIN_MESSAGE_MAP(CGodoriGUIView, CView)
	// 표준 인쇄 명령입니다.
	//ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(GUI_MESSAGE,OnGUIHandler)
END_MESSAGE_MAP()


extern HANDLE ghSemaphore_Wait_UI_Select;

extern queue<GUIMessage_t *> gGUIQueue;
extern queue<GUI_SELECT_t *> gSelctQueue;


// CGodoriGUIView 생성/소멸

CGodoriGUIView::CGodoriGUIView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	select_done = false;
	BackBrush.CreateSolidBrush(RGB(15,60,20));
	RedCard.CreatePatternBrush(&CardBack);
	YelloPen.CreatePen(PS_SOLID, 5, RGB(255, 200, 40));
	NULLPen.CreateStockObject(NULL_PEN);
	BackShadowBrush.CreateSolidBrush(RGB(15,50,18));

	mSelectBar.SetPlayerID(ID_PLAYER1);
	mOpSelectBar.SetPlayerID(ID_PLAYER2);
}



CGodoriGUIView::~CGodoriGUIView()
{
}

/*
BOOL CGodoriGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}
*/
// CGodoriGUIView 그리기

void CGodoriGUIView::OnDraw(CDC* /*pDC*/)
{
	CGodoriGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}



BOOL CGodoriGUIView::PreCreateWindow(CREATESTRUCT & cs)
{
	return CView::PreCreateWindow(cs);
}


// CGodoriGUIView 인쇄

BOOL CGodoriGUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGodoriGUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGodoriGUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGodoriGUIView 진단

#ifdef _DEBUG
void CGodoriGUIView::AssertValid() const
{
	CView::AssertValid();
}

void CGodoriGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGodoriGUIDoc* CGodoriGUIView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGodoriGUIDoc)));
	return (CGodoriGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CGodoriGUIView 메시지 처리기

CAnimationItem::CAnimationItem(CPoint _StartPoint, CPoint _EndPoint, int delay, int nCount)
{
	iStep = 0;
	nStep = nCount;
	DelayCnt = delay;
	StartPoint = _StartPoint;
	EndPoint = _EndPoint;
	next = NULL;
}

int CAnimationItem::DoAnimation(CDC *pDC)
{
	CAnimationItem *pItemTemp;
	pItemTemp = AnimationQueue.front();
	switch (pItemTemp->MotionType)
	{
	case ANIMATION_MOVE:
		break;
	case ANIMATION_CENTER_CARD_OPEN:
		break;
	case ANIMATION_PLAYER1_WIN:
		break;
	}
	if (DelayCnt > 0) {
		DelayCnt--;
		return DelayCnt * -1;
	}
	//CBrush *pOldBrush;
	//pOldBrush  = pDC->SelectObject(&RedCard);
	if (nStep == 0) return 0;

	CurPoint.x = ((nStep - iStep) * StartPoint.x + iStep * EndPoint.x) / nStep;
	CurPoint.y = ((nStep - iStep) * StartPoint.y + iStep * EndPoint.y) / nStep;
	CRect rctCard;
	rctCard.left = CurPoint.x - 30;
	rctCard.top = CurPoint.y - 40;
	rctCard.right = CurPoint.x + 30;
	rctCard.bottom = CurPoint.y + 40;
	pDC->RoundRect(rctCard.left, rctCard.top, rctCard.right, rctCard.bottom, 5, 5);

	//pDC->SelectObject(pOldBrush);

	iStep++;
	return iStep;
}


int CGodoriGUIView::DoAnimation(CDC *pDC)
{

	CAnimationItem *pItemTemp;

	while (AnimationQueue.size())
	{
		pItemTemp = AnimationQueue.front();
		pItemTemp->DoAnimation(pDC);
	
		AnimationQueue.pop();
		delete pItemTemp;
	}
	return 0;
}





void CGodoriGUIView::OnPaint()
{
	static int init = 0;
	CRect ViewRect;
	CRect TempRect;
	CBrush  *pOldBrush;

	CPaintDC dc(this); // device context for painting
	this->GetClientRect(&ViewRect);
	rctCenterCard = ViewRect;
	
	
	if (init == 0)
	{
		mStroyBoard.SetRegion(ViewRect);
		//dc.Rectangle(ViewRect);
		mSelectBar.InitSelectBar(&dc, &ViewRect);
		mOpSelectBar.InitSelectBar(&dc, &ViewRect);
		mGroundSelect.InitSelectBar(&dc, &ViewRect);
		mStroyBoard.InitSelectBar(&dc, &ViewRect);
		mSelectMenu.InitSelectCardMenu(&dc, &ViewRect);
		mOpSelectBar.SetOpsit();
		dc.SetBkMode(TRANSPARENT);
		init = 1;

	}
	SetTimer(1, 33, NULL);

	pOldBrush = dc.SelectObject(&BackBrush);
	dc.Rectangle(0, 0, ViewRect.Width(), ViewRect.Height());
	dc.SelectObject(pOldBrush);

	/*
		dc.MoveTo(ViewRect.left,ViewRect.CenterPoint().y);
		dc.LineTo(ViewRect.right,ViewRect.CenterPoint().y);

		dc.MoveTo(ViewRect.CenterPoint().x,ViewRect.top);
		dc.LineTo(ViewRect.CenterPoint().x,ViewRect.bottom);

		dc.MoveTo(0,0);
		dc.LineTo(100,100);
	*/
	TempRect.left = ViewRect.left;
	TempRect.right = ViewRect.right;
	TempRect.top = ViewRect.bottom * 7 / 10;
	TempRect.bottom = ViewRect.bottom;

	mSelectBar.SetRegion(TempRect);

	TempRect.top = 0;
	TempRect.bottom = ViewRect.bottom / 6;
	mOpSelectBar.SetRegion(TempRect);

	TempRect.top = ViewRect.bottom / 6;
	TempRect.bottom = ViewRect.bottom * 7 / 10;
	mOpSelectBar.SetMousePoint(ViewRect.CenterPoint());
	mGroundSelect.SetRegion(TempRect);
	
	TempRect.left = ViewRect.left+100;
	TempRect.right = ViewRect.right-100;
	TempRect.top = ViewRect.bottom / 5+100;
	TempRect.bottom = ViewRect.bottom * 8 / 10-100;
	mSelectMenu.SetRegion(TempRect);

	if (mStroyBoard.mLock)
	{
		mStroyBoard.DrawFace(&dc);
	}
	else
	{
		if (mSelectMenu.mLock)
		{
			mGroundSelect.DrawFace(&dc);
			mSelectMenu.DrawFace(&dc);
		}
		else
		{
			mGroundSelect.DrawFace(&dc);
			mSelectBar.DrawFace(&dc);
			mOpSelectBar.DrawFace(&dc);
		}
	}

	//DrawPopup(&dc,1,_T("텍스트 출력 테스트"));
}
void CGodoriGUIView::GameStartSeq(void)
{
	//Clean
	for (int j = 0; j < 10; j++)
	{
		CAnimationItem *pAni = new CAnimationItem(mGroundSelect.CenterPoint, mSelectBar.refPoint[j], j, 10 + j);
		AnimationQueue.push(pAni);

	}
	for (int j = 0; j < 10; j++)
	{
		CAnimationItem *pAni = new CAnimationItem(mGroundSelect.CenterPoint, mOpSelectBar.refPoint[10 - j], 20 + j, 10 + j);
		AnimationQueue.push(pAni);
	}
}


void CGodoriGUIView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	CDC *pDC = this->GetWindowDC();

	mSelectBar.SetMousePoint(point);

	
	if (mStroyBoard.mLock) {
		mStroyBoard.SetMousePoint(point);
		mStroyBoard.DrawFace(pDC);

	}
	else
	{
		if (mSelectMenu.mLock)
		{
		//	mGroundSelect.DrawFace(pDC);
			mSelectMenu.SetMousePoint(point);
			mSelectMenu.DrawFace(&mGroundSelect.BackDC);
			mGroundSelect.DCUpdate(pDC);
		}
		else
		{
			mGroundSelect.SetMousePoint(point);
		//	mGroundSelect.DrawFace(pDC);
		///	mSelectBar.DrawFace(pDC);
		//	mOpSelectBar.DrawFace(pDC);
		//	mGroundSelect.DCUpdate(pDC);
		}
	}
	ReleaseDC(pDC);
}


void CGodoriGUIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnTimer(nIDEvent);
	CDC *pDC = this->GetWindowDC();

	if (mStroyBoard.mLock) {
		mStroyBoard.DrawFace(pDC);
	}
	else
	{
		if (mSelectMenu.mLock)
		{
			mGroundSelect.DrawFace(pDC);
			mSelectMenu.DrawFace(&mGroundSelect.BackDC);
			mGroundSelect.DCUpdate(pDC);
		}
		else
		{
			mGroundSelect.DrawFace(pDC);
			mSelectBar.DrawFace(pDC);
			mOpSelectBar.DrawFace(pDC);
			mGroundSelect.DCUpdate(pDC);
		}
	}
	ReleaseDC(pDC);

	ChekGUIMessage();

	SetTimer(1, 10, NULL);
}


void CGodoriGUIView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 
	CDC *pDC = this->GetWindowDC();


	mGroundSelect.GForce = 30;
	if (mGroundSelect.nItem < 0) {
		mGroundSelect.nItem = 40;
	}

	if (!mStroyBoard.mLock)
	{
		if (mSelectMenu.mLock)
		{
			mSelectMenu.SetMousePoint(point);
			mIndex = mSelectMenu.DrawFace(pDC);
			select_done = true;

			GUI_SELECT_t *pNewSelect = new GUI_SELECT_t;
			pNewSelect->ID = ID_PLAYER1;
			pNewSelect->SELECT_INDEX = mIndex;
			gSelctQueue.push(pNewSelect);


			if (!ReleaseSemaphore(
				ghSemaphore_Wait_UI_Select, // handle to semaphore
				1,      // increase count by one
				NULL))    // not interested in previous count
			{
				TRACE("ReleaseSemaphore error: %d\n", GetLastError());
			}
			mSelectMenu.DrawFace(pDC);
		}
		else
		{

			mSelectBar.SetMousePoint(point);
			mIndex = mSelectBar.DrawFace(pDC);
			select_done = true;

			GUI_SELECT_t *pNewSelect = new GUI_SELECT_t;
			pNewSelect->ID = ID_PLAYER1;
			pNewSelect->SELECT_INDEX = mIndex;
			gSelctQueue.push(pNewSelect);


			if (!ReleaseSemaphore(
				ghSemaphore_Wait_UI_Select, // handle to semaphore
				1,      // increase count by one
				NULL))    // not interested in previous count
			{
				TRACE("ReleaseSemaphore error: %d\n", GetLastError());
			}
			mOpSelectBar.DrawFace(pDC);
		}
	}
	else
	{

		mIndex = mStroyBoard.DrawFace(pDC);
		select_done = true;

		GUI_SELECT_t *pNewSelect = new GUI_SELECT_t;
		pNewSelect->ID = ID_PLAYER1;
		pNewSelect->SELECT_INDEX = mIndex;
		gSelctQueue.push(pNewSelect);

		if (!ReleaseSemaphore(
			ghSemaphore_Wait_UI_Select, // handle to semaphore
			1,      // increase count by one
			NULL))    // not interested in previous count
		{
			TRACE("ReleaseSemaphore error: %d\n", GetLastError());
		}

	}
	CView::OnLButtonUp(nFlags, point);
}

void CGodoriGUIView::DrawPopup(CDC *pDC, int oposit, CString Message)
{
	CRect POPRect(100, 100, 300, 200);

	pDC->RoundRect(POPRect.left, POPRect.top, POPRect.right, POPRect.bottom, 30, 30);

	pDC->SetTextColor(RGB(0, 0, 0));

	pDC->TextOut(POPRect.left + 10, POPRect.CenterPoint().y - 21, Message);
	pDC->TextOut(POPRect.left + 10, POPRect.CenterPoint().y - 19, Message);
	pDC->TextOut(POPRect.left + 10 - 1, POPRect.CenterPoint().y - 20, Message);
	pDC->TextOut(POPRect.left + 10 + 1, POPRect.CenterPoint().y - 20, Message);

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOut(POPRect.left + 10, POPRect.CenterPoint().y - 20, Message);
}


int  CGodoriGUIView::ChekGUIMessage()
{
	GUIMessage_t *pMessage;
	while (!gGUIQueue.empty())
	{
		pMessage = gGUIQueue.front();
		OnGUIHandler(pMessage->GUI_COMMAND, pMessage->Turn);
		gGUIQueue.pop();
		delete pMessage;
	}
	

	return 0;
}


extern CMenuPresetting SelectMenuPresetting[NUM_OF_SELECT_MENU];
LRESULT  CGodoriGUIView::OnGUIHandler(WPARAM wParam, LPARAM lParam ){
	CDC *pDC;
	CPoint newPoint;
	GUIDraw_t GUI= (GUIDraw_t)wParam;
	CString StrMessege;
	int player = (int)lParam;
	mGroundSelect.mActiveStage = 0;
	switch(GUI)
	{

		case PLAYER_CHANCE:	//누구의 차례인가	
			mTurn = player;
			pDC = this->GetWindowDC();
			if (mTurn == ID_PLAYER1)
			{
				
				mSelectBar.SetPlayerTurn(pDC,true);
				mOpSelectBar.SetPlayerTurn(pDC,false);
			}
			else
			{
				mSelectBar.SetPlayerTurn(pDC,false);
				mOpSelectBar.SetPlayerTurn(pDC,true);
			}
			ReleaseDC(pDC);
			break; 
		case PAYBACK_A_CARD:
		{
			if (this) { pDC = this->GetWindowDC(); }
			else { return -1; }
			//pMessage->Param1;	//Card ID
			//pMessage->Param2;	//Start X
			//pMessage->Param3;	//Start Y
			//pMessage->Param4; //End   X
			//pMessage->Param5; //End   Y
			//pMessage->Param6; //Location
			GUIMessage_t *pMessage;

			pMessage = gGUIQueue.front();
			if (pMessage->Turn == ID_PLAYER1)
			{
				mSelectBar.GetCardLocation(pMessage->Param6, newPoint.x, newPoint.y);
			}
			else
			{
				mOpSelectBar.GetCardLocation(pMessage->Param6, newPoint.x, newPoint.y);
			}
			mGroundSelect.SetLatestCardPosition(newPoint);
			mGroundSelect.DrawFace(pDC);
			
			if (mTurn == ID_PLAYER1)
			{
				mSelectBar.SetPlayerTurn(pDC, true);
				mOpSelectBar.SetPlayerTurn(pDC, false);
			}
			else
			{
				mSelectBar.SetPlayerTurn(pDC, false);
				mOpSelectBar.SetPlayerTurn(pDC, true);
			}
			ReleaseDC(pDC);
			//TRACE("G:%d,T:%d\r\n", gCardList.GroundCardList->GetCardNumber(), gCardList.GroundTempList->GetCardNumber());
			//gCardList.GroundCardList->PrintList();
			break;
		}
		case OPEN_CENTER_CARD:
		{
			//Center Card Animation 
			//중앙 타워에서 카드를 보여준다. 
			newPoint.x = 15;
			newPoint.y = mGroundSelect.mRegion.CenterPoint().y;
			mGroundSelect.SetLatestCardPosition(newPoint);
			mGroundSelect.CenterTowerOpen = 5;
			//if (this) { pDC = this->GetWindowDC(); }
			//else { return -1; }
			//mGroundSelect.DrawFace(pDC);
			//mGroundSelect.DCUpdate(pDC);
			//ReleaseDC(pDC);
			//Sleep(100);
			break;
		}		
		
		case GIVEMACHED_CARD:
		{

			// OnFloor에 있는 것을 Ground로 보내줘야 한다. 
			// Move Animation 
			if (this) { pDC = this->GetWindowDC(); }
			else { return -1; }
			GUIMessage_t *pMessage;
			int count;
			pMessage = gGUIQueue.front();
			if (pMessage->Turn == ID_PLAYER1)
			{
				CPen *pOldPen;
				mGroundSelect.mActiveStage = ID_PLAYER1;
				pOldPen= pDC->SelectObject(&YelloPen);
				auto & Player = CPlayer::GetInstance(ID_PLAYER1);
				count = Player.mOnFloor.GetCount();
				for_each(Player.mOnFloor.Begin(), Player.mOnFloor.End(), [&](CCard * a) {
					pDC->MoveTo(a->mPhysic.mPosition);
//					pDC->LineTo(mGroundSelect.mRegion.Width()-180, mSelectBar.mRegion.top);
					pDC->LineTo(a->mPhysic.mPosition.x, mSelectBar.mRegion.top);

				});
				pDC->SelectObject(pOldPen);
				//Sleep(600);
				//mGroundSelect.mActiveStage = 0;
			}
			else
			{
				CPen *pOldPen;
				mGroundSelect.mActiveStage = ID_PLAYER2;
				auto & Player = CPlayer::GetInstance(ID_PLAYER2);
				pOldPen = pDC->SelectObject(&YelloPen);
				count = Player.mOnFloor.GetCount();
				count = Player.mOnFloor.GetCount();
				for_each(Player.mOnFloor.Begin(), Player.mOnFloor.End(), [&](CCard * a) {
					pDC->MoveTo(a->mPhysic.mPosition);
					pDC->LineTo(mGroundSelect.mRegion.Width() - 180, mOpSelectBar.mRegion.bottom);
				});
				pDC->SelectObject(pOldPen);
				//Sleep(600);
				//mGroundSelect.mActiveStage = 0;
			}
			ReleaseDC(pDC);
			//CPoint newPoint(pMessage->Param2, pMessage->Param3);
			//mGroundSelect.SetLatestCardPosition(newPoint);
	//		if (this) { pDC = this->GetWindowDC(); }
	//		else { return -1; }
	//		mGroundSelect.DrawFace(pDC);
	//		ReleaseDC(pDC);
		}
		case DRAW_GROUND:
		case DRAW_SELECT_BAR:
			if (this) { pDC = this->GetWindowDC(); }
			else { return -1; }
			if (mTurn == ID_PLAYER1)
			{
				mSelectBar.SetPlayerTurn(pDC, true);
				mOpSelectBar.SetPlayerTurn(pDC, false);
			}
			else
			{
				mSelectBar.SetPlayerTurn(pDC, false);
				mOpSelectBar.SetPlayerTurn(pDC, true);
			}
			ReleaseDC(pDC);
			break; 
		case GIVE_CARD:
		{
			GUIMessage_t *pMessage;

			pMessage = gGUIQueue.front();
			CPoint newPoint(pMessage->Param2, pMessage->Param3);
			mGroundSelect.SetLatestCardPosition(newPoint);
			pDC = this->GetWindowDC(); 
			mGroundSelect.DrawFace(pDC);
			ReleaseDC(pDC);
			break;
		}
		case MIXED_CARD:
			break; 
		case SHOW_MAIN_MENU:
			mStroyBoard.mLock=1;
			break; 
		case SELECT_MAIN_MENU:
			mSelectMenu.mLock=1;
			break; 
		case HIDE_MAIN_MENU:
			mStroyBoard.mLock=0;
			GameStartSeq();
			break;
		case SELECT_GO_OR_STOP:
		{
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 2;
			auto & Player = CPlayer::GetInstance(ID_PLAYER1);
			switch (Player.mStatus.NumOfGo)
			{
			case 0:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_1xGO].mMessage;
				break;
			case 1:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_2xGO].mMessage;
				break;
			case 2:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_3xGO].mMessage;
				break;
			case 3:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_4xGO].mMessage;
				break;
			case 4:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_5xGO].mMessage;
				break;
			case 5:
				mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_6xGO].mMessage;
				break;
			}
			mSelectMenu.CardArray[0] = pMessage->Param1;	//Score
			mSelectMenu.CardArray[1] = pMessage->Param2;	//Sub Score
			mSelectMenu.mLock = 1;
			break;
		}
		case SELECT_CHONGTONG:
		{
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 4;
			mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_CHONG_TONG].mMessage;
			mSelectMenu.CardArray[0] = pMessage->Param1;
			mSelectMenu.CardArray[1] = pMessage->Param2;
			mSelectMenu.CardArray[2] = pMessage->Param3;
			mSelectMenu.CardArray[3] = pMessage->Param4;
			mSelectMenu.mLock = 1;
			break;
		}
		case SELECT_WAIT_OK:
			mSelectMenu.mLock = 1;
			break; 
		case SELECT_TO_OF_ONE:
		{
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 2;
			mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_CHOOSE_ONE_OF_TWO].mMessage;
			mSelectMenu.CardArray[0] = pMessage->Param1;
			mSelectMenu.CardArray[1] = pMessage->Param2;
			mSelectMenu.mLock = 1;
			break;
		}
		case SELECT_GOOKJIN:
		{
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 1;
			mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_USE_IT_DOUBLE_P].mMessage;
			mSelectMenu.CardArray[0] = pMessage->Param1;
			mSelectMenu.CardArray[1] = pMessage->Param2;
			mSelectMenu.mLock = 1;
			break;
		}
		case SELECT_WINDOW_HIDE: 
			mSelectMenu.mLock = 0;
			break;
		case MESSEGE_ASSA_ALL_IS_MINE:
		{
			StrMessege = _T("앗싸! 다내꺼!!;이것도 실력!");
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mGroundSelect.AddMessege(StrMessege, 60, pMessage->Param1);
			/*if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);
				*/
			break;
		}
		case MESSEGE_ASSA_ZZOCK:
			{
				StrMessege = _T("오호 쪽!;땡쓰!");
				GUIMessage_t *pMessage;
				pMessage = gGUIQueue.front();
				mGroundSelect.AddMessege(StrMessege, 60, pMessage->Param1);
				break;
			}
		case MESSEGE_ASSA_GODORI:
			StrMessege = _T("아싸 고도리;새다섯마리 다오셨네;고도리는 내꺼!");
			if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);
			
			break;
		case MESSEGE_ASSA_CHUNGDAN:
			StrMessege = _T("아싸 청단!;청단했지롱;");
			if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);
			
			break;
		case MESSEGE_ASSA_HONGDAN:
			StrMessege = _T("아싸 홍단!;홍단했지롱");
			if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);
			
			break;
		case MESSEGE_ASSA_CHODAN:
			StrMessege = _T("아싸 초단!;초단했지롱");
			if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);
			
			break;
		case MESSEGE_ASSA_FLOOR_CLAER:
			{
				StrMessege = _T("쓸~~! 한장 주시고~!!");
				GUIMessage_t *pMessage;
				pMessage = gGUIQueue.front();
				mGroundSelect.AddMessege(StrMessege, 60, pMessage->Param1);
				break;
			}
		case MESSEGE_GO:
		{
			
			//switch (Player.mStatus.NumOfGo)
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			if (mTurn == ID_PLAYER1)
			{
				auto & Player = CPlayer::GetInstance(ID_PLAYER1);
				StrMessege.Format(_T("%d Go!!"), Player.mStatus.NumOfGo);
				mSelectBar.AddMessege(StrMessege, 60);
			}
			else
			{
				auto & Player = CPlayer::GetInstance(ID_PLAYER2);
				StrMessege.Format(_T("%d Go!!"), Player.mStatus.NumOfGo);
				mOpSelectBar.AddMessege(StrMessege, 60);
			}
			break;
		}
		case MESSEGE_STOP:
		{
			StrMessege = _T("Stop!!");
			if (mTurn == ID_PLAYER1)
				mSelectBar.AddMessege(StrMessege, 60);
			else
				mOpSelectBar.AddMessege(StrMessege, 60);

			break;
		}
		case SELECT_WIN:
		{
			
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 1;
			mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_WIN].mMessage;
			mSelectMenu.CardArray[0] = pMessage->Param1;
			mSelectMenu.CardArray[1] = pMessage->Param2;
			mSelectMenu.mLock = 1;
			break;
			
			break;
		}
		case SELECT_LOST:
		{
			GUIMessage_t *pMessage;
			pMessage = gGUIQueue.front();
			mSelectMenu.nItem = 1;
			mSelectMenu.strMenuTitle = SelectMenuPresetting[SELECT_MENU_LOST].mMessage;
			mSelectMenu.CardArray[0] = pMessage->Param1;
			mSelectMenu.CardArray[1] = pMessage->Param2;
			mSelectMenu.mLock = 1;
			break;
			break;
		}

		case SELECT_BUCK:
			{
				StrMessege = _T("아이쿠 쌋다!");
				GUIMessage_t *pMessage;
				pMessage = gGUIQueue.front();
				mGroundSelect.AddMessege(StrMessege, 60, pMessage->Param1);

				break;
			}
	};

	return 0;
}
