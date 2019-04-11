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
	
	CDC MemoryDC; // �޸� DC�� �����ϰ� pDC�� ȣȯ�ǵ��� ����
	CDC OctopusDC;
	CBitmap Octopus;
	CBitmap curBitmap; // ��Ʈ�� ��ü ����
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

	struct { //Event�� �����ֱ� ���ؼ� (1������)
		//7�� �̻󿡼� Go or Stop�� ���Ҽ� ����
		int NumOfGo;		//�� �θ� Ƚ�� 1G +1��, 2Go + 2��  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		int NumOfBuck;
		int GoDori;
		int ChungDan;
		int HongDan;		//Godory = 5��
		int ChoDan;
		int Shake;
		int Mungtungguri;	// ���� 7�� �̻��� ��� ������ x2
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
