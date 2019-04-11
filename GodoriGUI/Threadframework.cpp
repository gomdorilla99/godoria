#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "Player.h"
#include "Diler.h"
#include "GodoriGUIView.h"
#include <queue>

using namespace std;

#define MAX_SEM_COUNT 3
#define THREADCOUNT 3
#define TIMEROUT 1000000


HANDLE ghSemaphore_To_Diller;
HANDLE ghSemaphore_To_Player1;
HANDLE ghSemaphore_To_Player2;
HANDLE ghSemaphore_Wait_UI_Select;

queue<GUIMessage_t *> gGUIQueue;
queue<GUI_SELECT_t *> gSelctQueue;



DWORD WINAPI ThreadDillerProc(LPVOID);
DWORD WINAPI ThreadPlayer1Proc(LPVOID);
DWORD WINAPI ThreadPlayer2Proc(LPVOID);

static BOOL bContinue = TRUE;

DWORD WINAPI ThreadDillerProc(LPVOID lpParam)
{
	DWORD dwWaitResult;

	int Turn=0;
	auto & Diler = CDiler::GetInstance(ID_DILLER);
	Sleep(2000);
	Diler.DoGame();

	while (bContinue)
	{
		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject(
			ghSemaphore_To_Diller,  // handle to semaphore
			100000UL);      // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			// TODO: Perform task
		//	TRACE("Diller %d: Get Mesage\n", GetCurrentThreadId());
			// Simulate thread spending time on task
			// Release the semaphore when task is finished
			if(Turn==0)
			{ 
				if (!ReleaseSemaphore(
					ghSemaphore_To_Player1, // handle to semaphore
					1,      // increase count by one
					NULL))    // not interested in previous count
				{
					TRACE("ReleaseSemaphore error: %d\n", GetLastError());
				}
				Turn++;
			}
			else if(Turn ==1)
			{
				if (!ReleaseSemaphore(
					ghSemaphore_To_Player2, // handle to semaphore
					1,      // increase count by one
					NULL))    // not interested in previous count
				{
					TRACE("ReleaseSemaphore error: %d\n", GetLastError());
				}
				Turn++;
			}
			else
			{
				Turn = 0;
			}

			if (Turn > 1)
				Turn = 0;
			break;

			// The semaphore was nonsignaled, so a time-out occurred.
		case WAIT_TIMEOUT:
			TRACE("Diller(%d): wait timed out\n", GetCurrentThreadId());
			break;
		}
	}
	TRACE("DILER DONE");
	return TRUE;
}

DWORD WINAPI ThreadPlayer1Proc(LPVOID lpParam)
{
	DWORD dwWaitResult;
	
	auto & Player1 = CPlayer::GetInstance(ID_PLAYER1);
	//auto & Player2 = CPlayer::GetInstance(ID_PLAYER2);
	Player1.GetPoint();
	//Player2.GetPoint();

	while (bContinue)
	{
		dwWaitResult = WaitForSingleObject(
			ghSemaphore_To_Player1,  // handle to semaphore
			100000L);      // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			if (Player1.DoGame() < 0)
			{
				bContinue = 0;
			}
			if (!ReleaseSemaphore(ghSemaphore_To_Diller, 	1,	NULL))    // not interested in previous count
			{
				TRACE("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;
		case WAIT_TIMEOUT:
			TRACE("Player1(%d): wait timed out\n", GetCurrentThreadId());
			break;
		}
	}
	TRACE("PLAYER1 DONE");
	return TRUE;
}


DWORD WINAPI ThreadPlayer2Proc(LPVOID lpParam)
{
	DWORD dwWaitResult;
	auto & Player2 = CPlayer::GetInstance(ID_PLAYER2);
	Player2.GetPoint();

	while (bContinue)
	{
		dwWaitResult = WaitForSingleObject(
			ghSemaphore_To_Player2,  // handle to semaphore
			TIMEROUT);      // zero-second time-out interval

		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			if (Player2.DoGame() < 0)
			{
				bContinue = 0;
			}

			if (!ReleaseSemaphore(
				ghSemaphore_To_Diller, // handle to semaphore
				1,      // increase count by one
				NULL))    // not interested in previous count
			{
				TRACE("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;
		case WAIT_TIMEOUT:
			TRACE("Player(2)%d: wait timed out\n", GetCurrentThreadId());
			break;
		}
	}
	TRACE("PLAYER2 DONE");
	return TRUE;
}



static HANDLE aThread[THREADCOUNT];
static DWORD ThreadID[THREADCOUNT];

void WaitFrameworkmain()
{

	bContinue = false;

	if (!ReleaseSemaphore(
		ghSemaphore_Wait_UI_Select, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}
	if (!ReleaseSemaphore(
		ghSemaphore_To_Player2, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}
	if (!ReleaseSemaphore(
		ghSemaphore_To_Player1, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}
	if (!ReleaseSemaphore(
		ghSemaphore_To_Diller, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}

	if (!ReleaseSemaphore(
		ghSemaphore_To_Player2, // handle to semaphore
		1,      // increase count by one
		NULL))    // not interested in previous count
	{
		TRACE("ReleaseSemaphore error: %d\n", GetLastError());
	}
	// Wait for all threads to terminate
	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

	// Close thread and semaphore handles
	for (int i = 0; i < THREADCOUNT; i++)
		CloseHandle(aThread[i]);

	CloseHandle(ghSemaphore_To_Diller);
	CloseHandle(ghSemaphore_To_Player1);
	CloseHandle(ghSemaphore_To_Player2);
	CloseHandle(ghSemaphore_Wait_UI_Select);
}
void InitFrameworkmain()
{
	ghSemaphore_To_Diller = CreateSemaphore(NULL,0,MAX_SEM_COUNT,NULL);
	if (ghSemaphore_To_Diller == NULL)
	{
		TRACE("CreateSemaphore error: %d\n", GetLastError());
		return;
	}

	ghSemaphore_To_Player1 = CreateSemaphore(NULL, 0, MAX_SEM_COUNT, NULL);
	if (ghSemaphore_To_Player1 == NULL)
	{
		TRACE("CreateSemaphore error: %d\n", GetLastError());
		return;
	}

	ghSemaphore_To_Player2 = CreateSemaphore(NULL, 0, MAX_SEM_COUNT, NULL);
	if (ghSemaphore_To_Player2 == NULL)
	{
		TRACE("CreateSemaphore error: %d\n", GetLastError());
		return;
	}

	ghSemaphore_Wait_UI_Select = CreateSemaphore(NULL, 0, MAX_SEM_COUNT, NULL);
	if (ghSemaphore_Wait_UI_Select == NULL)
	{
		TRACE("CreateSemaphore error: %d\n", GetLastError());
		return;
	}
	
	
	aThread[0] = CreateThread(NULL,  0, (LPTHREAD_START_ROUTINE)ThreadDillerProc,	NULL, 0, &ThreadID[0]); // receive thread identifier
	if (aThread[0] == NULL)
	{
		TRACE("CreateThread error: %d\n", GetLastError());
		return;
	}

	aThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPlayer1Proc, NULL, 0, &ThreadID[1]); // receive thread identifier
	if (aThread[1] == NULL)
	{
		TRACE("CreateThread error: %d\n", GetLastError());
		return;
	}

	aThread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPlayer2Proc, NULL, 0, &ThreadID[1]); // receive thread identifier
	if (aThread[2] == NULL)
	{
		TRACE("CreateThread error: %d\n", GetLastError());
		return;
	}	
}