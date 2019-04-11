// godoria.cpp : main project file.

#include "stdafx.h"
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <stdio.h>
#include "Diler.h"
#include "Player.h"
#include "Card.h"
#include "Singleton.h"

auto & Diller = CDiler::GetInstance(ID_PLAYER2);
auto & Player1 = CPlayer::GetInstance(ID_PLAYER1);
auto & Player2 = CPlayer::GetInstance(ID_PLAYER2);

void InitFrameworkmain();
void WaitFrameworkmain();

int main(int argc, char *argv)
{
	
    printf("자 고스톱을 쳐봅시다");

	InitFrameworkmain();
	// 임의로 썩기

	printf("Ready To Play..\r\n");
	WaitFrameworkmain();

	// 점수 계산
	getchar();
	return 0;
}

