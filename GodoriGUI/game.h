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
	PLAYER_CHANCE,		//����� Turn�� ǥ��
	PAYBACK_A_CARD,		//ù���� ��������
	OPEN_CENTER_CARD,	//��� �����п��� ������ ������
	GIVEMACHED_CARD,	//��Ī�� �и� ������
	DRAW_SELECT_BAR,	//����ڰ� ����ȭ�� �ٽ� �׸�
	DRAW_GROUND,		//�ٴ��� �ٽñ׸�
	GIVE_CARD,			//������ �и� �÷��̾�� ���� �ٶ�
	MIXED_CARD,			//�ٴ��� ����
	SHOW_MAIN_MENU,		//����ȭ������ ���ư�
	SELECT_MAIN_MENU,	//����ȭ�鿡�� ������ ��ٸ�.
	HIDE_MAIN_MENU,		//����ȭ�� ����
	SELECT_GO_OR_STOP,
	SELECT_CHONGTONG,
	SELECT_BUCK,
	SELECT_TO_OF_ONE,
	SELECT_WAIT_OK,
	SELECT_WINDOW_HIDE, //���� ������ �����
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