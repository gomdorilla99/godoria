#pragma once

class CStoryBoard
{
public:
	CStoryBoard(void);
	~CStoryBoard(void);


private : 
	CDC *mpDC;
	CDC BackDC;
	CDC MemoryDC; // 메모리 DC를 생성하고 pDC와 호환되도록 설정
	CBitmap curBitmap; // 비트맵 객체 생성
	CBitmap CardBack;
	CBitmap BuffBMP;
	CBrush BackBrush;
	CBrush BackShadowBrush;
	CBrush RedCard;
	CPen NULLPen;
	CPen WhitePen[5];
	CFont mBaseFont; 
	CFont mSmallFont; 
	
	CPoint Gravity(CPoint p1, CPoint p2,int m1, int m2);
	int GravityX(CPoint p1, int X,int m1, int m2);
	int GravityY(CPoint p1, int Y,int m1, int m2);
	int SetDest(CDC *pDC,CString Message);

public:
	CPoint mrefPoint[41];
	CPoint Velocity[41];
	CPoint power[41];
	
	int nFlower;
	CPoint Flower[1000];
	CPoint FlowerVelocity[1000];
	CPoint FlowerDest[1000];
	int mLock;
	void SetOpsit(void);	
	CRect mRegion;
	CPoint CenterPoint;
	CPoint mMousePoint;
	CPoint mOldMousePoint;
	int stage;
	int nItem;
	int SetRegion(CRect rect);
	int SetMousePoint(CPoint);
	int DrawFace(CDC *pDC);
	int InitSelectBar(CDC *pDC,CRect *crect);
	int DrawCard(CDC *pDC, int index);
	int GForce;
	int nGo,nNG,nPoint;
	void DrawPopText(int x,int y,CDC *pDC,CString Message);
	int DoEnding(void);
};
