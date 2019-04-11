#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <list>
#include <windows.h>
#include <algorithm>
#include <stdio.h>
#include "Player.h"
#include "Diler.h"
#include "CardPorket.h"
#include "Card.h"
#include "game.h"

using namespace std;
extern queue<GUIMessage_t *> gGUIQueue;

extern CGodoriGUIView *G_Gostop;

extern HANDLE ghSemaphore_To_Diller;
extern HANDLE ghSemaphore_To_Player1;
extern HANDLE ghSemaphore_To_Player2;

extern HANDLE ghSemaphore_Wait_UI_Select;
extern queue<GUI_SELECT_t *> gSelctQueue;


CCard staticCardList[54]={
	{ 0,	NoP	,ANIMAL_NO	, 1,BAND_NO		,HAVE_Guang	, _T("송학광")},	//송학 광
	{ 1,	NoP	,ANIMAL_NO	, 1,BAND_RED	,No_Guang	, _T("송학홍단") },	//송학 홍단
	{ 2,	OneP,ANIMAL_NO	, 1,BAND_NO		,No_Guang	, _T("송학피") },	//송학 피
	{ 3,	OneP,ANIMAL_NO	, 1,BAND_NO		,No_Guang	, _T("송학피") },	//송학 피
	{ 4,	YEOL,ANIMAL_BRID, 2,BAND_NO		,No_Guang	, _T("매조끗") },	//매조 끗(새)
	{ 5,	NoP	,ANIMAL_NO	, 2,BAND_RED	,No_Guang	, _T("매화홍단") },	//매화 홍단
	{ 6,	OneP,ANIMAL_NO	, 2,BAND_NO		,No_Guang	, _T("매화피") },	//매화 피
	{ 7,	OneP,ANIMAL_NO	, 2,BAND_NO		,No_Guang	, _T("매화피") },	//매화 피
	{ 8,	NoP	,ANIMAL_NO	, 3,BAND_NO		,HAVE_Guang , _T("사쿠라광") },	//사쿠라 광
	{ 9,	NoP	,ANIMAL_NO	, 3,BAND_RED	,No_Guang	, _T("사쿠라홍단") },	//사쿠라 홍단
	{ 10,	OneP,ANIMAL_NO	, 3,BAND_NO		,No_Guang	, _T("사쿠라피") },	//사쿠라 피
	{ 11,	OneP,ANIMAL_NO	, 3,BAND_NO		,No_Guang	, _T("사쿠라피") },	//사쿠라 피
	{ 12,	YEOL,ANIMAL_BRID, 4,BAND_NO		,No_Guang	, _T("싸리끗(새)") },	//싸리 끗(새)
	{ 13,	NoP	,ANIMAL_NO	, 4,BAND_GLASS	,No_Guang	, _T("싸리초단") },	//싸리 초단
	{ 14,	OneP,ANIMAL_NO	, 4,BAND_NO		,No_Guang	, _T("싸리피") },	//싸리 피
	{ 15,	OneP,ANIMAL_NO	, 4,BAND_NO		,No_Guang	, _T("싸리피") },	//싸리 피
	{ 16,	YEOL,ANIMAL_NO	, 5,BAND_NO 	,No_Guang	, _T("난초끗") },	//난초 끗//
	{ 17,	NoP	,ANIMAL_NO	, 5,BAND_GLASS	,No_Guang	, _T("난초초단") },	//난초 초단 
	{ 18,	OneP,ANIMAL_NO	, 5,BAND_NO		,No_Guang	, _T("난초피") },	//난초 피
	{ 19,	OneP,ANIMAL_NO	, 5,BAND_NO		,No_Guang	, _T("난초피") },	//난초 피
	{ 20,	YEOL,ANIMAL_NO	, 6,BAND_NO 	,No_Guang	, _T("모란끗") },	//모란 끛
	{ 21,	NoP	,ANIMAL_NO	, 6,BAND_BLUE	,No_Guang	, _T("모란청단") },	//모란 청단
	{ 22,	OneP,ANIMAL_NO	, 6,BAND_NO		,No_Guang	, _T("모란피") },	//모란 피
	{ 23,	OneP,ANIMAL_NO	, 6,BAND_NO		,No_Guang	, _T("모란피") },	//모란 피
	{ 24,	YEOL,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("홍사리끗") },	//홍사리 끝
	{ 25,	NoP	,ANIMAL_NO	, 7,BAND_GLASS	,No_Guang	, _T("홍사리초단") },	//홍사리 초단
	{ 26,	OneP,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("홍사리피") },	//홍사리 피
	{ 27,	OneP,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("홍사리피") },	//홍사리 피
	{ 28,	NoP	,ANIMAL_NO	, 8,BAND_NO		,HAVE_Guang , _T("공산광") },	//공산 광
	{ 29,	NoP	,ANIMAL_TREE_BRID	, 8,BAND_NO		,No_Guang , _T("공산끗") },	//공산 끗(새3)
	{ 30,	OneP,ANIMAL_NO	, 8,BAND_NO		,No_Guang	, _T("공산피") },	//공산 열
	{ 31,	OneP,ANIMAL_NO	, 8,BAND_NO		,No_Guang	, _T("공산피") },	//공산 피
	{ 32,	YEOL,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("국진끗") },	//국진 끗
	{ 33,	NoP	,ANIMAL_NO	, 9,BAND_BLUE	,No_Guang	, _T("국진청단") },	//국진 청단
	{ 34,	OneP,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("국진피") },	//국진 피
	{ 35,	OneP,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("국진피") },	//국진 피
	{ 36,	YEOL,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("단풍끗") },	//단풍 끗
	{ 37,	NoP	,ANIMAL_NO	,10,BAND_BLUE	,No_Guang	, _T("단풍청단") },	//단풍 청단
	{ 38,	OneP,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("단풍피") },	//단풍 피	
	{ 39,	OneP,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("단풍피") },	//단풍 피
	{ 40,	NoP	,ANIMAL_NO	,11,BAND_NO		,HAVE_Guang , _T("오동광") },	//오동 광
	{ 41,	TwoP,ANIMAL_NO	,11,BAND_NO 	,No_Guang	, _T("오동쌍피") },	//오동 쌍피
	{ 42,	OneP,ANIMAL_NO	,11,BAND_NO		,No_Guang	, _T("오동피") },	//오동 피
	{ 43,	OneP,ANIMAL_NO	,11,BAND_NO 	,No_Guang	, _T("오동피") },	//오동 피
	{ 44,	NoP	,ANIMAL_NO	,12,BAND_NO		,HAVE_Guang , _T("비광") },	//비광
	{ 45,	YEOL,ANIMAL_NO	,12,BAND_NO		,No_Guang	, _T("비끗") },	//비끗	새로 안침
	{ 46,	NoP	,ANIMAL_NO	,12,BAND_GLASS 	,No_Guang	, _T("비초단") },	//비초단 
	{ 47,	TwoP,ANIMAL_NO	,12,BAND_NO 	,No_Guang	, _T("비쌍피") },	//비쌍피
	{ 48,	TwoP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("보너스2") },
	{ 49,	ThreeP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("보너스3") },
	{ 50,	ThreeP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("보너스3") },
	{ 51,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("폭탄") },
	{ 52,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("GO") },
	{ 53,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("STOP")},
};


void CDiler::DoMixCard()
{
	TRACE("딜러가 카드를 섞고 있습니다.\r\n");
	int num = 0;
	int CheckBoard[FULL_CARD_NUMBER] = { 0, };
	srand((unsigned)time(NULL));
	for (int i = 0; i < FULL_CARD_NUMBER; i)
	{
		num = rand() % (FULL_CARD_NUMBER + 1);
		if (CheckBoard[num] == 0)
		{
			CheckBoard[num] = 1;
			mMixedCardArry[i] = (CCard*)(staticCardList + num);
			i++;
		}
	}
	NumOfRemainCard = FULL_CARD_NUMBER;
}

CCard* CDiler::PopCard()	// 빼서 전달
{
	NumOfRemainCard--;
//	TRACE("Diler Now, %d card remain\r\n", NumOfRemainCard);
	if (NumOfRemainCard > 0)
		return mMixedCardArry[NumOfRemainCard];
	else
		return nullptr;
}

CCard *CDiler::Push_OnFloor(CCard *pCard)
{
	mOnFloor.Push(pCard);
	return nullptr;
}


int CDiler::CalculatePoint()
{
	TRACE("[Diller]\r\n");
	mOnFloor.SimpleList();
	return  0;
}

void CDiler::MatchingCard(CCard * pCard, CCardPorket *MatchedCardList)
{

}



void CDiler::DispatchCard()
{
	// 10장 뽑아서 전달
	CCard *pCard;
	TRACE("[Diler]\r\n");
	for (int i = 0; i < 8; i++)	// 바닥 8
	{
		pCard = PopCard();

		Push_OnFloor(pCard);
		//	pCard->PrintCard;
	}
	//TRACE("[Player1]");
	for (int i = 0; i < 10; i++) // 1번 사용자 5
	{
		pCard = PopCard();


		mpPlayer1->Push_InHand(pCard);
		//	pCard->PrintCard;
	}
	//TRACE("[Player2]");
	for (int i = 0; i < 10; i++) // 2번 사용자 5
	{
		pCard = PopCard();
		mpPlayer2->Push_InHand(pCard);
		//	pCard->PrintCard;
	}
	mpPlayer1->mInHand.Sort();
	mpPlayer2->mInHand.Sort();

	mpPlayer1->CalculatePoint();
	mpPlayer2->CalculatePoint();
	return;
}


int CDiler::GUI_DRAW(int gui, int attack, int param1, int param2, int param3, int param4, int param5, int param6)
{
	CString StrMessage;
	//StrMessage.Format(_T("Player %d, GUI %d"), attack, gui);
	//AfxMessageBox(StrMessage);
	GUIMessage_t *pNewMessage = new GUIMessage_t;
	pNewMessage->GUI_COMMAND = gui;
	pNewMessage->Turn = attack;
	pNewMessage->Param1 = param1;
	pNewMessage->Param2 = param2;
	pNewMessage->Param3 = param3;
	pNewMessage->Param4 = param4;
	pNewMessage->Param5 = param5;
	pNewMessage->Param6 = param6;
	gGUIQueue.push(pNewMessage);
	return 0;
}

CDiler::CDiler() :NumOfRemainCard(0), Singleton<CDiler>(ID_DILLER)
{
	if (NumOfRemainCard == 0)
	{
		NumOfRemainCard = FULL_CARD_NUMBER;
		DoMixCard();
	}

};
int CDiler::GUI_GETSELECT(int gui, int attack) {
	int index = 0;
	CPlayer *pPlayer = (attack == ID_PLAYER1)? (CPlayer *)mpPlayer1:(CPlayer *)mpPlayer2;

	//Send Select Message
	DWORD dwWaitResult;
	GUI_SELECT_t *pSelect;

	TRACE(_T("골라봐"));
	dwWaitResult = WaitForSingleObject(
		ghSemaphore_Wait_UI_Select,  // handle to semaphore
		10000000UL);      // zero-second time-out interval

	switch (dwWaitResult)
	{
		// The semaphore object was signaled.
	case WAIT_OBJECT_0:
		//선택된 값이 뭔지 알아내어야한다
		if (gSelctQueue.size())
		{
			pSelect = gSelctQueue.front();
			index = -1;
			gSelctQueue.pop();
			if (pSelect)
			{
				index = pSelect->SELECT_INDEX;
				delete pSelect;
			}
		}
		break;

		// The semaphore was nonsignaled, so a time-out occurred.
	case WAIT_TIMEOUT:
		TRACE("Diller(%d): wait timed out\n", GetCurrentThreadId());
		break;
	}

	//Wait Semaphore
ReTry:
	return index;
}





int CDiler::DoMainMenu(void)
{
	int index;
	GUI_DRAW(SHOW_MAIN_MENU, ID_FOR_EVERY,0,0,0,0,0,0);

	index = GUI_GETSELECT(SELECT_MAIN_MENU, ID_FOR_EVERY);
	switch (index) {
	case 0:	//New Game
	case 1:	//Continue Game
		GUI_DRAW(HIDE_MAIN_MENU, ID_FOR_EVERY, 0, 0, 0, 0, 0, 0);

		break;
	case 2:
		ShowScorePannel();
		break;
	case 3:
		EndingPannel();
		return -1;
		break;
	}
	   	 

	return 1;
}

int CDiler::DoGame()
{
	mpPlayer1 = &CPlayer::GetInstance(ID_PLAYER1);
	mpPlayer2 = &CPlayer::GetInstance(ID_PLAYER2);

	if (!ReleaseSemaphore(
		ghSemaphore_To_Diller, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}
	CCard *pFirstCard, *pSecandCard;

	CCardPorket temp;
	list<CCard*> MatchingList_1st;
	list<CCard*> MatchingList_2nd;

	
	if (DoMainMenu() == -1) {
		return -1;
	}

	DoMixCard();
	GUI_DRAW(MIXED_CARD, ID_DILLER, 0, 0, 0, 0, 0, 0);
	DispatchCard();//
	GUI_DRAW(GIVE_CARD, ID_PLAYER1, 0, 0, 0, 0, 0, 0);
	GUI_DRAW(GIVE_CARD, ID_PLAYER2, 0, 0, 0, 0, 0, 0);
	CalculatePoint();
	return 0;
}


int CDiler::ShowScorePannel() {
	return 0;
}
int CDiler::EndingPannel() {
	return 0;
}