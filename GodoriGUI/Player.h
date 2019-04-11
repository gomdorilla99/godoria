#pragma once

#include "CardPorket.h"
#include "Singleton.h"
#include "Diler.h"


class CPlayer : public Singleton<CPlayer>
{
	friend class Singleton<CPlayer>;
private:
	struct{
		int ThPointOfGuang;
		int ThPointOfP;
		int ThGodori; // 새 5마리 이상
		int ThTotalBand;
		int ThBand;
	} mGameRole;

	CCard *pCard_1st;
	CCard *pCard_1stMatched ;
	CCard *pCard_2nd;
	CCard *pCard_2ndtMatched;

public:
	int CalculatePoint();
	CPlayer *pTheOtherPlayer;
	CDiler  *mpDiler;
	
	CCardPorket mInHand;	//손에 든카드
	CCardPorket mOnFloor;	//바닥에 카드
	CCardPorket mPrime;		//Guang
	CCardPorket mCharictor;	//열
	CCardPorket mBand;		//띠
	CCardPorket mPi;		//피

	CCardPorket * GetHand() {
		return &mInHand;
	}

	CCardPorket * GetFloor() {
		return &mOnFloor;
	}

	struct {
		int NumOfGuang;
		int NumOfBiGuang;	//비광이 포함된 여부
		int NumOfP;
		int NumOfYeol;
		int NumOfBird;
		int NumOfBand;
		int NumOfHong;
		int NumOfChung;
		int NumOfCho;

	} mStatic;

	struct {
		//7점 이상에서 Go or Stop을 정할수 있음
		int NumOfGo;		//고를 부른 횟수 1G +1점, 2Go + 2점  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		int NumOfBuck;
		int ChungDan;
		int HongDan;		//Godory = 5점
		int ChoDan;
		int Shake;
		int P_Bak;			// P가 1~5개일때 
		int Guang_Bak;		//광이 하나도 없을경우
		int Mungtungguri;	// 열끝 7장 이상일 경우 총점수 x2
	} mStatus;


	struct {
		int FinalPoint;		// 흔들기등 배수 고려 계산
		int SemiFinalPoint; // 순수 점수만 계산
		int Godori;			// 2, 4, 8 의 열끝을 모두 모은 경우 5점 획득
		int PointOfGuang;
		int PointOfP;
		int PointOfGodori;	//Godory = 5점
		int PointOfYoel;	//5개 이상에서 1점씩 누적(5개 1점, 6개 2점)
		int PointOfBand;	//홍단 3점, 청단 3점, 초단 3점 의 누계 점수
		int PointHongDan;   //홍단 3점,
		int PointChoDan;	//초단 3점,
		int PointChungDan;	//청단 3점
		
	} mScore;

	CCard *GetOnCardFromHand(CCardPorket *pOppositCardList, CCardPorket *pFloorCard);
	CCard *SelectOneCard(CCardPorket & MatchedCardList, CCardPorket & UnMatchedCardList, int &index);
	CCard *GiveCardsToFloor(CCardPorket *pMatchedCardList);
	int CheckInHandsCards();
	CCard *MatchingCardPost(CCard * pCard, CCardPorket *MatchedCardList, CCardPorket *DillersFloor);
	CCard *MatchingCardPre(CCard * pCard, CCardPorket * MatchedCardList, CCardPorket * DillersFloor);
	int GUI_GETSELECT(int gui, int attack);
	int GUI_GETSELECT(int gui, int attack, CCard * pLeft, CCard * pRight);
	int GUI_GETSELECT(int gui, int attack, list<CCard*>& CardList);
	bool  GoOrStop();
	CPlayer()
	{ 
		mGameRole.ThPointOfGuang = 2;
		mGameRole.ThPointOfP = 9;
		mGameRole.ThGodori = 5; // 새 5마리 이상
		mGameRole.ThTotalBand = 4;
		mGameRole.ThBand = 3;		// 3개 이상

		mScore.FinalPoint = 0;
		mScore.PointOfGuang = 0;
		mScore.PointOfP = 0;
		mScore.PointOfGodori = 0;	//Godory = 5점
		mScore.PointOfYoel = 0;
		mScore.PointOfBand = 0;
		
		mStatus.NumOfGo = 0;		//고를 부른 횟수 1G +1점, 2Go + 2점  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		mStatus.NumOfBuck = 0;
		mStatus.ChungDan= 0;
		mStatus.HongDan = 0;		//Godory = 5점
		mStatus.ChoDan = 0;
		mStatus.Shake = 0;
		mStatus.P_Bak = 1;			// P가 1~5개일때 
		mStatus.Guang_Bak = 1;		//광이 하나도 없을경우
		mStatus.Mungtungguri = 0;	// 동물 7개 이상일 경우 ,총점수 x2 
		mpDiler = (CDiler *)&(CDiler::GetInstance(ID_DILLER));
	};
	int DoGame();

	void GetPoint()
	{
		printf("::::::%p::::\r\n", this);
	};
	

	CCard *Push_InHand(CCard *pCard);
	CCard *Push_OnFloor(CCard *pCard);
	CPlayer(int Type):Singleton<CPlayer>(Type ){ ; };
};

class CPlayer2;

class CPlayer1 final : public CPlayer
{
public :
	CPlayer1();
	static CPlayer1 &GetInstance()
	{
		return	Singleton<CPlayer1>::GetInstance(ID_PLAYER1);
	}

};

class CPlayer2 final : public CPlayer
{
public:
	CPlayer2();
	static CPlayer2 &GetInstance()
	{
		return Singleton<CPlayer2>::GetInstance(ID_PLAYER2);
	}
};
