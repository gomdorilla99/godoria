#pragma once
#include "Card.h"
#include "CardPorket.h"
#include "Player.h"
#include "Singleton.h"
#include "GodoriGUIView.h"
#include "game.h"

#define FULL_CARD_NUMBER (48+3)

typedef struct
{
	int GUI_COMMAND;	// Action
	int Turn;
	int Param1;			// Who
	int Param2;			// Card#
	int Param3;			// Card#
	int Param4;			// Card#
	int Param5;			// Card#
	int Param6;			// Card#


}GUIMessage_t;

class CPlayer;
class CPlayer1;
class CPlayer2;
typedef struct
{
	int ID;	// Action
	int SELECT_INDEX;			// Who
	CCard *pLeftCard;
	CCard *pRightCard;
	list<CCard *> *pCardList;
}GUI_SELECT_t;

class CDiler:  public Singleton<CDiler>
{
	friend class Singleton<CDiler>;
private:
	int NumOfRemainCard;
	CPlayer *mpPlayer1;
	CPlayer *mpPlayer2;	
public:	
	CCardPorket	mOnFloor;
	int chance;

	CCardPorket *GetFloor()
	{
		return &mOnFloor;
	};

	int	GetCount()
	{
		return NumOfRemainCard;
	}
	
	CCard * mMixedCardArry[FULL_CARD_NUMBER];
	//Mix Card 
	void DoMixCard();
	CCard *PopCard();	// »©¼­ Àü´Þ
	CCard *Push_OnFloor(CCard *pCard);
	void MatchingCard(CCard * pCard, CCardPorket *MatchedCardList);

	int CalculatePoint();
	CDiler();
	
	int DoMainMenu(void);

	int DoGame();
	int ShowScorePannel();
	int EndingPannel();
	void DispatchCard();
	
	int GUI_DRAW(int gui, int attack, int param1, int param2, int param3, int param4, int param5, int param6);
	int GUI_GETSELECT(int gui, int attack);
};

