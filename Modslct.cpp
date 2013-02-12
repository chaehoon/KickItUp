/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.
*/

#include <ddraw.h>
#include "modeslct.h"
#include "input.h"
#include "main.h"

extern	LPDIRECTDRAWSURFACE	g_pDDSBack;
extern	LPDIRECTDRAWSURFACE	ModeEasy;
extern	LPDIRECTDRAWSURFACE	ModeHard;
extern	LPDIRECTDRAWSURFACE	ModeDouble;
extern	LPDIRECTDRAWSURFACE	ModeNonstop;

extern	LPDIRECTDRAWSURFACE	SelectBack;

extern	DWORD	PressedKey[10];
extern	char	g_ProgramState;
extern	char	GameMode;

int ModeSwitch;

void ModeSelect(void)
{
	
	RECT	DiscSize,Button[10];

	DiscSize.top=0;
	DiscSize.left=0;
	DiscSize.right=128;
	DiscSize.bottom=128;
	
	Button[7].top=100;
	Button[7].left=96;
	Button[7].right=96+128;
	Button[7].bottom=100+128;

	Button[9].top=100;
	Button[9].left=416;
	Button[9].right=416+128;
	Button[9].bottom=100+128;

	Button[1].top=300;
	Button[1].left=96;
	Button[1].right=96+128;
	Button[1].bottom=300+128;

	Button[3].top=300;
	Button[3].left=416;
	Button[3].right=416+128;
	Button[3].bottom=300+128;

	g_pDDSBack->BltFast(0,0,SelectBack,NULL,DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	
	g_pDDSBack->Blt(&Button[9],ModeEasy,&DiscSize,DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	g_pDDSBack->Blt(&Button[7],ModeHard,&DiscSize,DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	g_pDDSBack->Blt(&Button[1],ModeDouble,&DiscSize,DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	g_pDDSBack->Blt(&Button[3],ModeNonstop,&DiscSize,DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	ReadGameInput();

	if(PressedKey[7])
	{
		if(ModeSwitch==7)
		{
			ModeSwitch=0;
			GameMode=MODE_EASY;
			g_ProgramState=SELECTSONG;
		}
		else ModeSwitch=7;
	}
	else if(PressedKey[9])
	{
		if(ModeSwitch==9)
		{
			ModeSwitch=0;
			GameMode=MODE_HARD;
			g_ProgramState=SELECTSONG;
		}
		else ModeSwitch=9;
	}
	else if(PressedKey[1])
	{
		if(ModeSwitch==1)
		{
			ModeSwitch=0;
			GameMode=MODE_DOUBLE;
			g_ProgramState=SELECTSONG;
		}
		else ModeSwitch=1;
	}
	else if(PressedKey[3])
	{
		if(ModeSwitch==3)
		{
			ModeSwitch=0;
			GameMode=MODE_HARD; // юс╫ц
			g_ProgramState=SELECTSONG;
		}
		else ModeSwitch=3;
	}

	Flipp();
}
