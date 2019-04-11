#ifndef __GAME__H_
#define __GAME__H_

#include "stdafx.h"
#include <WinUser.h>
#include "Card.h"
#include "CardPorket.h"
#include "Player.h"
#include "Diler.h"

#define GUI_MESSAGE (WM_USER+1) 

typedef enum GUI_DRAW {
	PLAYER_CHANCE,		//사용자 Turn을 표시
	PAYBACK_A_CARD,		//첫장을 내려놓음
	OPEN_CENTER_CARD,	//가운데 쌓인패에서 한장을 가져옴
	GIVEMACHED_CARD,	//매칭된 패를 가져옴
	DRAW_SELECT_BAR,	//사용자가 선택화면 다시 그림
	DRAW_GROUND,		//바닥패 다시그림
	GIVE_CARD,			//딜러가 패를 플레이어에게 나눠 줄때
	MIXED_CARD,			//바닥패 썩음
	SHOW_MAIN_MENU,		//메인화면으로 돌아감
	SELECT_MAIN_MENU,	//메인화면에서 선택을 기다림.
	HIDE_MAIN_MENU,		//메인화면 숨김
	SELECT_GO_OR_STOP,
	SELECT_CHONGTONG,
	SELECT_BUCK,
	SELECT_TO_OF_ONE,
	SELECT_WAIT_OK,
	SELECT_WINDOW_HIDE, //선택 윈도우 숨기기
	SELECT_GOOKJIN,
	SELECT_WIN,
	SELECT_LOST,
	MESSEGE_ASSA_ALL_IS_MINE,
	MESSEGE_ASSA_ZZOCK,
	MESSEGE_ASSA_GODORI,
	MESSEGE_ASSA_CHUNGDAN,
	MESSEGE_ASSA_HONGDAN,
	MESSEGE_ASSA_CHODAN,
	MESSEGE_ASSA_FLOOR_CLAER,
	MESSEGE_GO, 
	MESSEGE_STOP,
}GUIDraw_t;

typedef enum GoOrStop{
	OK_GO,
	NO_STOP,
}GoOrStop_t;




#endif