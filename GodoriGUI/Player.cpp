#include "StdAfx.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include "Card.h"
#include "Player.h"
#include "Diler.h"

using namespace std;

extern HANDLE ghSemaphore_Wait_UI_Select;
extern queue<GUI_SELECT_t *> gSelctQueue;


CPlayer1::CPlayer1() :CPlayer(ID_PLAYER1)
{
	pTheOtherPlayer = &CPlayer2::GetInstance();
}

CPlayer2::CPlayer2() : CPlayer(ID_PLAYER2)
{
	pTheOtherPlayer = &CPlayer1::GetInstance();
}

// 폭탄이 있는지 검색 --> 폭탄마크, 조건해재시 폭탄 마크 제거
	// 총통인지 검색  --> 게임 종료
int CPlayer::CheckInHandsCards()
{
	int number_check[15];
	for (int i = 0; i < 13; i++)
		number_check[i] = 0;

	for_each(mInHand.Begin(), mInHand.End(), [&](CCard *a) 
	{
		a->bomb = 0;
		number_check[a->mCharic.mTheMonth]++;
	});

	for (int i = 0; i < 14; i++)
	{
		if (number_check[i] == 4)
		{
			int numofcard;
			mpDiler->GUI_DRAW(SELECT_CHONGTONG, GetType(), i*4, i * 4+1, i * 4+2, i * 4+3, 0, 0);
			numofcard = GUI_GETSELECT(SELECT_CHONGTONG, ID_PLAYER1);
			if (numofcard == 0) {
				//chong tong 으로 끝내기
			}
			else if(numofcard == 1)
			{
				//그냥치기
			}
			else
			{
				//다시
			}
		}
		else if (number_check[i] == 3)
		{
			for_each(mInHand.Begin(), mInHand.End(), [&](CCard *a)
			{
				if (a->mCharic.mTheMonth == i)
				{
					a->bomb = 3;
				}
			});
		}
	}
	return 0;
}


int CPlayer::DoGame()
{
	TRACE("- Do Game - 0x%p\r\n", this);

	if (!mpDiler->GetCount())
	{
		mpDiler->GUI_DRAW(SHOW_MAIN_MENU, GetType(), 0, 0, 0, 0, 0, 0);
		return -1;
	}

	mpDiler->GUI_DRAW(PLAYER_CHANCE, GetType(), 0, 0, 0, 0, 0, 0);

	// 폭탄이 있는지 검색 --> 폭탄마크, 조건해재시 폭탄 마크 제거
	// 총통인지 검색  --> 게임 종료
	CheckInHandsCards();
	CCardPorket MatchedCardList;
	int i = 0;
	pCard_1st = nullptr;
	pCard_1stMatched = nullptr;
	pCard_2nd = nullptr;
	pCard_2ndtMatched = nullptr;;

	int count;
	count = mInHand.GetCount();

	//1. 플레이어 손에서 한장을 꺼넨다
	//mpDiler->GUI_DRAW(PAYBACK_A_CARD, GetType(), 0, 0, 0, 0, 0, 0);
	pCard_1st = GetOnCardFromHand(pTheOtherPlayer->GetFloor(), mpDiler->GetFloor());
	if (pCard_1st == nullptr)
	{
		TRACE("가진패가 없네요..%d\r\n", count);
		//나가리 인지 체크
	}
	else
	{
		//폭탄 인경우 
		// 흘들지 여부 확인
		// 3장을 모두 바닥으로 보내고 , 2장의 폭탄카드를 받아와야 한다. 

		mpDiler->mOnFloor.Push(pCard_1st);
	}
	Sleep(300);

	//2.2 한장과 바닥에 있는 카드가 메칭이되는지 확인한다
	pCard_1stMatched = MatchingCardPre(pCard_1st, &MatchedCardList, mpDiler->GetFloor());
	//3. 센터에서 한장을 꺼네다
	pCard_2nd = mpDiler->PopCard();

	if (pCard_2nd == nullptr)
	{
		return 1;
 	}
	mpDiler->mOnFloor.Push(pCard_2nd);
	mpDiler->GUI_DRAW(OPEN_CENTER_CARD, GetType(), pCard_2nd->mCardID, 0, 0, 0, 0, 0);
	Sleep(600);


	//2.1 바닥에 설사가 있는지확인하고, 메칭이 되는지 확인하다.
	pCard_2ndtMatched =  MatchingCardPost(pCard_2nd, &MatchedCardList, mpDiler->GetFloor());
	Sleep(100);

	if (pCard_2ndtMatched)
	{
		mOnFloor.Push(pCard_2nd);
		mOnFloor.Push(pCard_2ndtMatched);
		if (pCard_1stMatched)
		{
			mOnFloor.Push(pCard_1st);
			mOnFloor.Push(pCard_1stMatched);
		}
		else
		{
			//이미 처리했거나 뻑이다. 

		}
		mpDiler->GUI_DRAW(GIVEMACHED_CARD, GetType(), 0, 0, 0, 0, 0, 0);
		//Sleep(500);
	}
	else
	{
		if (pCard_1stMatched)
		{
			mOnFloor.Push(pCard_1st);
			mOnFloor.Push(pCard_1stMatched);
		}
		else
		{
			//이미 처리했거나 뻑이다. 

		}
		mpDiler->GUI_DRAW(GIVEMACHED_CARD, GetType(), 0, 0, 0, 0, 0, 0);
		Sleep(500);
	}
	
	CString strMessage;
	
	int select;
	if (mOnFloor.GetCount() > 0)
	{
		mOnFloor.Lock();
		for_each(mOnFloor.Begin(), mOnFloor.End(), [&](CCard *a) {

			try
			{
				if (a->mCardID == 32)
				{
					if (GetType() == ID_PLAYER1)
					{
					retry:
						mpDiler->GUI_DRAW(SELECT_GOOKJIN, GetType(), pCard_2nd->mCardID, 0, 0, 0, 0, 0);
						select = GUI_GETSELECT(SELECT_GOOKJIN, ID_PLAYER1, a, nullptr);
						if (select > 1)
							goto retry;
						if (select == 0)
						{
							mPi.Push(a);	// 쌍피
						}
						else
						{
							mCharictor.Push(a);
						}
					}
					else
					{
						mPi.Push(a);	// 쌍피
					}
				}
				else
				{
					if (a->mCharic.mGuang == HAVE_Guang)
					{
						mPrime.Push(a);
					}
					else if ((a->mCharic.mTheBand) > BAND_NO)
					{
						mBand.Push(a);
					}
					else if ((a->mCharic.mPValue & YEOL) == YEOL)
					{
						mCharictor.Push(a);
					}
					else
					{
						mPi.Push(a);
					}
				}
			}
			catch(exception e)
			{
						
			}			
		});
		mOnFloor.UnLock();

		if (pCard_2ndtMatched)
		{
			mpDiler->mOnFloor.Remove(pCard_2nd);
			mpDiler->mOnFloor.Remove(pCard_2ndtMatched);
			if (pCard_1stMatched)
			{
				mpDiler->mOnFloor.Remove(pCard_1st);
				mpDiler->mOnFloor.Remove(pCard_1stMatched);
			}
			else
			{
				//이미 처리했거나 뻑이다. 

			}
		}
		else
		{
			if (pCard_1stMatched)
			{
				mpDiler->mOnFloor.Remove(pCard_1st);
				mpDiler->mOnFloor.Remove(pCard_1stMatched);
			}
			else
			{
				//이미 처리했거나 뻑이다. 

			}
			//이미 처리했거나 뻑이다. 
		}
		mOnFloor.Clear();
	}
	//바닥패가 없으면 쓸이라 부른다
	if (!mpDiler->mOnFloor.GetCount())
	{
		mpDiler->GUI_DRAW(MESSEGE_ASSA_FLOOR_CLAER, GetType(), pCard_1st->mCardID, 0, 0, 0, 0, 0);
		//상대방 패를 받아와야 한다. 
	}

	
	mpDiler->GUI_DRAW(DRAW_SELECT_BAR, GetType(), 0, 0, 0, 0, 0, 0);
	CalculatePoint();

	// 게임 종료 확인 
	GoOrStop();
	
	//TRACE("%d\r\n", NumOfRemainCard);
	return 1;
}


CCard *CPlayer::MatchingCardPost(CCard * pCard, CCardPorket *MatchedCardList, CCardPorket *DillersFloor)
{
	CString StrMessage;
	CString StrTemp;

	CCard *pCardArray[10];

	MatchedCardList->Clear();
	auto & Diller = CDiler::GetInstance(ID_DILLER);
	for (unsigned int i = 0; i < DillersFloor->GetCount(); i++)
	{
		if (pCard->mCharic.mTheMonth == DillersFloor->GetItem(i)->mCharic.mTheMonth)
		{
			if (pCard_1st == nullptr)
				return nullptr;
			if ((pCard->mCardID != DillersFloor->GetItem(i)->mCardID) &&
				(pCard_1st->mCardID != DillersFloor->GetItem(i)->mCardID))
			{
				if (pCard_1stMatched)
				{
					if(pCard_1stMatched->mCardID != DillersFloor->GetItem(i)->mCardID)
						MatchedCardList->Push(DillersFloor->GetItem(i));
				}
				else
				{
					MatchedCardList->Push(DillersFloor->GetItem(i));
				}
			}
		}
	}
	// 바닥에 동일한 달의 카드가 있는지 확인
	int count = MatchedCardList->GetCount();
	int index = 0;
	int select;
	
	if (pCard_1stMatched)
	{
		if ((pCard->mCharic.mTheMonth == pCard_1st->mCharic.mTheMonth) &&
			(pCard_1stMatched->mCharic.mTheMonth == pCard->mCharic.mTheMonth))
		{
			mpDiler->GUI_DRAW(SELECT_BUCK, GetType(), pCard_1stMatched->mCardID, 0, 0, 0, 0, 0);
			pCard_1stMatched = nullptr;
			pCard_1st = nullptr;
			pCard_2nd = nullptr;
			Sleep(100);

			return nullptr; // 2nd== pCard_2ndMatched 인경우 뻑으로 간주
		}
	}

	switch (count)
	{
	case 0://  매칭되는 것이 없는 경우 딜러가 그 카드를 가진다
		TRACE("No Mached Card\r\n");
		if ((pCard->mCharic.mTheMonth == pCard_1st->mCharic.mTheMonth))
		{
			if (!pCard_1stMatched)
			{
				mpDiler->GUI_DRAW(MESSEGE_ASSA_ZZOCK, GetType(), pCard->mCardID, 0, 0, 0, 0, 0);
				mOnFloor.Push(pCard);
				mOnFloor.Push(pCard_1st);
				mpDiler->mOnFloor.Remove(pCard);
				mpDiler->mOnFloor.Remove(pCard_1st);
				return nullptr; // 2nd== pCard_2ndMatched 인경우 뻑으로 간주
			}
		}
		return nullptr;
		break;
	case 1://  한 카드와 매칭되는 경우 매칭되는 카드와 가지고 온카드를 MatchedCardList에 담는다.
		TRACE("One card Mached\r\n");
		{
			
				CCard *MatchedCard = *(MatchedCardList->Begin());
				return MatchedCard;
		}
		break;
	case 2:
	{
		// 뻑일수 있다. 
		if (pCard_1stMatched)
		{
			if ((pCard->mCharic.mTheMonth == pCard_1st->mCharic.mTheMonth) ||
				(pCard_1stMatched->mCharic.mTheMonth == pCard->mCharic.mTheMonth))
			{
				mpDiler->GUI_DRAW(MESSEGE_ASSA_ALL_IS_MINE, GetType(), pCard_1stMatched->mCardID, 0, 0, 0, 0, 0);
			
				return nullptr;
			}
		}
		else
		{
			if (pCard->mCharic.mTheMonth == pCard_1st->mCharic.mTheMonth)
			{
				mpDiler->GUI_DRAW(MESSEGE_ASSA_ALL_IS_MINE, GetType(), pCard_1stMatched->mCardID, 0, 0, 0, 0, 0);
	
				return nullptr;
			}
		}
		index = 0;
		if (GetType() == ID_PLAYER1)
		{
			for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
				StrTemp.Format(_T("%s,"), a->mCharic.mName);
				pCardArray[index++] = a;
				StrMessage += StrTemp;
				return a;
			});
			if ((pCardArray[0]->mCharic.mPValue == pCardArray[1]->mCharic.mPValue) && (pCardArray[0]->mCharic.mTheBand == pCardArray[1]->mCharic.mTheBand))
				return pCardArray[0];
		retry:
			select = GUI_GETSELECT(SELECT_TO_OF_ONE, ID_PLAYER1, pCardArray[0], pCardArray[1]);
			if (select > 1)
				goto retry;
			// 우선 첫 카드를 받는 것으로 가정
			CCard *MatchedCard = pCardArray[select];
			return MatchedCard;
		}
		else
		{  // 컴퓨터는 첫번째거
			for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
				StrTemp.Format(_T("%s,"), a->mCharic.mName);
				pCardArray[index++] = a;
				StrMessage += StrTemp;
				return a;
			});
			return pCardArray[0];
		}
		break;
	}
	case 3://  3개와 매칭되는 경우 : 이미 싼 카드가 있으므로 다가져 간다.
	{
		//AfxMessageBox(_T("아싸 다내꺼\r\n"));
		mpDiler->GUI_DRAW(MESSEGE_ASSA_ALL_IS_MINE, GetType(), pCard_1st->mCardID, 0, 0, 0, 0, 0);
			//mOnFloor.Push(pCard);
		int num = MatchedCardList->GetCount();

		for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
			mOnFloor.Push(a);
			DillersFloor->Remove(a);
			return a;
		});
		mOnFloor.Push(pCard);
		MatchedCardList->Clear();
		break;
	}
	default:
		TRACE("Ooops!, Don do it..\r\n");
	};
	return nullptr;
}

CCard* CPlayer::MatchingCardPre(CCard * pCard, CCardPorket *MatchedCardList, CCardPorket *DillersFloor)
{
	CString StrMessage;
	CString StrTemp; 
	if (pCard == nullptr)
		return nullptr;
	CCard *pCardArray[10];
	MatchedCardList->Clear();
	MatchedCardList->Lock();
	auto & Diller = CDiler::GetInstance(ID_DILLER);
	for (unsigned int i = 0; i < DillersFloor->GetCount(); i++)
	{
		if (pCard->mCharic.mTheMonth == DillersFloor->GetItem(i)->mCharic.mTheMonth)
		{
			if (pCard->mCardID != DillersFloor->GetItem(i)->mCardID)
			{
				MatchedCardList->UnLock();
				MatchedCardList->Push(DillersFloor->GetItem(i));
				MatchedCardList->Lock();
			}
		}
	}
	MatchedCardList->UnLock();
	// 바닥에 동일한 달의 카드가 있는지 확인
	int count = MatchedCardList->GetCount();
	int index = 0; 
	int select;
	switch (count)
	{
	case 0://  매칭되는 것이 없는 경우 딜러가 그 카드를 가진다
		TRACE("No Mached Card\r\n");
		return nullptr;
		break;
	case 1://  한 카드와 매칭되는 경우 매칭되는 카드와 가지고 온카드를 MatchedCardList에 담는다.
		TRACE("One card Mached\r\n");
		{
			CCard *MatchedCard = *(MatchedCardList->Begin());
			return MatchedCard;
		}
		break;
	case 2:
	{
		if (GetType() == ID_PLAYER1)
		{
			MatchedCardList->Lock();
			index = 0;
			for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
				StrTemp.Format(_T("%s,"), a->mCharic.mName);
				pCardArray[index++] = a;
				StrMessage += StrTemp;
				return a;
			});
			MatchedCardList->UnLock();
		retry:
			select = GUI_GETSELECT(SELECT_TO_OF_ONE, ID_PLAYER1, pCardArray[0], pCardArray[1]);
			if (select > 1)
				goto retry;
			// 우선 첫 카드를 받는 것으로 가정
			CCard *MatchedCard = pCardArray[select];
			return MatchedCard;
		}
		else
		{  // 컴퓨터는 첫번째거
			MatchedCardList->Lock();
			for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
				StrTemp.Format(_T("%s,"), a->mCharic.mName);
				pCardArray[index++] = a;
				StrMessage += StrTemp;
				return a;
			});
			MatchedCardList->UnLock();
			return pCardArray[0];
		}
		break;
	}
	case 3://  3개와 매칭되는 경우 : 이미 싼 카드가 있으므로 다가져 간다.
	{
		//StrMessage = _T("아싸 다 내꺼\n");
		mpDiler->GUI_DRAW(MESSEGE_ASSA_ALL_IS_MINE, GetType(), pCard_1st->mCardID, 0, 0, 0, 0, 0);
		int num = MatchedCardList->GetCount();
		MatchedCardList->Lock();
		for_each(MatchedCardList->Begin(), MatchedCardList->End(), [&](CCard *a) {
			mOnFloor.Push(a);
			DillersFloor->Remove(a);
			return a;
		});
		MatchedCardList->UnLock();
		mOnFloor.Push(pCard);
		DillersFloor->RemoveIf(pCard);
		break;
	}
	default:
		TRACE("Ooops!, Don do it..\r\n");
		return nullptr;
	};
	return nullptr;
}

// Select From Select Bar
int CPlayer::GUI_GETSELECT(int gui, int attack) {
	int index = 0;

	//Send Select Message
	DWORD dwWaitResult;
	GUI_SELECT_t *pSelect;
ReTry:

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
				if (index + 1 > mInHand.GetCount())
					goto ReTry;
			}
		}
		break;

		// The semaphore was nonsignaled, so a time-out occurred.
	case WAIT_TIMEOUT:
		TRACE("Diller(%d): wait timed out\n", GetCurrentThreadId());
		break;
	}
	//Wait Semaphore
	return index;
}

// Select From Two card in Select window , 국진 Case
int CPlayer::GUI_GETSELECT(int gui, int attack, CCard *pLeft, CCard *pRight)
{
	int index = 0;

	//Send Select Message
	DWORD dwWaitResult;
	GUI_SELECT_t *pSelect; 

	auto & Diller = CDiler::GetInstance(ID_DILLER);
ReTry:
	if (pRight)
	{
		Diller.GUI_DRAW(gui, ID_PLAYER1, pLeft->mCardID, pRight->mCardID, 0, 0, 0, 0);
	}
	else
	{
		Diller.GUI_DRAW(gui, ID_PLAYER1, pLeft->mCardID, pLeft->mCardID, 0, 0, 0, 0);
	}

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
				if (index > 1)
					goto ReTry;
			}
		}
		Diller.GUI_DRAW(SELECT_WINDOW_HIDE, ID_PLAYER1, 0, 0, 0, 0, 0, 0);
		break;

		// The semaphore was nonsignaled, so a time-out occurred.
	case WAIT_TIMEOUT:
		TRACE("Diller(%d): wait timed out\n", GetCurrentThreadId());
		break;
	}

	//Wait Semaphore

	return index;
}
 

// Select 총통 / 뻑 / in Select window
int CPlayer::GUI_GETSELECT(int gui, int attack, list<CCard *> &CardList) {
	int index = 0;
	int CardIDList[5];
	//Send Select Message
	DWORD dwWaitResult;
	GUI_SELECT_t *pSelect;

	auto & Diller = CDiler::GetInstance(ID_DILLER);

	int i = 0;
	for_each(CardList.begin(), CardList.end(), [&](CCard *a) {return CardIDList[i++] = a->mCardID; });

	if (CardList.size() == 3) // 뻑, 
	{
		Diller.GUI_DRAW(SELECT_BUCK, ID_PLAYER1, CardIDList[0], CardIDList[1], CardIDList[2],0,0,0);
	}
	else if(CardList.size() == 4) // 총통
	{
		Diller.GUI_DRAW(SELECT_CHONGTONG, ID_PLAYER1, CardIDList[0], CardIDList[1], CardIDList[2], CardIDList[3], 0, 0);
	}

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


CCard *CPlayer::Push_InHand(CCard *pCard)
{
	mInHand.Push(pCard);
	return nullptr;
}

CCard *CPlayer::Push_OnFloor(CCard *pCard)
{
	mOnFloor.Push(pCard);
	return nullptr;
}

CCard *CPlayer::GetOnCardFromHand(CCardPorket *pOppositCardList, CCardPorket *pFloorCard)
{
	auto & Diller = CDiler::GetInstance(ID_DILLER);
	CCard *pRetCard=nullptr;
	CCardPorket MatchedCardList;
	CCardPorket UnMatchedCardList;
	int indx;
	int nCountInHand = mInHand.GetCount();
	int nCountFloor = pFloorCard->GetCount();
	bool found = false;
	CCard *pHandCard;
	CCard *pCard;
	int count_found = 0;
retry:
	for (int j = 0; j < nCountInHand; j++)
	{
		found = false;
		pHandCard = mInHand.GetItem(j);
		pHandCard->Value = 0;
		for (int i = 0; i <  nCountFloor; i++)
		{

			pCard = pFloorCard->GetItem(i);
			if (pCard)
			{
				if (pHandCard->mCharic.mTheMonth == pCard->mCharic.mTheMonth)
				{
					MatchedCardList.Push(pHandCard);
					pHandCard->Value = GetType();
					found = true;
					count_found++;
					break;
				}
			}
			else
			{
				return nullptr;
			}
		}		
		if (!found)
		{
			UnMatchedCardList.Push(pHandCard);
		}
	}
	
	if (GetType() == ID_PLAYER1)	// User --> Manual Select  
	{
		// 바닥에 동일한 달의 카드가 있는지 확인
		int count = MatchedCardList.GetCount();
		int count_inhand = mInHand.GetCount();
		if (!count_inhand)
		{
			return nullptr;
		}

		switch (count)
		{
		case 0://  매칭되는 것이 없는 경우 아무패라도 내어야 한다
			MatchedCardList = mInHand;
			pRetCard = SelectOneCard(MatchedCardList, UnMatchedCardList, indx);
			break;
		default:
			pRetCard = SelectOneCard(MatchedCardList, UnMatchedCardList, indx);
			//pRetCard = MatchedCardList.GetItem(0)->pCard;
			break;
		};

		if (pRetCard)
			mpDiler->GUI_DRAW(PAYBACK_A_CARD, GetType(), pRetCard->mCardID, 0, 0, 0, 0, indx);
		else
			return nullptr;

	}
	else if(GetType() == ID_PLAYER2) //Computre --> Random Select
	{
		//TRACE("Auto\r\n");
		// 바닥에 동일한 달의 카드가 있는지 확인
		int count = MatchedCardList.GetCount();
		int count_inhand = mInHand.GetCount();
		if (!count_inhand)
		{
			TRACE("들고있는 카드가 없습니다\r\n");
			return nullptr;
		}
		switch (count)
		{
		case 0://  매칭되는 것이 없는 경우 아무패라도 내어야 한다
			indx = rand() % count_inhand;
			pRetCard = mInHand.GetItem(indx);
			break;
		default:
			indx = rand() % MatchedCardList.GetCount();
			pRetCard = MatchedCardList.GetItem(indx);
			break;
		};
		mpDiler->GUI_DRAW(PAYBACK_A_CARD, GetType(), pRetCard->mCardID, 0, 0, 0, 0, indx);

	}
	if (!pRetCard)
		goto retry;
	else
	{
		mInHand.Remove(pRetCard);	
	}
	
	CalculatePoint();

	return pRetCard;
}

CCard * CPlayer::SelectOneCard(CCardPorket & MatchedCardList, CCardPorket & UnMatchedCardList, int & index)
{	
retry:
	int numofcard = 0;
	numofcard = GUI_GETSELECT(SELECT_MAIN_MENU, ID_PLAYER1);
	

	if (numofcard < mInHand.GetCount())
	{
		if (mInHand.GetItem(numofcard))
		{
			index = numofcard;
			return mInHand.GetItem(numofcard);
		}
		else
		{
			goto retry;
		}
	}
	index = -1;
	return nullptr;
}

CCard *CPlayer::GiveCardsToFloor(CCardPorket *pMatchedCardList)
{
	return nullptr;
}

extern CCard staticCardList[54];
bool  CPlayer::GoOrStop()
{
	int YesOrNo;
	if (GetType() == ID_PLAYER1)
	{
		if (mScore.SemiFinalPoint > 6)
		{
			mpDiler->GUI_DRAW(SELECT_GO_OR_STOP, GetType(), mScore.FinalPoint, mScore.SemiFinalPoint, 0, 0, 0, 0);
		retry:
			YesOrNo = GUI_GETSELECT(SELECT_GO_OR_STOP, ID_PLAYER1, &staticCardList[52], &staticCardList[53]);
		
				
			if (YesOrNo < 3)
			{
				if (YesOrNo > 0)
				{
					mpDiler->GUI_DRAW(SELECT_WINDOW_HIDE, ID_PLAYER1, 0, 0, 0, 0, 0, 0);
					if (YesOrNo==1)
					{
						mStatus.NumOfGo++;
						mpDiler->GUI_DRAW(MESSEGE_GO, GetType(), mScore.FinalPoint, mScore.SemiFinalPoint, 0, 0, 0, 0);
						return true;
					}
					else
					{
						mpDiler->GUI_DRAW(MESSEGE_STOP, GetType(), mScore.FinalPoint, mScore.SemiFinalPoint, 0, 0, 0, 0);

						return false;
					}
				}
				else
				{
					goto retry;
				}
			}
			return nullptr;
		}
	}
	else
	{
		if (mScore.SemiFinalPoint > 6)
		{
			mStatus.NumOfGo++;
			mpDiler->GUI_DRAW(MESSEGE_GO, GetType(), mScore.FinalPoint, mScore.SemiFinalPoint, 0, 0, 0, 0);
		}
		return true;
	}
}


int CPlayer::CalculatePoint()
{

	mStatic.NumOfGuang = 0;
	mStatic.NumOfBiGuang = 0;	//비광이 포함된 여부
	mStatic.NumOfP = 0;
	mStatic.NumOfBand = 0;
	mStatic.NumOfYeol = 0;
	mStatic.NumOfBird = 0;
	mStatic.NumOfHong = 0;;
	mStatic.NumOfChung = 0;;
	mStatic.NumOfCho = 0;

	mScore.FinalPoint = 0;
	mScore.SemiFinalPoint = 0;
	mScore.PointOfGuang = 0;
	mScore.PointOfP = 0;
	mScore.PointOfGodori = 0;	//Godory = 5점
	mScore.PointOfYoel = 0;
	mScore.PointOfBand = 0;
	mScore.PointHongDan = 0;
	mScore.PointChoDan = 0;
	mScore.PointChungDan = 0;


	//////////////////////////////////////////////////////////////////////////////////////////
	//광 3개 부터 3점 ,1장 증가에  +1점, 비광이 있으면 -1
	list<CCard *>::iterator iter = mPrime.Begin();
	while (iter != mPrime.End())
	{
		mStatic.NumOfGuang += (*iter)->mCharic.mGuang;
		if ((*iter)->mCardID == 44)
			mStatic.NumOfBiGuang = 1;
		iter++;
	}
	
	if (mStatic.NumOfGuang == 3)
		mScore.PointOfGuang = (mStatic.NumOfGuang - 2) * 3 - mStatic.NumOfBiGuang;
	else if (mStatic.NumOfGuang == 4)
		mScore.PointOfGuang = 4;	//4광은 비와 관계  없이 4점
	else if (mStatic.NumOfGuang == 5)
		mScore.PointOfGuang = 15;	//5광은 15점
	else
		mScore.PointOfGuang = 0;



	//////////////////////////////////////////////////////////////////////////////////////////
	// 동물 계산 (새의 갯수)
	list<CCard *>::iterator iter1 = mCharictor.Begin();
	while (iter1 != mCharictor.End())
	{		
		mStatic.NumOfBird += (*iter1)->mCharic.mAnimalCharic;
		iter1++;
	}
	mStatic.NumOfBird = mStatic.NumOfBird >> 16;
	if(mStatic.NumOfBird==5) //고도리
		mScore.PointOfGodori = 5;
	   
	//////////////////////////////////////////////////////////////////////////////////////////
	// 띠 계산  3개씩 모이면 3점	//띠 5개 이상일 경우 1점씩 추가된다
	int band = 0;
	list<CCard *>::iterator iter2 = mBand.Begin();
	while (iter2 != mBand.End())
	{
		band += (*iter2)->mCharic.mTheBand;
		iter2++;
	}
	mStatic.NumOfHong = NUMOF_BAND_RED(band);
	mStatic.NumOfChung = NUMOF_BAND_BLUE(band);
	mStatic.NumOfCho = NUMOF_BAND_GLASS(band);
	mStatic.NumOfBand = mStatic.NumOfHong + mStatic.NumOfChung + mStatic.NumOfCho;
	if (mStatic.NumOfHong == 3)		mScore.PointHongDan = 3;
	if (mStatic.NumOfChung== 3)		mScore.PointChungDan = 3;
	if (mStatic.NumOfCho  == 3)		mScore.PointChoDan = 3;
	mScore.PointOfBand = mScore.PointChoDan + mScore.PointChungDan + mScore.PointHongDan;
	if (mStatic.NumOfBand > 4)	//띠 5개 이상일 경우 1점씩 추가된다
		mScore.PointOfBand += (mStatic.NumOfBand - 4);

	
	


	//////////////////////////////////////////////////////////////////////////////////////////
	// 피 계산  

	list<CCard *>::iterator iter3 = mPi.Begin();
	while (iter3 != mPi.End())
	{
		mStatic.NumOfP += (*iter3)->mCharic.mPValue;
		iter3++;
	}
	
	mStatic.NumOfP = mStatic.NumOfP & 0xFFFF;
	// 10장 1점, 1점씩
	if ((mStatic.NumOfP & 0xFFFF) > mGameRole.ThPointOfP)
	{
		mScore.PointOfP = (mStatic.NumOfP & 0xFFFF) - mGameRole.ThPointOfP;
	}
	else
		mScore.PointOfP = 0;

	//////////////////////////////////////////////////////////////////////////////////////////
	// 열 점수  // 5개 부터 1점
	list<CCard *>::iterator iter4 = mCharictor.Begin();
	while (iter4 != mCharictor.End())
	{
		mStatic.NumOfYeol += (*iter4)->mCharic.mPValue;
		iter4++;
	}


	mStatic.NumOfYeol = mStatic.NumOfYeol >> 16;
	if (mStatic.NumOfYeol  > 4)
	{
		mScore.PointOfYoel = mStatic.NumOfYeol - 4;
	}
	if (mStatic.NumOfYeol > 6)
		mStatus.Mungtungguri = 1;


	mScore.SemiFinalPoint += mScore.PointOfGuang;
	mScore.SemiFinalPoint += mScore.PointOfP;
	mScore.SemiFinalPoint += mScore.PointOfGodori;	//Godory = 5점
	mScore.SemiFinalPoint += mScore.PointOfBand;
	mScore.SemiFinalPoint += mScore.PointOfYoel;
	mScore.FinalPoint = mScore.SemiFinalPoint;

	if (mStatus.NumOfBuck > 2)
	{
		//3뻑이면 게임 종료할수 있다
	}


	//고를 부른 횟수 1G +1점, 2Go + 2점  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
	if (mStatus.NumOfGo > 0)
	{
		mScore.FinalPoint + mStatus.NumOfGo;
		if (mStatus.NumOfGo == 3)
		{
			mScore.FinalPoint *=2;
		}
		else if(mStatus.NumOfGo == 4)
		{ 
			mScore.FinalPoint *= 4;
		}
		else if (mStatus.NumOfGo == 5)
		{
			mScore.FinalPoint *= 8;

		}
		else if (mStatus.NumOfGo == 6)
		{
			mScore.FinalPoint *= 16;
		}
	}	

	if (mStatus.Shake) //흔들기 2배
		mScore.FinalPoint * 2;

	if (mStatus.Mungtungguri) //흔들기 2배
		mScore.FinalPoint * 2;
	

	//피박 - 상대의 피가 5개 이하일 때
	//광박 - 광의 점수가 있을때 상대방이 광이 없다면
	//고박 - 고를 불렀으나 졌을떄
	//흔들기 - 흔들기 선언후 
	//
	//Card List 합계	
	return mScore.FinalPoint;
}