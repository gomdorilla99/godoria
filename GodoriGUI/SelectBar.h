#pragma once
#include <queue>
#include "Card.h"

using namespace std;
class CPlayer;
constexpr auto MAX_CARDITEM = 12;


class CSelectBar
{
private : 
	int mPlayerId;
	CDC *mpDC;
	CDC BackDC;
	
	CDC MemoryDC; // 메모리 DC를 생성하고 pDC와 호환되도록 설정
	CDC OctopusDC;
	CBitmap Octopus;
	CBitmap curBitmap; // 비트맵 객체 생성
	CBitmap CardBack;
	CBitmap BuffBMP;
	
	CBrush BackBrush;
	CBrush BackShadowBrush;
	CBrush RedCard;
	CBrush OCTOPUS;
	CPen NULLPen;
	CPen YELLOPen;
	CPen GREENPen;
	CFont mBaseFont; 
	CFont mBigFont;

	struct { //Event를 보여주기 위해서 (1번씩만)
		//7점 이상에서 Go or Stop을 정할수 있음
		int NumOfGo;		//고를 부른 횟수 1G +1점, 2Go + 2점  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		int NumOfBuck;
		int GoDori;
		int ChungDan;
		int HongDan;		//Godory = 5점
		int ChoDan;
		int Shake;
		int Mungtungguri;	// 열끝 7장 이상일 경우 총점수 x2
	} mStatus;

	queue<CMessege_t *> mMessageQ;
	int option_opsit;
	int shake_x;
	int shake_y;
public:
	bool isMyTurn;
	void SetPlayerID(int player_id){mPlayerId = player_id;};
	CPoint refPoint[20];

	void SetOpsit(void);
	int SetPlayerTurn(CDC * pDC, bool bMyTurn);

	void DrawToast(int x, int y, CDC * pDC, int oposit, CString Message);

	CRect mRegion;
	CPoint CenterPoint;
	CPoint mMousePoint;
	CSelectBar(void);
	~CSelectBar(void);
	int nItem;
	int AddMessege(CString & StrMessage, int Count);
	int SetRegion(CRect rect);
	int SetMousePoint(CPoint);
	int DrawFace(CDC *pDC);
	int InitSelectBar(CDC *pDC,CRect *crect);
	unsigned int GetCardLocation(int index, long int & x, long int & y);
	int DrawCard(CDC *pDC, int index);
	int DrawBaseCard(CDC *pDC,int group, int index);
	void DrawFlatText(int x, int y, CDC * pDC, int oposit, CString Message);
	void DrawPopText(int x,int y,CDC *pDC, int oposit,CString Message);
	CPlayer *GetPlayer();

};
