#pragma once

#include "Card.h"
#include <list>

using namespace std;

class CPlayer;

enum {
	SELECT_MENU_1xGO,
	SELECT_MENU_2xGO,
	SELECT_MENU_3xGO,
	SELECT_MENU_4xGO,
	SELECT_MENU_5xGO,
	SELECT_MENU_6xGO,
	SELECT_MENU_7xGO,
	SELECT_MENU_8xGO,
	SELECT_MENU_USE_IT_DOUBLE_P,
	SELECT_MENU_CHOOSE_ONE_OF_TWO,
	SELECT_MENU_CHONG_TONG,
	SELECT_MENU_FIRST_TRIPLE_FAULT,
	SELECT_MENU_THIRED_TRIPLE_FAULT,
	SELECT_MENU_SHAKE,
	SELECT_MENU_WIN,
	SELECT_MENU_BANKOUT,
	SELECT_MENU_LOST,
	NUM_OF_SELECT_MENU
};

enum {
	SELECT_RESULT_GO,
	SELECT_RESULT_STOP,
	SELECT_RESULT_YEOL,
	SELECT_RESULT_DUOUBLE_P,
	SELECT_RESULT_LEFT,
	SELECT_RESULT_RIGHT,
	SELECT_RESULT_OK,
	SELECT_RESULT_CONTINUE,
	SELECT_RESULT_GAME_STOP,
	SELECT_RESULT_NO,
	SELECT_RESULT_SHAKE,
};
class CMenuPresetting
{
public:
	int mMENU_ID;
	CString mMessage;
	int mNumberOfSelect;
	int mResult1;
	int mResult2;
	CMenuPresetting(int menu_id, wchar_t *message, int number_of_select, int result1, int result2);
};



class SelectCardMenu
{
	CDC *mpDC;
	CDC BackDC;
	CDC MemoryDC; // ¸Þ¸ð¸® DC¸¦ »ý¼ºÇÏ°í pDC¿Í È£È¯µÇµµ·Ï ¼³Á¤
	CBitmap BuffBMP;
	CBitmap curBitmap; // ºñÆ®¸Ê °´Ã¼ »ý¼º
	CFont mBaseFont;
	CRect mRegion;
	
	CPoint CenterPoint;
	CPoint mMousePoint;
	CMenuPresetting *mpPresetting;
	CString mStrComment;
	CString mStrButton1;
	CString mStrButton2;
	CString mStrButton3;
	int result1;
	int result2;
	int result3;
	CPen WhitePen[5];
	CPen NULLPen;
	CPoint refPoint[4];
	CPoint Flower[1000];
	CPoint FlowerVelocity[1000];
	CPoint FlowerDest[1000];

public:
	int mLock; 
	int nItem;
	int CardArray[5];
	CString strMenuTitle;
	void PopupSelectMenu()
	{
		mLock = 1;
	};
	int SetMenuSetting(int ID); //±¹ÁøÇÇ ½ÖÇÇ, °í or ½ºÅé, ½Â¸®, ÆÐ¹è
	int SetMenuSetting(int ID, CCard *pLeft, CCard *pRight); //Èçµé±â, Ã¹»¶, ¾²¸®»¶ 
	int SetMenuSetting(int ID, list<CCard *> &CardList);	//ÃÑÅë


	SelectCardMenu();
	~SelectCardMenu();
	int SetRegion(CRect rect);
	int SetMousePoint(CPoint point);
	CPlayer * GetPlayer();
	int DrawFace(CDC * pDC);
	unsigned int GetCardLocation(int index, long int & x, long int & y);
	int DrawCard(CDC *pDC, int index, int nCard, CCard *pCard);
	void DrawPopText(int x, int y, CDC * pDC, CString Message);
	void DrawPopText1(int x, int y, CDC * pDC, CString Message);

	int InitSelectCardMenu(CDC * pDC, CRect * crect);
};

