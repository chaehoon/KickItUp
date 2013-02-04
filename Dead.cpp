/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

#include <ddraw.h>
#include <mmsystem.h>
#include <dsound.h>
#include "dead.h"
#include "main.h"
//#include "sound.h"
#include "dsutil.h"

extern	LPDIRECTDRAWSURFACE		DeadScreen;
extern	LPDIRECTDRAWSURFACE		GameOver;

extern	LPDIRECTSOUNDBUFFER		g_dsDead;

extern	DWORD	dwGameCount;

void Dead(void)
{
	static DWORD	cur,last;
	static RECT	UpRect, DownRect;

	static int Firs;

	Score1p=Score2p=0;

	if(Firs==0)
	{
		UpRect.top=240;
		UpRect.left=0;
		UpRect.right=640;
		UpRect.bottom=240;

		DownRect.top=240;
		DownRect.left=0;
		DownRect.right=640;
		DownRect.bottom=240;

		last=cur=timeGetTime();
		
		Firs++;
	}

	if(Firs==1)
	{
		if(g_dsDead)g_dsDead->Play(0,0,0);

		if(UpRect.top==0)Firs++;
		else
		{
			UpRect.top-=40;
			DownRect.bottom+=40;
		}

		g_pDDSBack->BltFast(0,0,DeadScreen,&UpRect,DDBLTFAST_NOCOLORKEY);
		g_pDDSBack->BltFast(0,UpRect.top+240,DeadScreen,&DownRect,DDBLTFAST_NOCOLORKEY);
	}

	if(Firs==2)
	{
		g_pDDSBack->BltFast(0,0,DeadScreen,NULL,DDBLTFAST_NOCOLORKEY);

		cur=timeGetTime();
		if((cur-last)>2500)
		{
			Firs=0;
			Start1p=Start2p=FALSE;
			dwGameCount=0;

			g_ProgramState=GAMEOVER;
		}
	}

	Flipp();
}
	