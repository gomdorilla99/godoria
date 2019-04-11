
// GodoriGUIView.h: CGodoriGUIView 클래스의 인터페이스
//
#pragma once
#include "game.h"
#include "SelectBar.h"
#include "GroundSelect.h"
#include "StoryBoard.h"
#include "GodoriGUIDoc.h"
#include "SelectCardMenu.h"

enum{
		ANIMATION_MOVE,
		ANIMATION_CENTER_CARD_OPEN,
		ANIMATION_PLAYER1_1GO,
		ANIMATION_PLAYER1_2GO,
		ANIMATION_PLAYER1_3GO,
		ANIMATION_PLAYER1_WIN,
		ANIMATION_PLAYER1_LOST
};

class CAnimationItem
{
public:
	CAnimationItem(void){
		iStep=0;
		nCard=0;
	};
	CAnimationItem(CPoint _StartPoint,CPoint _EndtPoint,int delay, int nCount);
	int MotionType;
	int DelayCnt;
	CPoint CurPoint;
	CPoint StartPoint;
	CPoint EndPoint;
	int iStep;
	int nCard;
	int nStep;
	CAnimationItem *next;
public:
	int DoAnimation(CDC *pDC);
};



class CGodoriGUIView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGodoriGUIView() noexcept;
	DECLARE_DYNCREATE(CGodoriGUIView)

// 특성입니다.
public:
	CGodoriGUIDoc* GetDocument() const;

// 작업입니다.
public:
	int	mTurn;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGodoriGUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
	int DoAnimation(CDC * pDC);
	void OnPaint();
	void GameStartSeq(void);
private: 
	CSelectBar mSelectBar;
	CSelectBar mOpSelectBar;
	CGroundSelect mGroundSelect;
	CStoryBoard mStroyBoard;
	SelectCardMenu mSelectMenu;

	CBrush RedCard;
	CBrush BackBrush;
	CBrush BackShadowBrush;
	CBitmap CardBack;
	CPen NULLPen;
	CPen YelloPen;
	CRect rctCenterCard;

	//void DrawCeterTower(CDC *pDC);
	void DrawPopup(CDC *pDC, int oposit,CString Message);
	bool select_done;
	int mIndex;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int ChekGUIMessage();
	afx_msg LRESULT OnGUIHandler(WPARAM wParam, LPARAM lParam); 
};

#ifndef _DEBUG  // GodoriGUIView.cpp의 디버그 버전
inline CGodoriGUIDoc* CGodoriGUIView::GetDocument() const
   { return reinterpret_cast<CGodoriGUIDoc*>(m_pDocument); }
#endif

