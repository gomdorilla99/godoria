#pragma once
#include <list>
#include <queue>
#include "card.h"
#define MAX_CARD 52
class CDiler;
using namespace std;

class CGroundSelect
{
public:
	CGroundSelect(void);
	~CGroundSelect(void);
	CDC BackDC;	//ī�� �޸� 
private : 
	
	CDC *mpDC;
	CDC OctopusDC;
	CDC MemoryDC; // �޸� DC�� �����ϰ� pDC�� ȣȯ�ǵ��� ����

	CFont mBaseFont;
	CFont mBigFont;	// ū��Ʈ 
	CDiler *mpDiler;
	CBitmap curBitmap; // ��Ʈ�� ��ü ����
	CBitmap CardBack;
	CBitmap BuffBMP;
	CBitmap Octopus;

	CBrush BackBrush;
	CBrush BackShadowBrush;
	CBrush RedCard;
	CPen NULLPen;
	CBrush BackGround;
	CBitmap BackGroundImage;
	queue<CMessege_t *> mMessageQ;
	void DrawToast(int x, int y, CDC * pDC, int oposit, CString Message);

	void DrawCeterTower(CDC *pDC);
	
	

	
	CPoint Gravity(CPoint p1, CPoint p2,int m1, int m2);
	int GravityX(CPoint p1, int X,int m1, int m2);
	int GravityY(CPoint p1, int Y,int m1, int m2);
public:
	int mActiveStage;
	int SetLatestCardPosition(CPoint newPoint);
	int AddMessege(CString & StrMessage, int Count, int CardNumber);
	int CenterTowerOpen;
	//void SetOpsit(void);	
	CRect mRegion;
	CPoint CenterPoint;
	CPoint mMousePoint;
	
	int mTurn;
	int nItem;
	int SetRegion(CRect rect);
	int SetMousePoint(CPoint);
	int DrawFace(CDC *pDC);
	int DCUpdate(CDC * pDC);
	int InitSelectBar(CDC *pDC,CRect *crect);

	void FligtTo(int index, long int & x, long int & y, int xx, int yy);
	int DrawCard(CDC *pDC, int index);
	int GForce;
	int nGo,nNG,nPoint;
	void SetTurn(int turn);
	void DrawPopText(int x,int y,CDC *pDC, int oposit,CString Message);
};
