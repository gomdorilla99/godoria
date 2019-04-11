#pragma once

enum PValue
{
	NoP = 0,	//피가 없는 카드
	OneP,		//1피
	TwoP,		//쌍피
	ThreeP,		//3피 (보너스피)
	YEOL = 0x00010000
};

enum GuangValue
{
	No_Guang = 0,	//Guang 이 안닌 카드 Guang
	HAVE_Guang,			//Guang 인카드
};


#define ANIMAL_BRID		 0x00010000
#define ANIMAL_TREE_BRID 0x00110000
#define ANIMAL_OTHER	 0x00000001
#define ANIMAL_NO		 0x00000000 

//청단
#define BAND_BLUE	0x00000001
#define NUMOF_BAND_BLUE(x) (x & 0x000003)		
//홍단
#define BAND_RED	0x00000010
#define NUMOF_BAND_RED(x) ((x>>4) & 0x000003)		

//초단
#define BAND_GLASS  0x00000100
#define BAND_NO		0x00000000
#define NUMOF_BAND_GLASS(x) ((x>>8) & 0x000007)		

#define COUNT_GuangThreshold 3
#define COUNT_PThreshold	11
#define COUNT_Godori		5

struct control_t
{
	control_t() :prev_err(0), I_err(0) { ; };
	double I_err;
	double prev_err;  // 오차. 이전 오차 
	double current_val;    // 현재값 
	double CurMeasureData;
	double control;
};



#define Kp  0.8
#define Ki  0.4
#define Kd  0.01
#define dt  0.33


class CMessege_t
{
public:
	control_t cont_x;
	control_t cont_y;
	int mRemainCount;
	CString mMessege;
};

//Guang 점수 경계
class CCard
{
public:
	CCard();
	CCard(int ID, int P, int Ani, int theMonth, int theBand, int Guang, wchar_t *Name);
	struct{
		int mPValue;
		int mAnimalCharic;
		int mTheMonth;
		int mTheBand;
		int mGuang;
		wchar_t mName[30];
	}mCharic;
	int mCardID;
	int Value;
	int bomb;
	int Category;
	struct {
		control_t cont_x;
		control_t cont_y;

		CPoint mPosition;
		CPoint mVelocity;
	}mPhysic;

};

