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
	{ 0,	NoP	,ANIMAL_NO	, 1,BAND_NO		,HAVE_Guang	, _T("���б�")},	//���� ��
	{ 1,	NoP	,ANIMAL_NO	, 1,BAND_RED	,No_Guang	, _T("����ȫ��") },	//���� ȫ��
	{ 2,	OneP,ANIMAL_NO	, 1,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 3,	OneP,ANIMAL_NO	, 1,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 4,	YEOL,ANIMAL_BRID, 2,BAND_NO		,No_Guang	, _T("������") },	//���� ��(��)
	{ 5,	NoP	,ANIMAL_NO	, 2,BAND_RED	,No_Guang	, _T("��ȭȫ��") },	//��ȭ ȫ��
	{ 6,	OneP,ANIMAL_NO	, 2,BAND_NO		,No_Guang	, _T("��ȭ��") },	//��ȭ ��
	{ 7,	OneP,ANIMAL_NO	, 2,BAND_NO		,No_Guang	, _T("��ȭ��") },	//��ȭ ��
	{ 8,	NoP	,ANIMAL_NO	, 3,BAND_NO		,HAVE_Guang , _T("�����") },	//����� ��
	{ 9,	NoP	,ANIMAL_NO	, 3,BAND_RED	,No_Guang	, _T("�����ȫ��") },	//����� ȫ��
	{ 10,	OneP,ANIMAL_NO	, 3,BAND_NO		,No_Guang	, _T("�������") },	//����� ��
	{ 11,	OneP,ANIMAL_NO	, 3,BAND_NO		,No_Guang	, _T("�������") },	//����� ��
	{ 12,	YEOL,ANIMAL_BRID, 4,BAND_NO		,No_Guang	, _T("�θ���(��)") },	//�θ� ��(��)
	{ 13,	NoP	,ANIMAL_NO	, 4,BAND_GLASS	,No_Guang	, _T("�θ��ʴ�") },	//�θ� �ʴ�
	{ 14,	OneP,ANIMAL_NO	, 4,BAND_NO		,No_Guang	, _T("�θ���") },	//�θ� ��
	{ 15,	OneP,ANIMAL_NO	, 4,BAND_NO		,No_Guang	, _T("�θ���") },	//�θ� ��
	{ 16,	YEOL,ANIMAL_NO	, 5,BAND_NO 	,No_Guang	, _T("���ʲ�") },	//���� ��//
	{ 17,	NoP	,ANIMAL_NO	, 5,BAND_GLASS	,No_Guang	, _T("�����ʴ�") },	//���� �ʴ� 
	{ 18,	OneP,ANIMAL_NO	, 5,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 19,	OneP,ANIMAL_NO	, 5,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 20,	YEOL,ANIMAL_NO	, 6,BAND_NO 	,No_Guang	, _T("�����") },	//��� ��
	{ 21,	NoP	,ANIMAL_NO	, 6,BAND_BLUE	,No_Guang	, _T("���û��") },	//��� û��
	{ 22,	OneP,ANIMAL_NO	, 6,BAND_NO		,No_Guang	, _T("�����") },	//��� ��
	{ 23,	OneP,ANIMAL_NO	, 6,BAND_NO		,No_Guang	, _T("�����") },	//��� ��
	{ 24,	YEOL,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("ȫ�縮��") },	//ȫ�縮 ��
	{ 25,	NoP	,ANIMAL_NO	, 7,BAND_GLASS	,No_Guang	, _T("ȫ�縮�ʴ�") },	//ȫ�縮 �ʴ�
	{ 26,	OneP,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("ȫ�縮��") },	//ȫ�縮 ��
	{ 27,	OneP,ANIMAL_NO	, 7,BAND_NO		,No_Guang	, _T("ȫ�縮��") },	//ȫ�縮 ��
	{ 28,	NoP	,ANIMAL_NO	, 8,BAND_NO		,HAVE_Guang , _T("���걤") },	//���� ��
	{ 29,	NoP	,ANIMAL_TREE_BRID	, 8,BAND_NO		,No_Guang , _T("�����") },	//���� ��(��3)
	{ 30,	OneP,ANIMAL_NO	, 8,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 31,	OneP,ANIMAL_NO	, 8,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 32,	YEOL,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 33,	NoP	,ANIMAL_NO	, 9,BAND_BLUE	,No_Guang	, _T("����û��") },	//���� û��
	{ 34,	OneP,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 35,	OneP,ANIMAL_NO	, 9,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 36,	YEOL,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("��ǳ��") },	//��ǳ ��
	{ 37,	NoP	,ANIMAL_NO	,10,BAND_BLUE	,No_Guang	, _T("��ǳû��") },	//��ǳ û��
	{ 38,	OneP,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("��ǳ��") },	//��ǳ ��	
	{ 39,	OneP,ANIMAL_NO	,10,BAND_NO		,No_Guang	, _T("��ǳ��") },	//��ǳ ��
	{ 40,	NoP	,ANIMAL_NO	,11,BAND_NO		,HAVE_Guang , _T("������") },	//���� ��
	{ 41,	TwoP,ANIMAL_NO	,11,BAND_NO 	,No_Guang	, _T("��������") },	//���� ����
	{ 42,	OneP,ANIMAL_NO	,11,BAND_NO		,No_Guang	, _T("������") },	//���� ��
	{ 43,	OneP,ANIMAL_NO	,11,BAND_NO 	,No_Guang	, _T("������") },	//���� ��
	{ 44,	NoP	,ANIMAL_NO	,12,BAND_NO		,HAVE_Guang , _T("��") },	//��
	{ 45,	YEOL,ANIMAL_NO	,12,BAND_NO		,No_Guang	, _T("���") },	//���	���� ��ħ
	{ 46,	NoP	,ANIMAL_NO	,12,BAND_GLASS 	,No_Guang	, _T("���ʴ�") },	//���ʴ� 
	{ 47,	TwoP,ANIMAL_NO	,12,BAND_NO 	,No_Guang	, _T("�����") },	//�����
	{ 48,	TwoP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("���ʽ�2") },
	{ 49,	ThreeP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("���ʽ�3") },
	{ 50,	ThreeP,ANIMAL_NO,13,BAND_NO		,No_Guang	, _T("���ʽ�3") },
	{ 51,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("��ź") },
	{ 52,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("GO") },
	{ 53,	NoP,ANIMAL_NO	,31,BAND_NO 	,No_Guang	, _T("STOP")},
};


void CDiler::DoMixCard()
{
	TRACE("������ ī�带 ���� �ֽ��ϴ�.\r\n");
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

CCard* CDiler::PopCard()	// ���� ����
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
	// 10�� �̾Ƽ� ����
	CCard *pCard;
	TRACE("[Diler]\r\n");
	for (int i = 0; i < 8; i++)	// �ٴ� 8
	{
		pCard = PopCard();

		Push_OnFloor(pCard);
		//	pCard->PrintCard;
	}
	//TRACE("[Player1]");
	for (int i = 0; i < 10; i++) // 1�� ����� 5
	{
		pCard = PopCard();


		mpPlayer1->Push_InHand(pCard);
		//	pCard->PrintCard;
	}
	//TRACE("[Player2]");
	for (int i = 0; i < 10; i++) // 2�� ����� 5
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

	TRACE(_T("����"));
	dwWaitResult = WaitForSingleObject(
		ghSemaphore_Wait_UI_Select,  // handle to semaphore
		10000000UL);      // zero-second time-out interval

	switch (dwWaitResult)
	{
		// The semaphore object was signaled.
	case WAIT_OBJECT_0:
		//���õ� ���� ���� �˾Ƴ�����Ѵ�
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