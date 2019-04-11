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
		int ThGodori; // �� 5���� �̻�
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
	
	CCardPorket mInHand;	//�տ� ��ī��
	CCardPorket mOnFloor;	//�ٴڿ� ī��
	CCardPorket mPrime;		//Guang
	CCardPorket mCharictor;	//��
	CCardPorket mBand;		//��
	CCardPorket mPi;		//��

	CCardPorket * GetHand() {
		return &mInHand;
	}

	CCardPorket * GetFloor() {
		return &mOnFloor;
	}

	struct {
		int NumOfGuang;
		int NumOfBiGuang;	//���� ���Ե� ����
		int NumOfP;
		int NumOfYeol;
		int NumOfBird;
		int NumOfBand;
		int NumOfHong;
		int NumOfChung;
		int NumOfCho;

	} mStatic;

	struct {
		//7�� �̻󿡼� Go or Stop�� ���Ҽ� ����
		int NumOfGo;		//�� �θ� Ƚ�� 1G +1��, 2Go + 2��  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		int NumOfBuck;
		int ChungDan;
		int HongDan;		//Godory = 5��
		int ChoDan;
		int Shake;
		int P_Bak;			// P�� 1~5���϶� 
		int Guang_Bak;		//���� �ϳ��� �������
		int Mungtungguri;	// ���� 7�� �̻��� ��� ������ x2
	} mStatus;


	struct {
		int FinalPoint;		// ����� ��� ��� ���
		int SemiFinalPoint; // ���� ������ ���
		int Godori;			// 2, 4, 8 �� ������ ��� ���� ��� 5�� ȹ��
		int PointOfGuang;
		int PointOfP;
		int PointOfGodori;	//Godory = 5��
		int PointOfYoel;	//5�� �̻󿡼� 1���� ����(5�� 1��, 6�� 2��)
		int PointOfBand;	//ȫ�� 3��, û�� 3��, �ʴ� 3�� �� ���� ����
		int PointHongDan;   //ȫ�� 3��,
		int PointChoDan;	//�ʴ� 3��,
		int PointChungDan;	//û�� 3��
		
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
		mGameRole.ThGodori = 5; // �� 5���� �̻�
		mGameRole.ThTotalBand = 4;
		mGameRole.ThBand = 3;		// 3�� �̻�

		mScore.FinalPoint = 0;
		mScore.PointOfGuang = 0;
		mScore.PointOfP = 0;
		mScore.PointOfGodori = 0;	//Godory = 5��
		mScore.PointOfYoel = 0;
		mScore.PointOfBand = 0;
		
		mStatus.NumOfGo = 0;		//�� �θ� Ƚ�� 1G +1��, 2Go + 2��  3Go +3 x 2  4Go +4 x 4, 5Go +5  x 8
		mStatus.NumOfBuck = 0;
		mStatus.ChungDan= 0;
		mStatus.HongDan = 0;		//Godory = 5��
		mStatus.ChoDan = 0;
		mStatus.Shake = 0;
		mStatus.P_Bak = 1;			// P�� 1~5���϶� 
		mStatus.Guang_Bak = 1;		//���� �ϳ��� �������
		mStatus.Mungtungguri = 0;	// ���� 7�� �̻��� ��� ,������ x2 
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
