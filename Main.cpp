/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	2000/07/22 'Work for Opening Source'
			- Removed Motion-Video Support (for improve safety and speed)
			- Removed Xaudio player mp3 play (this required their own dlls)

			- Added Audio-Media play using DirectShow.
	2000/07/23 'Patching'
			- Bpm changing bug fixed (bpm change was incorrected.)
*/
#include "main.h"

// Dshow ..
#include "Media.h"

// Dshow ..

#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <dsound.h>
#include <mmsystem.h>
#include <time.h>
#include <io.h>

#include "result.h"
#include "config.h"
#include "dead.h"
#include "Double.h"
#include "ddutil.h"
#include "dsutil.h"
#include "song.h"
#include "select.h"
//#include "sound.h"
#include "input.h"
#include "resource.h"

#define VER_NUM	"0.4b"
char	TITLE[MAX_PATH];

#define	PRGNAME		"Kick It UP!"

// 일반 하드 데이터 부분
char				Data[MAX_DATA+1][14];
char				Data_Judge[MAX_DATA+1][14];
double				Data_y[MAX_DATA+1];

char				Data1[MAX_DATA+1][14];
char				Data_Judge1[MAX_DATA+1][14];
double				Data_y1[MAX_DATA+1];
// 여기까지

// 더블 데이터 시작
char				Data_Double[MAX_DATA+1][14];
char				Data_Double_Judge[MAX_DATA+1][14];
double				Data_Double_y[MAX_DATA+1];//족보 데이터의 Y값을 가지고 있는 배열
// 더블 데이터 끝


char				SongName[MAX_PATH+1];
char				SongName2[MAX_PATH+1];
char				SongName3[MAX_PATH+1];

char				LogoTitleName[MAX_PATH];

double				bpm;
double				bpm2;
double				bpm3;

int					start,start2,start3;
DWORD					bunki,bunki2;

int					tick;

char				Title[MAX_PATH+1];

char				g_ProgramState=GAMETITLE;
char				ArrowState1p[10];
char				ArrowState2p[10];

char				ArrowState_Joy[10];

DWORD				PressedKey1p[10];
DWORD				PressedKey2p[10];
DWORD				PressedKey_Joy[10];

char				Judgement1p;
char				Judgement2p;
DWORD				Combo1p;
DWORD				Combo2p;
DWORD				dwState;
DWORD				dwState2;
BOOL				g_bActive;

long				Score1p;
long				Score2p;

int					Gauge1p=10;
int					Gauge2p=10;

char				GameMode=MODE_HARD;

char				Couple=FALSE;
char				Double=FALSE;

char				First;
int start1;

BOOL				Start1p;
BOOL				Start2p;

HWND hWnd;
HINSTANCE	g_hInst;

LPDIRECTDRAW g_pDD                = NULL;
LPDIRECTDRAWSURFACE	g_pDDSPrimary = NULL;
LPDIRECTDRAWSURFACE g_pDDSBack    = NULL;

LPDIRECTDRAWSURFACE	GameTITLE		= NULL;
LPDIRECTDRAWSURFACE	Background		= NULL;
LPDIRECTDRAWSURFACE	SongTitle 		= NULL;
LPDIRECTDRAWSURFACE	SongBack 		= NULL;
LPDIRECTDRAWSURFACE	SelectBack		= NULL;
LPDIRECTDRAWSURFACE	JudgeFont		= NULL;
LPDIRECTDRAWSURFACE	NumberFont		= NULL;
LPDIRECTDRAWSURFACE	ComboFont		= NULL;
LPDIRECTDRAWSURFACE NoDISC			= NULL;
LPDIRECTDRAWSURFACE	ShiftLeft		= NULL;
LPDIRECTDRAWSURFACE	ShiftRight		= NULL;
LPDIRECTDRAWSURFACE	GaugeWaku		= NULL;
LPDIRECTDRAWSURFACE Gauge			= NULL;
LPDIRECTDRAWSURFACE Score			= NULL;
LPDIRECTDRAWSURFACE DeadScreen		= NULL;
LPDIRECTDRAWSURFACE GameOver		= NULL;
LPDIRECTDRAWSURFACE Logo			= NULL;
LPDIRECTDRAWSURFACE	Diff			= NULL;
LPDIRECTDRAWSURFACE	DoubleIcon		= NULL;
LPDIRECTDRAWSURFACE	CrazyIcon		= NULL;
LPDIRECTDRAWSURFACE	EasyIcon		= NULL;
LPDIRECTDRAWSURFACE	HardIcon		= NULL;

LPDIRECTDRAWSURFACE	SmallFont		= NULL;
LPDIRECTDRAWSURFACE	Arrow1			= NULL;
LPDIRECTDRAWSURFACE	Arrow2			= NULL;
LPDIRECTDRAWSURFACE	wArrow			= NULL;

LPDIRECTDRAWSURFACE	pArrow1			= NULL;
LPDIRECTDRAWSURFACE	pArrow3			= NULL;
LPDIRECTDRAWSURFACE	pArrow5			= NULL;
LPDIRECTDRAWSURFACE	pArrow7			= NULL;
LPDIRECTDRAWSURFACE	pArrow9			= NULL;

LPDIRECTDRAWSURFACE	cArrow1			= NULL;
LPDIRECTDRAWSURFACE	cArrow3			= NULL;
LPDIRECTDRAWSURFACE	cArrow5			= NULL;
LPDIRECTDRAWSURFACE	cArrow7			= NULL;
LPDIRECTDRAWSURFACE	cArrow9			= NULL;

LPDIRECTDRAWSURFACE	ModeIcon		= NULL;
LPDIRECTDRAWSURFACE	g_cFont			= NULL;

LPDIRECTDRAWSURFACE	ResultFont		= NULL;
LPDIRECTDRAWSURFACE	ResultBack		= NULL;
LPDIRECTDRAWSURFACE	StageCount		= NULL;

LPDIRECTSOUND		lpds			= NULL;
LPDIRECTSOUNDBUFFER	lpdsbd			= NULL;

LPDIRECTSOUNDBUFFER				g_dsOpening	= NULL;
LPDIRECTSOUNDBUFFER				g_dsDead		= NULL;
LPDIRECTSOUNDBUFFER				g_dsMode		= NULL;
LPDIRECTSOUNDBUFFER				g_dsCancel		= NULL;
LPDIRECTSOUNDBUFFER				g_dsMove		= NULL;
LPDIRECTSOUNDBUFFER				g_dsBeat		= NULL;
LPDIRECTSOUNDBUFFER				g_dsSelectSong	= NULL;

SONG				CSONG[512];

BOOL	SongFlag;
BOOL	IntroFlag;

int		HighSpeed1p=1;
int		HighSpeed2p=1;

int		HighSpeed1p_1;
int		HighSpeed1p_3;
int		HighSpeed1p_5;
int		HighSpeed1p_7;
int		HighSpeed1p_9;

int		HighSpeed2p_1;
int		HighSpeed2p_3;
int		HighSpeed2p_5;
int		HighSpeed2p_7;
int		HighSpeed2p_9;

int		MaxSpeed;
int		MinSpeed;

int		JudgeArray[110];

BOOL	bModeMirror1p;
BOOL	bModeNonstep1p;
BOOL	bModeSynchro;
BOOL	bModeUnion1p;
BOOL	bModeRandom1p;
BOOL	b4dMix1p;			// 1p 4DMIX mode.
BOOL	bModeVanish1p;
BOOL	bModeCrazy1p;
BOOL	bModeSuddenR1p;
BOOL	bModeRandomS1p;

BOOL	bModeMirror2p;
BOOL	bModeNonstep2p;
BOOL	bModeUnion2p;
BOOL	bModeRandom2p;
BOOL	b4dMix2p;
BOOL	bModeVanish2p;
BOOL	bModeCrazy2p;
BOOL	bModeSuddenR2p;
BOOL	bModeRandomS2p;

int	ALPHA=0;
int	inc=20;
DWORD	CKey_CFont;
DWORD	CKey_Arr;

DWORD	cPerfect1p;
DWORD	cGreat1p;
DWORD	cGood1p;
DWORD	cBad1p;
DWORD	cMiss1p;
DWORD	cMaxCombo1p;

DWORD	cPerfect2p;
DWORD	cGreat2p;
DWORD	cGood2p;
DWORD	cBad2p;
DWORD	cMiss2p;
DWORD	cMaxCombo2p;

// Data of configuration
KIUCONFIG	KCFG;

DWORD		dwGameCount;

BOOL debugflag=TRUE;
char g_szDebugName[MAX_PATH];

BOOL	g_fullscreen=FALSE;

CMedia *intro;
CMedia *song;

RECT                    g_rcViewport;           // Pos. & size to blt from
RECT                    g_rcScreen;             // Screen pos. for blt

void	DebugPrintf(char *str,...)
{
    if(debugflag)
	{
		char	szBuff[128];
		va_list va;
		FILE	*F;

		va_start(va, str);
		vsprintf(szBuff, str, va);
		F=fopen(g_szDebugName,"a");
		fprintf(F,szBuff);
		va_end(va);
		fclose(F);
	}
}

void	JudgementSet(void)
{
	int i;

	for(i=50;i<110;i++) JudgeArray[i] = BAD;
	for(i=57;i<104;i++) JudgeArray[i] = GOOD;
	for(i=64;i<96;i++)	JudgeArray[i] = GREAT;
	for(i=73;i<88;i++)	JudgeArray[i] = PERFECT;
}

void	DisplayStageCount(DWORD Count)
{
	RECT	sssRect;
	
	sssRect.top=0;
	sssRect.left=Count*80;
	sssRect.right=sssRect.left+80;
	sssRect.bottom=46;

	g_pDDSBack->BltFast(280, 0, StageCount, &sssRect, DDBLTFAST_SRCCOLORKEY);
}



void ClearMode(void)
{
		HighSpeed1p=1;
		bModeMirror1p=FALSE;
		bModeNonstep1p=FALSE;
		bModeSynchro=FALSE;
		bModeUnion1p=FALSE;
		bModeRandom1p=FALSE;
		b4dMix1p=FALSE;
		HighSpeed1p_1=1;
		HighSpeed1p_3=1;
		HighSpeed1p_5=1;
		HighSpeed1p_7=1;
		HighSpeed1p_9=1;
		bModeVanish1p=FALSE;
		bModeRandomS1p=FALSE;
		bModeSuddenR1p=FALSE;

		HighSpeed2p=1;
		bModeMirror2p=FALSE;
		bModeNonstep2p=FALSE;
		bModeUnion2p=FALSE;
		bModeRandom2p=FALSE;
		b4dMix2p=FALSE;
		HighSpeed2p_1=1;
		HighSpeed2p_3=1;
		HighSpeed2p_5=1;
		HighSpeed2p_7=1;
		HighSpeed2p_9=1;
		bModeVanish2p=FALSE;
		Double=FALSE;
		bModeRandomS2p=FALSE;
		bModeSuddenR2p=FALSE;
}

void	GameOver1(void)
{
	static int count;

	if(First==0)
	{
		count=0;
		First++;
	}
	count++;
	g_pDDSBack->BltFast(0,0, GameOver, NULL, DDBLTFAST_NOCOLORKEY);
	if(count==60)First=0, g_ProgramState=GAMETITLE;

	Flipp();
}

void DrawBackground(char Data[][14], DWORD i, int temp)
{
	g_pDDSBack->BltFast(0,0,SongBack,NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
}

void DrawScore1p(void)
{
	RECT	cRect;
	char chScore[11];
	int Loop;

	sprintf(chScore,"%07d",Score1p);
			
	for(Loop=0;;Loop++)
	{
		if(chScore[Loop]==NULL)break;
		chScore[Loop]-=48;
		cRect.left=chScore[Loop]*22;
		cRect.right=cRect.left+21;
		cRect.top=0;
		cRect.bottom=35;
		g_pDDSBack->BltFast(20+Loop*22,444,Score,&cRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}/* 여기까지 */
}

void DrawScore2p(void)
{
	RECT	cRect;
	char chScore[11];
	int Loop;

	sprintf(chScore,"%07d",Score2p);
			
	for(Loop=0;;Loop++)
	{
		if(chScore[Loop]==NULL)break;
		chScore[Loop]-=48;
		cRect.left=chScore[Loop]*22;
		cRect.right=cRect.left+21;
		cRect.top=0;
		cRect.bottom=35;
		g_pDDSBack->BltFast(463+Loop*22,444,Score,&cRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}/* 여기까지 */
}

void DrawGauge1p(void)
{
	int CurG;
	int i;
//	static	int	Phase;

	RECT sRect;

	CurG=Gauge1p;
	
	if(CurG<0)CurG=0;

	g_pDDSBack->BltFast(32,0,GaugeWaku,NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

/*	if(Phase==1)
	{
		Phase=0;
		return;
	}
	else Phase++;*/
	
	sRect.top=0;
	sRect.left=0;
	sRect.right=6;
	sRect.bottom=20;

	for(i=0;i<7;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	sRect.top=0;
	sRect.left=6;
	sRect.right=12;
	sRect.bottom=20;

	for(i=7;i<21;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	sRect.top=0;
	sRect.left=12;
	sRect.right=18;
	sRect.bottom=20;

	for(i=21;i<42;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

}

void DrawGauge2p(void)
{
	int CurG;
	int i;
	RECT sRect;
//	static	int	Phase;

	CurG=Gauge2p;
	
	if(CurG<0)CurG=0;

	g_pDDSBack->BltFast(352,0,GaugeWaku,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

/*	if(Phase==1)
	{
		Phase=0;
		return;
	}
	else Phase++;*/

	sRect.top=0;
	sRect.left=0;
	sRect.right=6;
	sRect.bottom=20;

	for(i=0;i<7;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(352+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	sRect.top=0;
	sRect.left=6;
	sRect.right=12;
	sRect.bottom=20;

	for(i=7;i<21;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(352+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	sRect.top=0;
	sRect.left=12;
	sRect.right=18;
	sRect.bottom=20;

	for(i=21;i<42;i++)
	{
		if(i>CurG)break;
		g_pDDSBack->BltFast(352+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

}

void KIU_STAGE(void)
{
	static int temp;
	static DWORD i;
	static DWORD cur,last,sec;
	static DWORD starttime, curtime;

	static RECT rect1[7],rect3[7],rect5[7],rect7[7],rect9[7];
	int k;
	DWORD delta;

	static int sta;

	static double tail;

	static time_t t;

	static	HRESULT	hr;

	char s[50];

	double bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

	DrawBackground(Data,i,temp);
	DisplayStageCount(dwGameCount);
	
	
	// Display debug msg.
	sprintf(s, "start1: %d", start);
	DisplayMessage(0,0,s);
	sprintf(s, "start2: %d", start2);
	DisplayMessage(0,16,s);
	sprintf(s, "start3: %d", start3);
	DisplayMessage(0,32,s);
	sprintf(s, "bpm 1:%g", bpm);
	DisplayMessage(0,48,s);
	sprintf(s, "bpm 2:%g", bpm2);
	DisplayMessage(0,64,s);
	sprintf(s, "bpm 3:%g", bpm3);
	DisplayMessage(0,80,s);
	sprintf(s, "bunki :%ld", bunki);
	DisplayMessage(0,96,s);
	sprintf(s, "bunki2:%ld", bunki2);
	DisplayMessage(0,112,s);

	if(start1==0)
	{
		if(b4dMix1p==TRUE)
		{
			MaxSpeed = MinSpeed = HighSpeed1p_1;

			MaxSpeed = max( MaxSpeed, HighSpeed1p_3);
			MaxSpeed = max( MaxSpeed, HighSpeed1p_5);
			MaxSpeed = max( MaxSpeed, HighSpeed1p_7);
			MaxSpeed = max( MaxSpeed, HighSpeed1p_9);
			
			MinSpeed = min( MinSpeed, HighSpeed1p_3);
			MinSpeed = min( MinSpeed, HighSpeed1p_5);
			MinSpeed = min( MinSpeed, HighSpeed1p_7);
			MinSpeed = min( MinSpeed, HighSpeed1p_9);
		}
		else 
		{
			MaxSpeed = MinSpeed = HighSpeed1p;
			HighSpeed1p_1 = HighSpeed1p_3 = HighSpeed1p_5 = HighSpeed1p_7 = HighSpeed1p_9 = HighSpeed1p;
		}

		if(b4dMix2p)
		{
			MaxSpeed=MinSpeed=HighSpeed2p_1;

			MaxSpeed = max( MaxSpeed, HighSpeed2p_3);
			MaxSpeed = max( MaxSpeed, HighSpeed2p_5);
			MaxSpeed = max( MaxSpeed, HighSpeed2p_7);
			MaxSpeed = max( MaxSpeed, HighSpeed2p_9);
			
			MinSpeed = min( MinSpeed, HighSpeed2p_3);
			MinSpeed = min( MinSpeed, HighSpeed2p_5);
			MinSpeed = min( MinSpeed, HighSpeed2p_7);
			MinSpeed = min( MinSpeed, HighSpeed2p_9);
		}
		else 
		{
			MaxSpeed = MinSpeed = HighSpeed2p;
			HighSpeed2p_1 = HighSpeed2p_3 = HighSpeed2p_5 = HighSpeed2p_7 = HighSpeed2p_9 = HighSpeed2p;
		}

		for(sta=0;sta<6;sta++)
		{
			rect7[sta].top=0;
			rect7[sta].left=PUMP_SPRITE_NEW*sta;
			rect7[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect7[sta].bottom=PUMP_SPRITE_NEW;

			rect9[sta].top=PUMP_SPRITE_NEW;
			rect9[sta].left=PUMP_SPRITE_NEW*sta;
			rect9[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect9[sta].bottom=PUMP_SPRITE_NEW*2;

			rect5[sta].top=PUMP_SPRITE_NEW*2;
			rect5[sta].left=PUMP_SPRITE_NEW*sta;
			rect5[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect5[sta].bottom=PUMP_SPRITE_NEW*3;

			rect3[sta].top=PUMP_SPRITE_NEW*3;
			rect3[sta].left=PUMP_SPRITE_NEW*sta;
			rect3[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect3[sta].bottom=PUMP_SPRITE_NEW*4;

			rect1[sta].top=PUMP_SPRITE_NEW*4;
			rect1[sta].left=PUMP_SPRITE_NEW*sta;
			rect1[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect1[sta].bottom=PUMP_SPRITE_NEW*5;
		}
		sta=0;

		Gauge1p=10;
		Gauge2p=10;

		if(bModeRandom1p)
		{
			srand((unsigned) time(&t));
			
			for(i=0;i<MAX_DATA;i++)
			{
				Data[MAX_DATA][0]=Data[i][0];
				Data[MAX_DATA][1]=Data[i][1];
				Data[MAX_DATA][2]=Data[i][2];
				Data[MAX_DATA][3]=Data[i][3];
				Data[MAX_DATA][4]=Data[i][4];

				if(Data[i][0]=='2')break;
				
				Data[i][0]=Data[i][1]=Data[i][2]=Data[i][3]=Data[i][4]='0';

				if(Data[MAX_DATA][0]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][1]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][2]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][3]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][4]=='1')Data[i][rand()%5]='1';
			}
		}
		
		if(bModeRandom2p)
		{
			if(!bModeRandom1p)srand((unsigned) time(&t));
			
			for(i=0;i<MAX_DATA;i++)
			{
				Data1[MAX_DATA][5]=Data1[i][5];
				Data1[MAX_DATA][6]=Data1[i][6];
				Data1[MAX_DATA][7]=Data1[i][7];
				Data1[MAX_DATA][8]=Data1[i][8];
				Data1[MAX_DATA][9]=Data1[i][9];

				if(Data1[i][0]=='2')break;
				
				Data1[i][5]=Data1[i][6]=Data1[i][7]=Data1[i][8]=Data1[i][9]='0';

				if(Data1[MAX_DATA][5]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][6]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][7]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][8]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][9]=='1')Data1[i][5+rand()%5]='1';
			}
		}

		if(bModeMirror1p)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				Data[MAX_DATA][0]=Data[i][0];
				Data[MAX_DATA][1]=Data[i][1];
				Data[MAX_DATA][2]=Data[i][2];
				Data[MAX_DATA][3]=Data[i][3];
				Data[MAX_DATA][4]=Data[i][4];

				if(Data[i][0]=='2')break;

				if(Data[MAX_DATA][0]=='1')Data[i][3]='1'; else Data[i][3]='0'; 
				if(Data[MAX_DATA][1]=='1')Data[i][4]='1'; else Data[i][4]='0'; 
				if(Data[MAX_DATA][2]=='1')Data[i][2]='1'; else Data[i][2]='0'; 
				if(Data[MAX_DATA][3]=='1')Data[i][0]='1'; else Data[i][0]='0'; 
				if(Data[MAX_DATA][4]=='1')Data[i][1]='1'; else Data[i][1]='0'; 

			}
		}
		if(bModeMirror2p)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				Data1[MAX_DATA][5]=Data1[i][5];
				Data1[MAX_DATA][6]=Data1[i][6];
				Data1[MAX_DATA][7]=Data1[i][7];
				Data1[MAX_DATA][8]=Data1[i][8];
				Data1[MAX_DATA][9]=Data1[i][9];

				if(Data1[i][0]=='2')break;

				if(Data1[MAX_DATA][5]=='1')Data1[i][8]='1'; else Data1[i][8]='0'; 
				if(Data1[MAX_DATA][6]=='1')Data1[i][9]='1'; else Data1[i][9]='0'; 
				if(Data1[MAX_DATA][7]=='1')Data1[i][7]='1'; else Data1[i][7]='0'; 
				if(Data1[MAX_DATA][8]=='1')Data1[i][5]='1'; else Data1[i][5]='0'; 
				if(Data1[MAX_DATA][9]=='1')Data1[i][6]='1'; else Data1[i][6]='0'; 

			}
		}
		
		memcpy(&Data_Judge,&Data,sizeof(Data));
		memcpy(&Data_Judge1,&Data1,sizeof(Data));
	
		if(bModeNonstep1p)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				if(!(Data[i][0]=='0' && Data[i][1]=='0' && Data[i][2]=='0' && Data[i][3]=='0' && Data[i][4]=='0'))
				{
					i++;
					for(;i<MAX_DATA;i++)
					{
						if(Data[i][0]=='2')i=MAX_DATA;
						else
						{
							Data[i][0]='3';
							Data[i][1]='3';
							Data[i][2]='3';
							Data[i][3]='3';
							Data[i][4]='3';
						}
					}
				}
			}
		}
		
		if(bModeNonstep2p)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				if(!(Data1[i][5]=='0' && Data1[i][6]=='0' && Data1[i][7]=='0' && Data1[i][8]=='0' && Data1[i][9]=='0'))
				{
					i++;
					for(;i<MAX_DATA;i++)
					{
						if(Data1[i][0]=='2')i=MAX_DATA;
						else
						{
							Data1[i][5]='3';
							Data1[i][6]='3';
							Data1[i][7]='3';
							Data1[i][8]='3';
							Data1[i][9]='3';
						}
					}
				}
			}
		}

		//FadeToSurface(SongBack);
		g_pDDSBack->BltFast(0,0, SongBack, NULL, DDBLTFAST_NOCOLORKEY);

		if(SongFlag==TRUE)
		{
			song->OnMediaPlay();
		}

		start*=10;
		start2*=10;start3*=10;
		bunki*=10;bunki2*=10;
		
		last=cur=timeGetTime();
		tail=0;
		i=0;
		temp=0;
		start1++;
		temp+=55;
		starttime=start;
		curtime=0;
	}
	
	cur=timeGetTime();        // 130/ 600000
	delta=cur-last;
	last=cur;

	if(Start1p)DrawArrow1p(i); //회색 화살표를 말합니다.
	if(Start2p)DrawArrow2p(i);

	start-=delta;

	if(bunki!=0 && (curtime >= bunki))
	{
		bunki=0;
		bpm=bpm2;
		bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

		starttime=start2;

	}

	if(bunki==0 && bunki2!=0 && (curtime >= bunki2))
	{
		bunki2=0;
		bpm=bpm3;
		bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

		starttime=start3;
	}

	if(start<=0)
	{
		start=0;
		if(SongFlag)
		{
			i=0;
			temp=+55;
			tail=0;

			curtime=(DWORD)(song->GetCurrentPosition()*1000);

			if(curtime > starttime) 
			delta=(DWORD)curtime-starttime;
			else delta=(DWORD)curtime;
		}

		//1000 Tick당 180/60 -> 1초에 64*(180/60)  즉 1 tick 당 64*(bpm/60)/1000
		temp-=(int)(delta*bpmpix);
		tail+=(double)((double)(delta*bpmpix)-(int)(delta*bpmpix));

		if(tail>=1)
		{
			temp-=(int)tail;
			tail-=(int)tail;
		}

		if(temp<-100)
		{
			while(temp<-100)
			{
				temp+=(PUMP_SPRITE_Y);

				i+=tick;
			}
		}

	}
	if(timeGetTime()-sec>50)
	{
		sec=timeGetTime();
		if(sta==5)sta=0;
		else sta++;
	}

	if(Start1p)
	for(k=0;k<48;k+=tick) 
	{
		if(Data[i][0]=='2' || Data[i+1][0]=='2' || Data[i+2][0]=='2' || Data[i+3][0]=='2')
		{
			k=48;
			if(SongFlag)
			{
				song->OnMediaStop();
				delete song;
				SongFlag=FALSE;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data[i+k][0]=='1')
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			
			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(bModeSuddenR1p)
			{
				if(Data_y[i+k]>240 && Data_y[i+k]<290)
				{
					Data[MAX_DATA][0]=Data[i+k][0];
					Data[MAX_DATA][1]=Data[i+k][1];
					Data[MAX_DATA][2]=Data[i+k][2];
					Data[MAX_DATA][3]=Data[i+k][3];
					Data[MAX_DATA][4]=Data[i+k][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k][0]=Data[i+k][1]=Data[i+k][2]=Data[i+k][3]=Data[i+k][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k][rand()%5]='1';

						Data_Judge[i+k][0]=Data[i+k][0];
						Data_Judge[i+k][1]=Data[i+k][1];
						Data_Judge[i+k][2]=Data[i+k][2];
						Data_Judge[i+k][3]=Data[i+k][3];
						Data_Judge[i+k][4]=Data[i+k][4];
					}
				}

				if(Data_y[i+k+1]>240 && Data_y[i+k+1]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+1][0];
					Data[MAX_DATA][1]=Data[i+k+1][1];
					Data[MAX_DATA][2]=Data[i+k+1][2];
					Data[MAX_DATA][3]=Data[i+k+1][3];
					Data[MAX_DATA][4]=Data[i+k+1][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+1][0]=Data[i+k+1][1]=Data[i+k+1][2]=Data[i+k+1][3]=Data[i+k+1][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+1][rand()%5]='1';

						Data_Judge[i+k+1][0]=Data[i+k+1][0];
						Data_Judge[i+k+1][1]=Data[i+k+1][1];
						Data_Judge[i+k+1][2]=Data[i+k+1][2];
						Data_Judge[i+k+1][3]=Data[i+k+1][3];
						Data_Judge[i+k+1][4]=Data[i+k+1][4];
					}
				}
			}
		}	
		else if(tick==4)
		{
			if(Data[i+k][0]=='1')
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+2][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data[i+k+3][0]=='1')
				ClpBlt(LP1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_1-(PUMP_SPRITE_Y)*(HighSpeed1p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][1]=='1')
				ClpBlt(LP7_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_7-(PUMP_SPRITE_Y)*(HighSpeed1p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][2]=='1')
				ClpBlt(LP5_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_5-(PUMP_SPRITE_Y)*(HighSpeed1p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][3]=='1')
				ClpBlt(LP9_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_9-(PUMP_SPRITE_Y)*(HighSpeed1p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][4]=='1')
				ClpBlt(LP3_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed1p_3-(PUMP_SPRITE_Y)*(HighSpeed1p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(bModeSuddenR1p)
			{
				if(Data_y[i+k]>240 && Data_y[i+k]<290)
				{
					Data[MAX_DATA][0]=Data[i+k][0];
					Data[MAX_DATA][1]=Data[i+k][1];
					Data[MAX_DATA][2]=Data[i+k][2];
					Data[MAX_DATA][3]=Data[i+k][3];
					Data[MAX_DATA][4]=Data[i+k][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k][0]=Data[i+k][1]=Data[i+k][2]=Data[i+k][3]=Data[i+k][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k][rand()%5]='1';

						Data_Judge[i+k][0]=Data[i+k][0];
						Data_Judge[i+k][1]=Data[i+k][1];
						Data_Judge[i+k][2]=Data[i+k][2];
						Data_Judge[i+k][3]=Data[i+k][3];
						Data_Judge[i+k][4]=Data[i+k][4];
					}
				}
				if(Data_y[i+k+1]>240 && Data_y[i+k+1]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+1][0];
					Data[MAX_DATA][1]=Data[i+k+1][1];
					Data[MAX_DATA][2]=Data[i+k+1][2];
					Data[MAX_DATA][3]=Data[i+k+1][3];
					Data[MAX_DATA][4]=Data[i+k+1][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+1][0]=Data[i+k+1][1]=Data[i+k+1][2]=Data[i+k+1][3]=Data[i+k+1][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+1][rand()%5]='1';

						Data_Judge[i+k+1][0]=Data[i+k+1][0];
						Data_Judge[i+k+1][1]=Data[i+k+1][1];
						Data_Judge[i+k+1][2]=Data[i+k+1][2];
						Data_Judge[i+k+1][3]=Data[i+k+1][3];
						Data_Judge[i+k+1][4]=Data[i+k+1][4];
					}
				}
				if(Data_y[i+k+2]>240 && Data_y[i+k+2]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+2][0];
					Data[MAX_DATA][1]=Data[i+k+2][1];
					Data[MAX_DATA][2]=Data[i+k+2][2];
					Data[MAX_DATA][3]=Data[i+k+2][3];
					Data[MAX_DATA][4]=Data[i+k+2][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+2][0]=Data[i+k+2][1]=Data[i+k+2][2]=Data[i+k+2][3]=Data[i+k+2][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+2][rand()%5]='1';

						Data_Judge[i+k+2][0]=Data[i+k+2][0];
						Data_Judge[i+k+2][1]=Data[i+k+2][1];
						Data_Judge[i+k+2][2]=Data[i+k+2][2];
						Data_Judge[i+k+2][3]=Data[i+k+2][3];
						Data_Judge[i+k+2][4]=Data[i+k+2][4];
					}
				}
				if(Data_y[i+k+3]>240 && Data_y[i+k+3]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+3][0];
					Data[MAX_DATA][1]=Data[i+k+3][1];
					Data[MAX_DATA][2]=Data[i+k+3][2];
					Data[MAX_DATA][3]=Data[i+k+3][3];
					Data[MAX_DATA][4]=Data[i+k+3][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+3][0]=Data[i+k+3][1]=Data[i+k+3][2]=Data[i+k+3][3]=Data[i+k+3][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+3][rand()%5]='1';

						Data_Judge[i+k+3][0]=Data[i+k+3][0];
						Data_Judge[i+k+3][1]=Data[i+k+3][1];
						Data_Judge[i+k+3][2]=Data[i+k+3][2];
						Data_Judge[i+k+3][3]=Data[i+k+3][3];
						Data_Judge[i+k+3][4]=Data[i+k+3][4];
					}
				}
			}
		}
	}

	if(Start2p)
	for(k=0;k<48;k+=tick) 
	{
		if(Data1[i][0]=='2' || Data1[i+1][0]=='2' || Data1[i+2][0]=='2' || Data1[i+3][0]=='2')
		{
			k=48;
			if(SongFlag)
			{
				song->OnMediaStop();
				delete song;
				SongFlag=FALSE;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data1[i+k][5]=='1')
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			
			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(bModeSuddenR2p)
			{
				if(Data_y1[i+k]>240 && Data_y1[i+k]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k][5];
					Data1[MAX_DATA][6]=Data1[i+k][6];
					Data1[MAX_DATA][7]=Data1[i+k][7];
					Data1[MAX_DATA][8]=Data1[i+k][8];
					Data1[MAX_DATA][9]=Data1[i+k][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k][5]=Data1[i+k][6]=Data1[i+k][7]=Data1[i+k][8]=Data1[i+k][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k][5 + rand()%5]='1';

						Data_Judge1[i+k][5]=Data1[i+k][5];
						Data_Judge1[i+k][6]=Data1[i+k][6];
						Data_Judge1[i+k][7]=Data1[i+k][7];
						Data_Judge1[i+k][8]=Data1[i+k][8];
						Data_Judge1[i+k][9]=Data1[i+k][9];
					}
				}
				if(Data_y1[i+k+1]>240 && Data_y1[i+k+1]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+1][5];
					Data1[MAX_DATA][6]=Data1[i+k+1][6];
					Data1[MAX_DATA][7]=Data1[i+k+1][7];
					Data1[MAX_DATA][8]=Data1[i+k+1][8];
					Data1[MAX_DATA][9]=Data1[i+k+1][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+1][5]=Data1[i+k+1][6]=Data1[i+k+1][7]=Data1[i+k+1][8]=Data1[i+k+1][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+1][5 + rand()%5]='1';

						Data_Judge1[i+k+1][5]=Data1[i+k+1][5];
						Data_Judge1[i+k+1][6]=Data1[i+k+1][6];
						Data_Judge1[i+k+1][7]=Data1[i+k+1][7];
						Data_Judge1[i+k+1][8]=Data1[i+k+1][8];
						Data_Judge1[i+k+1][9]=Data1[i+k+1][9];
					}
				}
			}
		}	
		else if(tick==4)
		{

			if(Data1[i+k][5]=='1')
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+2][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data1[i+k+3][5]=='1')
				ClpBlt(LP1_X1,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_1-(PUMP_SPRITE_Y)*(HighSpeed2p_1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][6]=='1')
				ClpBlt(LP7_X1,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_7-(PUMP_SPRITE_Y)*(HighSpeed2p_7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][7]=='1')
				ClpBlt(LP5_X1,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_5-(PUMP_SPRITE_Y)*(HighSpeed2p_5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][8]=='1')
				ClpBlt(LP9_X1,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_9-(PUMP_SPRITE_Y)*(HighSpeed2p_9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][9]=='1')
				ClpBlt(LP3_X1,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed2p_3-(PUMP_SPRITE_Y)*(HighSpeed2p_3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(bModeSuddenR2p)
			{
				if(Data_y1[i+k]>240 && Data_y1[i+k]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k][5];
					Data1[MAX_DATA][6]=Data1[i+k][6];
					Data1[MAX_DATA][7]=Data1[i+k][7];
					Data1[MAX_DATA][8]=Data1[i+k][8];
					Data1[MAX_DATA][9]=Data1[i+k][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k][5]=Data1[i+k][6]=Data1[i+k][7]=Data1[i+k][8]=Data1[i+k][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k][5 + rand()%5]='1';

						Data_Judge1[i+k][5]=Data1[i+k][5];
						Data_Judge1[i+k][6]=Data1[i+k][6];
						Data_Judge1[i+k][7]=Data1[i+k][7];
						Data_Judge1[i+k][8]=Data1[i+k][8];
						Data_Judge1[i+k][9]=Data1[i+k][9];
					}
				}
				if(Data_y1[i+k+1]>240 && Data_y1[i+k+1]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+1][5];
					Data1[MAX_DATA][6]=Data1[i+k+1][6];
					Data1[MAX_DATA][7]=Data1[i+k+1][7];
					Data1[MAX_DATA][8]=Data1[i+k+1][8];
					Data1[MAX_DATA][9]=Data1[i+k+1][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+1][5]=Data1[i+k+1][6]=Data1[i+k+1][7]=Data1[i+k+1][8]=Data1[i+k+1][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+1][5 + rand()%5]='1';

						Data_Judge1[i+k+1][5]=Data1[i+k+1][5];
						Data_Judge1[i+k+1][6]=Data1[i+k+1][6];
						Data_Judge1[i+k+1][7]=Data1[i+k+1][7];
						Data_Judge1[i+k+1][8]=Data1[i+k+1][8];
						Data_Judge1[i+k+1][9]=Data1[i+k+1][9];
					}
				}
				if(Data_y1[i+k+2]>240 && Data_y1[i+k+2]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+2][5];
					Data1[MAX_DATA][6]=Data1[i+k+2][6];
					Data1[MAX_DATA][7]=Data1[i+k+2][7];
					Data1[MAX_DATA][8]=Data1[i+k+2][8];
					Data1[MAX_DATA][9]=Data1[i+k+2][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+2][5]=Data1[i+k+2][6]=Data1[i+k+2][7]=Data1[i+k+2][8]=Data1[i+k+2][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+2][5 + rand()%5]='1';

						Data_Judge1[i+k+2][5]=Data1[i+k+2][5];
						Data_Judge1[i+k+2][6]=Data1[i+k+2][6];
						Data_Judge1[i+k+2][7]=Data1[i+k+2][7];
						Data_Judge1[i+k+2][8]=Data1[i+k+2][8];
						Data_Judge1[i+k+2][9]=Data1[i+k+2][9];
					}
				}
				if(Data_y1[i+k+3]>240 && Data_y1[i+k+3]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+3][5];
					Data1[MAX_DATA][6]=Data1[i+k+3][6];
					Data1[MAX_DATA][7]=Data1[i+k+3][7];
					Data1[MAX_DATA][8]=Data1[i+k+3][8];
					Data1[MAX_DATA][9]=Data1[i+k+3][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+3][5]=Data1[i+k+3][6]=Data1[i+k+3][7]=Data1[i+k+3][8]=Data1[i+k+3][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+3][5 + rand()%5]='1';

						Data_Judge1[i+k+3][5]=Data1[i+k+3][5];
						Data_Judge1[i+k+3][6]=Data1[i+k+3][6];
						Data_Judge1[i+k+3][7]=Data1[i+k+3][7];
						Data_Judge1[i+k+3][8]=Data1[i+k+3][8];
						Data_Judge1[i+k+3][9]=Data1[i+k+3][9];
					}
				}
			}
		}
	}

	if(Start1p)
	{
		if(KCFG.OptJudge)DrawJudge1p();
		DrawGauge1p();
		DrawScore1p();
	}
	if(Start2p)
	{
		if(KCFG.OptJudge)DrawJudge2p();
		DrawGauge2p();
		DrawScore2p();
	}

	if(bModeMirror1p)DrawMode(0,200,HMODE_MIRROR);
	if(bModeNonstep1p)DrawMode(0,240,HMODE_NONSTEP);
	if(bModeSynchro)DrawMode(0,280,HMODE_SYNCHRO);
	if(bModeUnion1p)DrawMode(0,320,HMODE_UNION);
	if(bModeRandom1p)DrawMode(0,360,HMODE_RANDOM);
	if(bModeVanish1p)DrawMode(0,400,HMODE_VANISH);

	if(HighSpeed1p>1)DrawMode(0,160,HMODE_2X);

	if(bModeMirror2p)DrawMode(600,200,HMODE_MIRROR);
	if(bModeNonstep2p)DrawMode(600,240,HMODE_NONSTEP);
	if(bModeUnion2p)DrawMode(600,320,HMODE_UNION);
	if(bModeRandom2p)DrawMode(600,360,HMODE_RANDOM);
	if(bModeVanish2p)DrawMode(600,400,HMODE_VANISH);

	if(HighSpeed2p>1)DrawMode(600,160,HMODE_2X);

	Flipp();
}


void WaveSet_Loading(void)
{
	g_dsOpening=DSLoadSoundBuffer(lpds, "wave\\Opening.wav");
	g_dsDead=DSLoadSoundBuffer(lpds,"wave\\Dead.wav");
	g_dsMode=DSLoadSoundBuffer(lpds,"wave\\Mode.wav");
	g_dsCancel=DSLoadSoundBuffer(lpds,"wave\\Cancel.wav");
	g_dsMove=DSLoadSoundBuffer(lpds,"wave\\Move.wav");
	g_dsBeat=DSLoadSoundBuffer(lpds,"wave\\Beat.wav");
	g_dsSelectSong=DSLoadSoundBuffer(lpds, "wave\\MusicSelect.wav");
}

void DisplayMessage(int x, int y, char * message)
{
#define FONT_SIZE	8
#define FONT_HEIGHT	16

	RECT sRect = { 0, };

	for(int i = 0 ; ; i++) {		
		if(message[i] == NULL)
			break;	
	
		char msg1 = toupper(message[i]);
		
		sRect.top = 0;
		sRect.bottom = FONT_HEIGHT;
		sRect.left = FONT_SIZE * (msg1-' ');
		sRect.right = sRect.left + FONT_SIZE;
		
		g_pDDSBack->BltFast(x, y, SmallFont, &sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		x += FONT_SIZE;
	}
}

			
HRESULT	ClpBlt(int x ,int y ,LPDIRECTDRAWSURFACE ds,LPRECT srect,DWORD mode)
{
	static RECT sRect;
	HRESULT	hRet;

	memcpy(&sRect,srect,sizeof(sRect));
	
	if(x>640 || y>480) return DD_OK;

	if(y+(srect->bottom-srect->top)>480)srect->bottom=srect->bottom-(y+(srect->bottom-srect->top)-480);
	if(y<0)
	{
		srect->top-=y;
		y=0;
	}

	if(x+(srect->right-srect->left)>640)srect->right=srect->right-(x+(srect->right-srect->left)-640);
	if(x<0)
	{
		srect->left-=x;
		x=0;
	}

	if(g_ProgramState==DOUBLE)
	{
		if(bModeVanish1p || bModeVanish2p)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(bModeSuddenR1p || bModeSuddenR2p)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}
	else if(x<320)
	{
		if(bModeVanish1p)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(bModeSuddenR1p)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}
	else if(x>320)
	{
		if(bModeVanish2p)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(bModeSuddenR2p)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}

	memcpy(srect,&sRect,sizeof(sRect));
	
	return hRet;

}

void StageTitle(void)
{
	RECT	lRect;

	if(First==0)
	{
		ClearMode();

		Start1p=FALSE;
		Start2p=FALSE;
		First++;
		if(g_dsOpening)
			g_dsOpening->Play(0,0,0);
	}

	ReadGameInput();

	if(PressedKey1p[5]==TRUE)
		Start1p=TRUE;

	if(PressedKey2p[5]==TRUE)
		Start2p=TRUE;
	
	// Draw Background image. "KICK IT UP"
	g_pDDSBack->BltFast(0,0,GameTITLE,NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);

	// Check Start.
	if(Start1p || Start2p)
	{
		if(Start1p && Start2p)
		{
			PressedKey2p[0]=3;
		}
		else if(Start1p)
		{
			if(PressedKey1p[5]==TRUE)
				PressedKey2p[0]=3;
		}
		else if(Start2p)
		{
			if(PressedKey2p[5]==TRUE)
				PressedKey2p[0]=3;
		}
	}

	// if start button is pressed.
	if(PressedKey2p[0]==3)
	{
		Couple = Start1p && Start2p;

		First=0;
		if(g_dsOpening)
			g_dsOpening->Stop();
		PressedKey2p[0]=0;

		// Change ProgramState to SelectSong Stage
		g_ProgramState=SELECTSONG;

	}

	// Draw to screen "FREE PLAY!"
	lRect.top=46;
	lRect.left=0;
	lRect.right=220;
	lRect.bottom=69;

	g_pDDSBack->BltFast(210,450,g_cFont, &lRect, DDBLTFAST_SRCCOLORKEY);

	if(Start1p==FALSE)
	{
		// Draw to screen (10, 450) "PRESS CENTER BUTTON"
		lRect.top=0;
		lRect.left=0;
		lRect.right=220;
		lRect.bottom=23;

		TransAlphaImproved(g_cFont, g_pDDSBack, 10, 450, lRect, ALPHA, CKey_CFont, 16);
	}
	if(Start2p==FALSE)//DisplayMessage(320,480-20,"PRESS CENTER STEP");
	{
		// Draw to screen (410, 450) "PRESS CENTER BUTTON"
		lRect.top=0;
		lRect.left=0;
		lRect.right=220;
		lRect.bottom=23;

		TransAlphaImproved(g_cFont, g_pDDSBack, 410, 450, lRect, ALPHA, CKey_CFont, 16);
	}

	ALPHA += inc;
	if (ALPHA > 256)
	{
		ALPHA = 256;
		inc = -20;
	}
	else if (ALPHA < 0)
	{
		ALPHA = 0;
		inc = 20;
	}


	Flipp();

}

void DrawJudge1p(void)
{
	static DWORD cur, last;
	static char LastJudge;

	char chCombo1p[255];

	static DWORD sec,delta;

	RECT rRect, cRect, destRect;
	int Loop;

	if(Judgement1p)
	{
		dwState=0;
	}
	else if(dwState)
	{
		Judgement1p=LastJudge;

		delta=timeGetTime()-sec;
		if(delta>16)
		{
			sec=timeGetTime();
			if(dwState>=40)
			{
				dwState=0;
				Judgement1p=NONE;
			}
			else
			{
				if(delta >16 && delta <32)dwState+=2;
				else if(delta >=32)dwState+=4;
			}
		}
	}

	switch(Judgement1p)
	{
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.top=0;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.top=JUDGE_SIZE_Y;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*2;
			if(dwState==0)dwState++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.top=JUDGE_SIZE_Y*2;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*3;
			if(dwState==0)dwState++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.top=JUDGE_SIZE_Y*3;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*4;
			if(dwState==0)dwState++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.top=JUDGE_SIZE_Y*4;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*5;
			if(dwState==0)dwState++;
			break;
	}

	if(dwState>15)
	{
		destRect.top=200;
		destRect.left=40;
		destRect.right=40+JUDGE_SIZE_X;
		destRect.bottom=200+JUDGE_SIZE_Y;
	}
	else
	{
		destRect.top=200-30+(dwState*2);
		destRect.left=40-60+(dwState*4);
		destRect.right=40+JUDGE_SIZE_X+60-(dwState*4);
		destRect.bottom=200+JUDGE_SIZE_Y+30-(dwState*2);
	}

	if(Judgement1p)
	{
		//g_pDDSBack->BltFast(40,200+dwState/2,JudgeFont,&rRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		
		g_pDDSBack->Blt(&destRect, JudgeFont, &rRect,DDBLT_WAIT | DDBLT_KEYSRC , NULL);

		/* 콤보 출력부 입니다. */
		if((Judgement1p==PERFECT || Judgement1p==GREAT) && Combo1p>3)
		{
			sprintf(chCombo1p,"%03d",Combo1p);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo1p[Loop]==NULL)break;
				chCombo1p[Loop]-=48;
				cRect.left=chCombo1p[Loop]*50;
				cRect.right=cRect.left+50;
				cRect.top=0;
				cRect.bottom=65;
				if(dwState>10)g_pDDSBack->BltFast(80+Loop*50,250+dwState*2-dwState*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(80+Loop*50,250+dwState*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);

				cRect.left=0;
				cRect.right=150;
				cRect.top=65;
				cRect.bottom=100;
				
				if(dwState>10)g_pDDSBack->BltFast(80,320+dwState*2-dwState*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(80,320+dwState*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
			}/* 여기까지 */
		}
	}

	Judgement1p=NONE;
}

void DrawJudge2p(void)
{
	static DWORD cur, last;
	static char LastJudge;

	char chCombo2p[255];

	static DWORD sec,delta;

	RECT rRect, cRect, destRect;
	int Loop;

	if(Judgement2p)
	{
		dwState2=0;
	}
	else if(dwState2)
	{
		Judgement2p=LastJudge;

		delta=timeGetTime()-sec;
		
		if(delta>16)
		{
			sec=timeGetTime();
			if(dwState2>=40)
			{
				dwState2=0;
				Judgement2p=NONE;
			}
			else
			{
				if(delta >16 && delta <32)dwState2+=2;
				else if(delta >=32)dwState2+=4;
			}
		}
	}

	switch(Judgement2p)
	{
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.top=0;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.top=JUDGE_SIZE_Y;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*2;
			if(dwState2==0)dwState2++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.top=JUDGE_SIZE_Y*2;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*3;
			if(dwState2==0)dwState2++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.top=JUDGE_SIZE_Y*3;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*4;
			if(dwState2==0)dwState2++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.top=JUDGE_SIZE_Y*4;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*5;
			if(dwState2==0)dwState2++;
			break;
	}

	if(dwState2>15)
	{
		destRect.top=200;
		destRect.left=350;
		destRect.right=350+JUDGE_SIZE_X;
		destRect.bottom=200+JUDGE_SIZE_Y;
	}
	else
	{
		destRect.top=200-30+(dwState2*2);
		destRect.left=350-60+(dwState2*4);
		destRect.right=350+JUDGE_SIZE_X+60-(dwState2*4);
		destRect.bottom=200+JUDGE_SIZE_Y+30-(dwState2*2);
	}

	if(Judgement2p)
	{
//		g_pDDSBack->BltFast(400,200+dwState22/2,JudgeFont,&rRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

		g_pDDSBack->Blt(&destRect, JudgeFont, &rRect,DDBLT_WAIT | DDBLT_KEYSRC , NULL);

		/* 콤보 출력부 입니다. */
		if((Judgement2p==PERFECT || Judgement2p==GREAT) && Combo2p>3)
		{
			sprintf(chCombo2p,"%03d",Combo2p);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo2p[Loop]==NULL)break;
				chCombo2p[Loop]-=48;
				cRect.left=chCombo2p[Loop]*50;
				cRect.right=cRect.left+50;
				cRect.top=0;
				cRect.bottom=65;
//				g_pDDSBack->BltFast(380+Loop*30+dwState22*2,250,NumberFont,&cRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				if(dwState2>10)g_pDDSBack->BltFast(400+Loop*50,250+dwState2*2-dwState2*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(400+Loop*50,250+dwState2*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);

				cRect.left=0;
				cRect.right=150;
				cRect.top=65;
				cRect.bottom=100;
				
				if(dwState2>10)g_pDDSBack->BltFast(400,320+dwState2*2-dwState2*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(400,320+dwState2*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
			}/* 여기까지 */
		}
	}

	Judgement2p=NONE;
}


void DrawArrow1p(DWORD cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720};

	static int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};

	static BYTE s1,s3,s5,s7,s9;
	static DWORD stat1,stat3,stat5,stat7,stat9;
	static DWORD cur2;
	static int beat;

	static BOOL Crash1, Crash3, Crash5, Crash7, Crash9;
	static BOOL	On1, On3, On5, On7, On9;

	static RECT pArr1,pArr3,pArr5,pArr7,pArr9;
	static RECT cArr1,cArr3,cArr5,cArr7,cArr9;


	BYTE JudgeTemp=0;
	BYTE	count;

	if(cur2!=cur)
	{
		if(bModeRandomS1p == TRUE)
		{
			HighSpeed1p_1 = HighSpeed1p_3 = HighSpeed1p_5 = HighSpeed1p_7 = HighSpeed1p_9 = 1 + rand() % 8 ;
		}
		cur2=cur;
		beat=4;
	}
	else 
	{
		beat--;
		if(beat<=0)beat=0;
	}

	ReadGameInput();
// 오토 버튼 지원 부분 
	if(KCFG.auto1_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][0]=='1')
				{
					Data_Judge[cur+count][0]='0';
					stat1=cur+count;
					s1=1;
					Crash1=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][1]=='1')
				{
					Data_Judge[cur+count][1]='0';
					stat7=cur+count;
					s7=1;
					Crash7=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][2]=='1')
				{
					Data_Judge[cur+count][2]='0';
					stat5=cur+count;
					s5=1;
					Crash5=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][3]=='1')
				{
					Data_Judge[cur+count][3]='0';
					stat9=cur+count;
					s9=1;
					Crash9=TRUE;
					break;
				}
			}
		}
	}

	if(KCFG.auto3_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][4]=='1')
				{
					Data_Judge[cur+count][4]='0';
					stat3=cur+count;
					s3=1;
					Crash3=TRUE;
					break;
				}
			}
			
		}
	}

	if(s1 || (PressedKey1p[1]==TRUE) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=FALSE;
		}
		else
		{
			s1++;
		}
		if(PressedKey1p[1]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed1p_1 < Data_y[cur+count] && 
				ZONE_D*HighSpeed1p_1 > Data_y[cur+count]  )
			{
				if( PERFECT_ZONE_U*HighSpeed1p_1 < Data_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed1p_1 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed1p_1 < Data_y[cur+count] && 
						 GREAT_ZONE_D*HighSpeed1p_1 > Data_y[cur+count]  )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed1p_1 < Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed1p_1 > Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed1p_1 < Data_y[cur+count] &&
						 BAD_ZONE_D*HighSpeed1p_1 > Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s3 || (PressedKey1p[3]==TRUE))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=FALSE;
		}
		else
		{
			s3++;
		}
		if(PressedKey1p[3]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed1p_3 < Data_y[cur+count] &&
				ZONE_D*HighSpeed1p_3 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed1p_3 < Data_y[cur+count] &&
				    PERFECT_ZONE_D*HighSpeed1p_3 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed1p_3<Data_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed1p_3>Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed1p_3 < Data_y[cur+count] &&
						 GOOD_ZONE_D*HighSpeed1p_3 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed1p_3<Data_y[cur+count] &&
						 BAD_ZONE_D*HighSpeed1p_3>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s5 || (PressedKey1p[5]==TRUE))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=FALSE;
		}
		else
		{
			s5++;
		}
		if(PressedKey1p[5]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed1p_5<Data_y[cur+count] &&
				ZONE_D*HighSpeed1p_5>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed1p_5<Data_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed1p_5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed1p_5<Data_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed1p_5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed1p_5<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed1p_5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed1p_5<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed1p_5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
				
			} 
		}

	}

	if(s7 || (PressedKey1p[7]==TRUE) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=FALSE;
		}
		else
		{
			s7++;
		}
		if(PressedKey1p[7]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed1p_7 < Data_y[cur+count] &&
				ZONE_D*HighSpeed1p_7 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed1p_7 < Data_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed1p_7 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed1p_7<Data_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed1p_7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed1p_7<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed1p_7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed1p_7<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed1p_7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}

			}
		}
	}

	if(s9 || (PressedKey1p[9]==TRUE))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=FALSE;
		}
		else
		{
			s9++;
		}
		if(PressedKey1p[9]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed1p_9<Data_y[cur+count] &&
				ZONE_D*HighSpeed1p_9>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed1p_9<Data_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed1p_9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed1p_9<Data_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed1p_9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed1p_9<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed1p_9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed1p_9<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed1p_9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}

	}
	
	Judgement1p=JudgeTemp;
	
	// 미스처리입니다.
	for(count=0;count<10;count++)
	if(Data_y[cur+count] < ZONE_U
		&& (Data_Judge[cur+count][0]!='0' || Data_Judge[cur+count][1]!='0' || Data_Judge[cur+count][2]!='0' || Data_Judge[cur+count][3]!='0' || Data_Judge[cur+count][4]!='0')
		&& (Data[cur+count][0]!='0' || Data[cur+count][1]!='0' || Data[cur+count][2]!='0' || Data[cur+count][3]!='0' || Data[cur+count][4]!='0' ))
	{
		if(Data[cur+count][0]=='2')break;
		Data_Judge[cur+count][0]=Data_Judge[cur+count][1]=Data_Judge[cur+count][2]=Data_Judge[cur+count][3]=Data_Judge[cur+count][4]='0';
		Judgement1p=MISS;
		Combo1p=0;
	}


	if(Crash1)
	{
		if(s1==2)if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
		{
			Data[stat1][0]=Data[stat1][1]=Data[stat1][2]=Data[stat1][3]=Data[stat1][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		}
		else Judgement1p=NONE;
	}
	if(Crash7)
	{
		if(s7==2)if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
		{
			Data[stat7][0]=Data[stat7][1]=Data[stat7][2]=Data[stat7][3]=Data[stat7][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		}
		else Judgement1p=NONE;
	}
	if(Crash5)
	{
		if(s5==2)if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
		{
			Data[stat5][0]=Data[stat5][1]=Data[stat5][2]=Data[stat5][3]=Data[stat5][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		}
		else Judgement1p=NONE;
	}
	if(Crash9)
	{
		if(s9==2)if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
		{
			Data[stat9][0]=Data[stat9][1]=Data[stat9][2]=Data[stat9][3]=Data[stat9][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		}
		else Judgement1p=NONE;
	}
	if(Crash3)
	{
		if(s3==2)if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
		{
			Data[stat3][0]=Data[stat3][1]=Data[stat3][2]=Data[stat3][3]=Data[stat3][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		}
		else Judgement1p=NONE;
	}

	if(Judgement1p==PERFECT || Judgement1p==GREAT)
	{
		if(g_dsBeat)
		{
			g_dsBeat->Stop();
			g_dsBeat->SetCurrentPosition(0);
			g_dsBeat->Play(0,0,0);
		}
		if(Judgement1p==PERFECT)cPerfect1p++;
		if(Judgement1p==GREAT)cGreat1p++;

		Combo1p++;
		if(Combo1p>cMaxCombo1p)cMaxCombo1p=Combo1p;
		if(Combo1p>10)
		{
			if(Gauge1p<0)Gauge1p=1;
			else Gauge1p++;
			if(Gauge1p>42)Gauge1p=41;
		}
		Crash1=On1;
		Crash3=On3;
		Crash5=On5;
		Crash7=On7;
		Crash9=On9;

		On1=On3=On5=On7=On9=FALSE;
	}
	else if(Judgement1p==GOOD || Judgement1p==BAD || Judgement1p==MISS)
	{
		if(Judgement1p==GOOD)cGood1p++;
		else if(Judgement1p==BAD)cBad1p++;
		else if(Judgement1p==MISS)cMiss1p++;
		Combo1p=0;
	}

	if(Combo1p)
	{
		if(Judgement1p==PERFECT)Score1p+=2000;
		else if(Judgement1p==GREAT)Score1p+=1500;
	}
	else
	{
		if(Judgement1p==PERFECT)Score1p+=1000;
		else if(Judgement1p==GREAT)Score1p+=500;
	}

	if(Judgement1p==GOOD)Score1p+=100;
	else if(Judgement1p==BAD)
	{
		Score1p-=700;
		Gauge1p-=2;
	}
	else if(Judgement1p==MISS)
	{
		Score1p-=1000;
		Gauge1p-=5;
	}

	if(KCFG.bcDead)
	{
		if(Couple)
		{
			if(Gauge1p<-40 && Gauge2p<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
		else
		{
			if(Gauge1p<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(Score1p<0)Score1p=0;

	if (beat) g_pDDSBack->BltFast(32,50,Arrow2,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else g_pDDSBack->BltFast(32,50,Arrow1,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	pArr1.top=0;
	pArr1.left=arrow_l[s1];
	pArr1.right=arrow_r[s1];
	pArr1.bottom=70;

	pArr3.top=0;
	pArr3.left=arrow_l[s3];
	pArr3.right=arrow_r[s3];
	pArr3.bottom=70;

	pArr5.top=0;
	pArr5.left=arrow_l[s5];
	pArr5.right=arrow_r[s5];
	pArr5.bottom=70;

	pArr7.top=0;
	pArr7.left=arrow_l[s7];
	pArr7.right=arrow_r[s7];
	pArr7.bottom=70;

	pArr9.top=0;
	pArr9.left=arrow_l[s9];
	pArr9.right=arrow_r[s9];
	pArr9.bottom=70;

	cArr1.top=0;
	cArr1.left=Carrow_l[s1];
	cArr1.right=Carrow_r[s1];
	cArr1.bottom=80;

	cArr3.top=0;
	cArr3.left=Carrow_l[s3];
	cArr3.right=Carrow_r[s3];
	cArr3.bottom=80;

	cArr5.top=0;
	cArr5.left=Carrow_l[s5];
	cArr5.right=Carrow_r[s5];
	cArr5.bottom=80;

	cArr7.top=0;
	cArr7.left=Carrow_l[s7];
	cArr7.right=Carrow_r[s7];
	cArr7.bottom=80;

	cArr9.top=0;
	cArr9.left=Carrow_l[s9];
	cArr9.right=Carrow_r[s9];
	cArr9.bottom=80;

	if(Crash1)g_pDDSBack->BltFast(25,43,cArrow1,&cArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s1)g_pDDSBack->BltFast(27,45,pArrow1,&pArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash7)g_pDDSBack->BltFast(75,43,cArrow7,&cArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s7)g_pDDSBack->BltFast(77,45,pArrow7,&pArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash5)g_pDDSBack->BltFast(125,43,cArrow5,&cArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s5)g_pDDSBack->BltFast(127,45,pArrow5,&pArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	if(Crash9)g_pDDSBack->BltFast(175,43,cArrow9,&cArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s9)g_pDDSBack->BltFast(177,45,pArrow9,&pArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash3)g_pDDSBack->BltFast(225,43,cArrow3,&cArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s3)g_pDDSBack->BltFast(227,45,pArrow3,&pArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void DrawArrow2p(DWORD cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720};

	static int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};

	static BYTE s1,s3,s5,s7,s9;
	static DWORD stat1,stat3,stat5,stat7,stat9;
	static DWORD cur2;
	static int beat;

	static BOOL Crash1, Crash3, Crash5, Crash7, Crash9;
	static BOOL	On1, On3, On5, On7, On9;

	static RECT pArr1,pArr3,pArr5,pArr7,pArr9;
	static RECT cArr1,cArr3,cArr5,cArr7,cArr9;


	BYTE JudgeTemp=0;
	BYTE	count;

	
	if(Start2p==TRUE && Start1p==FALSE)ReadGameInput();
	if(cur2!=cur)
	{
		if(bModeRandomS2p == TRUE)
		{
			HighSpeed2p_1 = HighSpeed2p_3 = HighSpeed2p_5 = HighSpeed2p_7 = HighSpeed2p_9 = 1 + rand() % 8 ;
		}
		cur2=cur;
		beat=4;
	}
	else 
	{
		beat--;
		if(beat<=0)beat=0;
	}
// 오토 버튼 지원 부분 
	if(KCFG.auto1_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][5]=='1')
				{
					Data_Judge1[cur+count][5]='0';
					stat1=cur+count;
					s1=1;
					Crash1=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][6]=='1')
				{
					Data_Judge1[cur+count][6]='0';
					stat7=cur+count;
					s7=1;
					Crash7=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][7]=='1')
				{
					Data_Judge1[cur+count][7]='0';
					stat5=cur+count;
					s5=1;
					Crash5=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][8]=='1')
				{
					Data_Judge1[cur+count][8]='0';
					stat9=cur+count;
					s9=1;
					Crash9=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto3_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][9]=='1')
				{
					Data_Judge1[cur+count][9]='0';
					stat3=cur+count;
					s3=1;
					Crash3=TRUE;
					break;
				}
			}
			
		}
	}

	if(s1 || (PressedKey2p[1]==TRUE) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=FALSE;
		}
		else
		{
			s1++;
		}
		if(PressedKey2p[1]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed2p_1 < Data_y1[cur+count] && 
				ZONE_D*HighSpeed2p_1 > Data_y1[cur+count]  )
			{
				if( PERFECT_ZONE_U*HighSpeed2p_1 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*HighSpeed2p_1 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed2p_1 < Data_y1[cur+count] && 
						 GREAT_ZONE_D*HighSpeed2p_1 > Data_y1[cur+count]  )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed2p_1 < Data_y1[cur+count] &&
					     GOOD_ZONE_D*HighSpeed2p_1 > Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed2p_1 < Data_y1[cur+count] &&
						 BAD_ZONE_D*HighSpeed2p_1 > Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s3 || (PressedKey2p[3]==TRUE))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=FALSE;
		}
		else
		{
			s3++;
		}
		if(PressedKey2p[3]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed2p_3 < Data_y1[cur+count] &&
				ZONE_D*HighSpeed2p_3 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed2p_3 < Data_y1[cur+count] &&
				    PERFECT_ZONE_D*HighSpeed2p_3 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed2p_3<Data_y1[cur+count] &&
					     GREAT_ZONE_D*HighSpeed2p_3>Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed2p_3 < Data_y1[cur+count] &&
						 GOOD_ZONE_D*HighSpeed2p_3 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed2p_3<Data_y1[cur+count] &&
						 BAD_ZONE_D*HighSpeed2p_3>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s5 || (PressedKey2p[5]==TRUE))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=FALSE;
		}
		else
		{
			s5++;
		}
		if(PressedKey2p[5]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed2p_5<Data_y1[cur+count] &&
				ZONE_D*HighSpeed2p_5>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed2p_5<Data_y1[cur+count] &&
					PERFECT_ZONE_D*HighSpeed2p_5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed2p_5<Data_y1[cur+count] &&
					     GREAT_ZONE_D*HighSpeed2p_5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed2p_5<Data_y1[cur+count] &&
					     GOOD_ZONE_D*HighSpeed2p_5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed2p_5<Data_y1[cur+count] &&
					     BAD_ZONE_D*HighSpeed2p_5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
				
			} 
		}

	}

	if(s7 || (PressedKey2p[7]==TRUE) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=FALSE;
		}
		else
		{
			s7++;
		}
		if(PressedKey2p[7]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed2p_7 < Data_y1[cur+count] &&
				ZONE_D*HighSpeed2p_7 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed2p_7 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*HighSpeed2p_7 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed2p_7<Data_y1[cur+count] &&
					     GREAT_ZONE_D*HighSpeed2p_7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed2p_7<Data_y1[cur+count] &&
					     GOOD_ZONE_D*HighSpeed2p_7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed2p_7<Data_y1[cur+count] &&
					     BAD_ZONE_D*HighSpeed2p_7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}

			}
		}
	}

	if(s9 || (PressedKey2p[9]==TRUE))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=FALSE;
		}
		else
		{
			s9++;
		}
		if(PressedKey2p[9]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed2p_9<Data_y1[cur+count] &&
				ZONE_D*HighSpeed2p_9>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed2p_9<Data_y1[cur+count] &&
					PERFECT_ZONE_D*HighSpeed2p_9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed2p_9<Data_y1[cur+count] &&
					     GREAT_ZONE_D*HighSpeed2p_9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed2p_9<Data_y1[cur+count] &&
					     GOOD_ZONE_D*HighSpeed2p_9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed2p_9<Data_y1[cur+count] &&
					     BAD_ZONE_D*HighSpeed2p_9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}

	}
	
	Judgement2p=JudgeTemp;
	
	// 미스처리입니다.
	for(count=0;count<10;count++)
	if(Data_y1[cur+count] < 40
		&& (Data_Judge1[cur+count][5]!='0' || Data_Judge1[cur+count][6]!='0' || Data_Judge1[cur+count][7]!='0' || Data_Judge1[cur+count][8]!='0' || Data_Judge1[cur+count][9]!='0')
		&& (Data1[cur+count][5]!='0' || Data1[cur+count][6]!='0' || Data1[cur+count][7]!='0' || Data1[cur+count][8]!='0' || Data1[cur+count][9]!='0' ))
	{
		if(Data1[cur+count][5]=='2')break;
		Data_Judge1[cur+count][5]=Data_Judge1[cur+count][6]=Data_Judge1[cur+count][7]=Data_Judge1[cur+count][8]=Data_Judge1[cur+count][9]='0';
		Judgement2p=MISS;
		Combo2p=0;
	}

	if(Crash1)
	{
		if(s1==2)if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
		{
			Data1[stat1][5]=Data1[stat1][6]=Data1[stat1][7]=Data1[stat1][8]=Data1[stat1][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		}
		else Judgement2p=NONE;
	}
	if(Crash7)
	{
		if(s7==2)if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
		{
			Data1[stat7][5]=Data1[stat7][6]=Data1[stat7][7]=Data1[stat7][8]=Data1[stat7][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		}
		else Judgement2p=NONE;
	}
	if(Crash5)
	{
		if(s5==2)if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
		{
			Data1[stat5][5]=Data1[stat5][6]=Data1[stat5][7]=Data1[stat5][8]=Data1[stat5][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		}
		else Judgement2p=NONE;
	}
	if(Crash9)
	{
		if(s9==2)if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
		{
			Data1[stat9][5]=Data1[stat9][6]=Data1[stat9][7]=Data1[stat9][8]=Data1[stat9][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		}
		else Judgement2p=NONE;
	}
	if(Crash3)
	{
		if(s3==2)if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
		{
			Data1[stat3][5]=Data1[stat3][6]=Data1[stat3][7]=Data1[stat3][8]=Data1[stat3][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		}
		else Judgement2p=NONE;
	}

	if(Judgement2p==PERFECT || Judgement2p==GREAT)
	{
		if(g_dsBeat)
		{
			g_dsBeat->Stop();
			g_dsBeat->SetCurrentPosition(0);
			g_dsBeat->Play(0,0,0);
		}
		Combo2p++;
		if(Judgement2p==PERFECT)cPerfect2p++;
		else if(Judgement2p==GREAT)cGreat2p++;

		if(Combo2p>cMaxCombo2p)cMaxCombo2p=Combo2p;
		if(Combo2p>10)
		{
			if(Gauge2p<0)Gauge2p=1;
			else Gauge2p++;
			if(Gauge2p>42)Gauge2p=41;
		}
		Crash1=On1;
		Crash3=On3;
		Crash5=On5;
		Crash7=On7;
		Crash9=On9;

		On1=On3=On5=On7=On9=FALSE;
	}
	else if(Judgement2p==GOOD || Judgement2p==BAD || Judgement2p==MISS)
	{
		if(Judgement2p==GOOD)cGood2p++;
		else if(Judgement2p==BAD)cBad2p++;
		else if(Judgement2p==MISS)cMiss2p++;
		Combo2p=0;
	}

	if(Combo2p)
	{
		if(Judgement2p==PERFECT)Score2p+=2000;
		else if(Judgement2p==GREAT)Score2p+=1500;
	}
	else
	{
		if(Judgement2p==PERFECT)Score2p+=1000;
		else if(Judgement2p==GREAT)Score2p+=500;
	}

	if(Judgement2p==GOOD)Score2p+=100;
	else if(Judgement2p==BAD)
	{
		Score2p-=700;
		Gauge2p-=2;
	}
	else if(Judgement2p==MISS)
	{
		Score2p-=1000;
		Gauge2p-=5;
	}

	if(KCFG.bcDead)
	{
		if(Couple)
		{
			if(Gauge1p<-40 && Gauge2p<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
		else
		{
			if(Gauge2p<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(Score2p<0)Score2p=0;

	if (beat) g_pDDSBack->BltFast(352,50,Arrow2,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else g_pDDSBack->BltFast(352,50,Arrow1,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	pArr1.top=0;
	pArr1.left=arrow_l[s1];
	pArr1.right=arrow_r[s1];
	pArr1.bottom=70;

	pArr3.top=0;
	pArr3.left=arrow_l[s3];
	pArr3.right=arrow_r[s3];
	pArr3.bottom=70;

	pArr5.top=0;
	pArr5.left=arrow_l[s5];
	pArr5.right=arrow_r[s5];
	pArr5.bottom=70;

	pArr7.top=0;
	pArr7.left=arrow_l[s7];
	pArr7.right=arrow_r[s7];
	pArr7.bottom=70;

	pArr9.top=0;
	pArr9.left=arrow_l[s9];
	pArr9.right=arrow_r[s9];
	pArr9.bottom=70;

	cArr1.top=0;
	cArr1.left=Carrow_l[s1];
	cArr1.right=Carrow_r[s1];
	cArr1.bottom=80;

	cArr3.top=0;
	cArr3.left=Carrow_l[s3];
	cArr3.right=Carrow_r[s3];
	cArr3.bottom=80;

	cArr5.top=0;
	cArr5.left=Carrow_l[s5];
	cArr5.right=Carrow_r[s5];
	cArr5.bottom=80;

	cArr7.top=0;
	cArr7.left=Carrow_l[s7];
	cArr7.right=Carrow_r[s7];
	cArr7.bottom=80;

	cArr9.top=0;
	cArr9.left=Carrow_l[s9];
	cArr9.right=Carrow_r[s9];
	cArr9.bottom=80;

	if(Crash1)g_pDDSBack->BltFast(345,43,cArrow1,&cArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s1)g_pDDSBack->BltFast(347,45,pArrow1,&pArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash7)g_pDDSBack->BltFast(395,43,cArrow7,&cArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s7)g_pDDSBack->BltFast(397,45,pArrow7,&pArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash5)g_pDDSBack->BltFast(445,43,cArrow5,&cArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s5)g_pDDSBack->BltFast(447,45,pArrow5,&pArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	if(Crash9)g_pDDSBack->BltFast(495,43,cArrow9,&cArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s9)g_pDDSBack->BltFast(497,45,pArrow9,&pArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash3)g_pDDSBack->BltFast(545,43,cArrow3,&cArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s3)g_pDDSBack->BltFast(547,45,pArrow3,&pArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

HRESULT	RestoreAll(void)
{
	HRESULT	hRet;

	hRet=g_pDDSPrimary->Restore();
 
	return hRet;
}

void Flipp(void)
{
	HRESULT hRet;

	while(TRUE)
	{
		if(g_fullscreen)
			hRet=g_pDDSPrimary->Flip(NULL,DDFLIP_WAIT);
		else
			hRet = g_pDDSPrimary->Blt(&g_rcScreen, g_pDDSBack,
                                                  &g_rcViewport, DDBLT_WAIT,
                                                  NULL);

		if(hRet==DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
		{
			hRet=RestoreAll();
			if(hRet != DD_OK)
				break;
		}
		if(hRet != DDERR_WASSTILLDRAWING)
			break;
	}
}


BOOL InitDSound(HWND hWnd, int Samples, int Bits, int nChannels)
{
	HRESULT			hRet;

	hRet=DirectSoundCreate(NULL,&lpds,NULL);

	if(hRet != DD_OK)
		InitFail(hWnd,hRet,"Cannot create Dsound");

	hRet=lpds->SetCooperativeLevel(hWnd,DSSCL_NORMAL);

	if(hRet != DD_OK)
		InitFail(hWnd,hRet,"Cannot cooperative (sound)");

	DisplayMessage(0,16,"Loading Music Intro File.. please Wait...");

	return TRUE;

}

void ReleaseDSound(void)
{
	int i;

	for(i=0;i<512;i++)
	{
		if(CSONG[i].Int_Song != NULL)
		{
			CSONG[i].Int_Song->Release();
			CSONG[i].Int_Song = NULL;
		}
	}

/*	if (lpdsbd != NULL)
	{
		lpdsbd->Release();
		lpdsbd = NULL;
	}
	if (g_DJVOICE_BUFFER != NULL)
	{
		g_DJVOICE_BUFFER->Release();
		g_DJVOICE_BUFFER = NULL;
	}
	if (g_PUMPSONG_BUFFER != NULL)
	{
		g_PUMPSONG_BUFFER->Release();
		g_PUMPSONG_BUFFER = NULL;
	}
	if (g_dsOpening != NULL)
	{
		g_dsOpening->Release();
		g_dsOpening = NULL;
	}
	if (g_Select_Song != NULL)
	{
		g_Select_Song->Release();
		g_Select_Song = NULL;
	}
	if (g_Cur_Song != NULL)
	{
		g_Cur_Song->Release();
		g_Cur_Song = NULL;
	}
	if (g_dsDead != NULL)
	{
		g_dsDead->Release();
		g_dsDead = NULL;
	}
*/
	if(lpds)lpds->Release();
}

void ReleaseAllObjects(void)
{
    if (g_pDD != NULL)
    {
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;

        }
		if (GameTITLE != NULL)
		{
			GameTITLE->Release();
			GameTITLE = NULL;
		}
		if (Background != NULL)
		{
			Background->Release();
			Background = NULL;
		}
		if (SongTitle != NULL)
		{
			SongTitle->Release();
			SongTitle = NULL;
		}

		if (SongBack != NULL)
		{
			SongBack->Release();
			SongBack = NULL;
		}
		if (SelectBack != NULL)
		{
			SelectBack->Release();
			SelectBack = NULL;
		}
		if (JudgeFont != NULL)
		{
			JudgeFont->Release();
			JudgeFont = NULL;
		}
		if (NumberFont != NULL)
		{
			NumberFont->Release();
			NumberFont = NULL;
		}
		if (ComboFont != NULL)
		{
			ComboFont->Release();
			ComboFont = NULL;
		}
		if (NoDISC != NULL)
		{
			NoDISC->Release();
			NoDISC = NULL;
		}
		if (ShiftLeft != NULL)
		{
			ShiftLeft->Release();
			ShiftRight = NULL;
		}
		if (ShiftRight != NULL)
		{
			ShiftRight->Release();
			ShiftRight = NULL;
		}
		if (GaugeWaku != NULL)
		{
			GaugeWaku->Release();
			GaugeWaku = NULL;
		}
		if (Gauge != NULL)
		{
			Gauge->Release();
			Gauge = NULL;
		}
		if (SmallFont != NULL)
		{
			SmallFont->Release();
			SmallFont = NULL;
		}
		if (Arrow1 != NULL)
		{
			Arrow1->Release();
			Arrow1 = NULL;
		}
		if (Arrow2 != NULL)
		{
			Arrow2->Release();
			Arrow2 = NULL;
		}
		if (wArrow != NULL)
		{
			wArrow->Release();
			wArrow = NULL;
		}
		if (pArrow1 != NULL)
		{
			pArrow1->Release();
			pArrow1 = NULL;
		}
		if (pArrow3 != NULL)
		{
			pArrow3->Release();
			pArrow3 = NULL;
		}
		if (pArrow5 != NULL)
		{
			pArrow5->Release();
			pArrow5 = NULL;
		}
		if (pArrow7 != NULL)
		{
			pArrow7->Release();
			pArrow7 = NULL;
		}
		if (pArrow9 != NULL)
		{
			pArrow9->Release();
			pArrow9 = NULL;
		}
		if(	cArrow1	!= NULL)
		{
			cArrow1->Release();
			cArrow1 = NULL;
		}
		if(	cArrow3	!= NULL)
		{
			cArrow3->Release();
			cArrow3 = NULL;
		}
		if(	cArrow5	!= NULL)
		{
			cArrow5->Release();
			cArrow5 = NULL;
		}
		if(	cArrow7	!= NULL)
		{
			cArrow7->Release();
			cArrow7 = NULL;
		}
		if(	cArrow9	!= NULL)
		{
			cArrow9->Release();
			cArrow9 = NULL;
		}
		if(	ModeIcon != NULL)
		{
			ModeIcon->Release();
			ModeIcon = NULL;
		}
		if( g_cFont != NULL)
		{
			g_cFont->Release();
			g_cFont = NULL;
		}
		if( ResultFont != NULL)
		{
			ResultFont->Release();
			ResultFont = NULL;
		}
		if( ResultBack != NULL)
		{
			ResultBack->Release();
			ResultBack = NULL;
		}
		if( StageCount != NULL)
		{
			StageCount->Release();
			StageCount = NULL;
		}
		if( Score != NULL)
		{
			Score->Release();
			Score = NULL;
		}
		if( DeadScreen != NULL)
		{
			DeadScreen->Release();
			DeadScreen = NULL;
		}
		if( GameOver != NULL)
		{
			GameOver->Release();
			GameOver=NULL;
		}
		if( Logo != NULL)
		{
			Logo->Release();
			Logo=NULL;
		}
		if( Diff != NULL)
		{
			Diff->Release();
			Diff=NULL;
		}
		if( DoubleIcon != NULL)
		{
			DoubleIcon->Release();
			DoubleIcon=NULL;
		}
		if( CrazyIcon != NULL)
		{
			CrazyIcon->Release();
			CrazyIcon=NULL;
		}
		if( EasyIcon != NULL)
		{
			EasyIcon->Release();
			EasyIcon=NULL;
		}
		if( HardIcon != NULL)
		{
			HardIcon->Release();
			HardIcon=NULL;
		}
        g_pDD->Release();
        g_pDD = NULL;

		ReleaseDSound();
		CleanupInput();
	}
}

void UpdateFrame(void)
{
	// FPS count start
	static DWORD lastTime, fpsTime,framesRendered,fps;

	char	buff[50];
	DWORD	cur = timeGetTime();
	DWORD	deltaTime = cur - lastTime;
	lastTime = cur;

	fpsTime += deltaTime;

	++framesRendered;

	if(fpsTime>1000)
	{
		fps = framesRendered;
		framesRendered = 0;
		fpsTime = 0;
	}

	// FPS count & print end
	sprintf(buff,"FPS:%3d",fps);
	DisplayMessage(583,463,buff);

	switch(g_ProgramState)
	{
		case GAMETITLE:
			StageTitle();
			break;
		case SELECTSONG:
			SelectSong();
			break;
		case STAGE1:
			KIU_STAGE();
			break;
		case DOUBLE:
			KIU_STAGE_DOUBLE();
			break;
		case COUPLE:
			KIU_STAGE();
			break;
		case DEAD:
			Dead();
			break;
		case CONFIG:
			Configuration();
			break;
		case RESULT:
			Result();
			break;
		case GAMEOVER:
			GameOver1();
			break;
		case END:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
	}

}

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
			case WM_MOVE:
				if(!g_fullscreen)
				{
					GetClientRect(hWnd, &g_rcViewport);
					GetClientRect(hWnd, &g_rcScreen);
					ClientToScreen(hWnd, (POINT*)&g_rcScreen.left);
					ClientToScreen(hWnd, (POINT*)&g_rcScreen.right);
				}
				break;
			/*case WM_PAINT:
				UpdateFrame();
				break;*/

			case WM_ACTIVATEAPP:
				g_bActive=wParam;
				break;

			case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
				case VK_F12:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0L;
/* SelectSong, StageTitle에서 참조합니다.
   추후에 바꿔주어야 합니다.
   */
				case VK_F2:
					First=0;
					switch(g_ProgramState)
					{
						case GAMETITLE:
							First=0;
							if(g_dsOpening)g_dsOpening->Stop();
							g_ProgramState=CONFIG;

							break;

						case SELECTSONG:
							g_ProgramState=CONFIG;
							if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
							if(g_dsSelectSong)g_dsSelectSong->Stop();
							First=0;
							break;

						case STAGE1:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case DOUBLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case COUPLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case CONFIG:
							g_ProgramState=CONFIG;
							break;
					}
					break;

				case VK_UP:
					switch(g_ProgramState)
					{
						case GAMETITLE:
							PressedKey2p[0]=8;
							break;
						case SELECTSONG:
							PressedKey2p[0]=8;
							break;
						case CONFIG:
							PressedKey2p[0]=8;
							break;
					}
					break;
				case VK_DOWN:
					switch(g_ProgramState)
					{
						case GAMETITLE:
							PressedKey2p[0]=2;
							break;
						case SELECTSONG:
							PressedKey2p[0]=2;
						case CONFIG:
							PressedKey2p[0]=2;
							break;
					}
					break;

				case VK_LEFT:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							PressedKey2p[0]=4;
							break;
						case CONFIG:
							PressedKey2p[0]=4;
							break;
					}
					break;
				case VK_RIGHT:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							PressedKey2p[0]=6;
							break;
						case CONFIG:
							PressedKey2p[0]=6;
							break;
					}
					break;

				case VK_RETURN:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							PressedKey2p[0]=3;
							break;
						case CONFIG:
							PressedKey2p[0]=3;
							break;
					}
					break;

/* 여기까지 입니다. */

				case VK_ESCAPE:
				{
					switch(g_ProgramState)
					{
						case GAMETITLE:
							CFGWrite();
							PostMessage(hWnd, WM_CLOSE, 0, 0);
							return 0L;
							break;

						case SELECTSONG:
							g_ProgramState=GAMETITLE;
							if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
							if(g_dsSelectSong)g_dsSelectSong->Stop();
							First=0;
							break;

						case STAGE1:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case DOUBLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case COUPLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case CONFIG:
							First=0;
							CFGWrite();
							g_ProgramState=GAMETITLE;
							break;
						case RESULT:
							First=0;
							g_ProgramState=SELECTSONG;
							break;
					}
				}

			}
            break;

			case WM_DESTROY:
				ReleaseAllObjects();
				PostQuitMessage(0);
				return 0L;
			case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
	            SetCursor(NULL);
            return TRUE;

    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
	char                        szBuff[128];
	va_list                     vl;

	va_start(vl, szError);
	vsprintf(szBuff, szError, vl);
	ReleaseAllObjects();
	MessageBox(hWnd, szBuff, TITLE, MB_OK);
	DestroyWindow(hWnd);
	va_end(vl);
	return hRet;
}

HRESULT	InitWin(HINSTANCE hInstance, DWORD Width, DWORD Height, int nCmdShow)
{
	WNDCLASS	wc;

	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style

	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)Width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)Height;		// Set Bottom Value To Requested Height

// Detect os version (for title bar)
	OSVERSIONINFO osver;

	osver.dwOSVersionInfoSize=sizeof(osver);
	GetVersionEx(&osver);
// Os ver detect finished.

	// Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(0, MAKEINTRESOURCE(IDI_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = PRGNAME;
    wc.lpszClassName = PRGNAME;

    RegisterClass(&wc);

	if (g_fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_POPUP | WS_SYSMENU | WS_CAPTION;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	sprintf(TITLE, "KICK IT UP! beta version v%s - Minor release / Compiled at %s / %s / Windows %d.%d.%d" ,VER_NUM, __DATE__, __TIME__, osver.dwMajorVersion, osver.dwMinorVersion, LOWORD(osver.dwBuildNumber));

    // Create a window
    hWnd = CreateWindowEx(dwExStyle ,	
							PRGNAME,
							TITLE,
							dwStyle,
							0, 0,								// Window Position
							WindowRect.right-WindowRect.left,	// Calculate Window Width
							WindowRect.bottom-WindowRect.top,	// Calculate Window Height
							NULL,
							NULL,
							hInstance,
							NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

	g_hInst=hInstance;
	return DD_OK;

}

HRESULT InitDD(void)
{
	DDSURFACEDESC			ddsd;
	DDSCAPS					ddscaps;

	HRESULT					hRet;

	LPDIRECTDRAWCLIPPER		pClipper;

	hRet=DirectDrawCreate(NULL,&g_pDD,NULL);
	if(hRet !=DD_OK)
		return	InitFail(hWnd,hRet,"Cannot Creat DirectDraw");

	if(g_fullscreen)
	{
		hRet=g_pDD->SetCooperativeLevel(hWnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT);
		if(hRet !=DD_OK)
			return InitFail(hWnd,hRet,"Cannot set cooperative");

		hRet=g_pDD->SetDisplayMode(640,480,16);
		if(hRet !=DD_OK)
			return InitFail(hWnd,hRet,"Cannot set display mode");

		// Create the primary surface
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; 
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							  DDSCAPS_FLIP |
							  DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 2;

		g_pDD->CreateSurface(&ddsd,&g_pDDSPrimary,NULL);

		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;

		// Get a pointer to the back buffer
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		hRet=g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
		if(hRet != DD_OK)
		{
			return InitFail(hWnd,hRet,"Cannot Attached surface");
		}
	}
	else
	{
		hRet=g_pDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL);
		if(hRet !=DD_OK)
			return InitFail(hWnd,hRet,"Cannot set cooperative");

    	GetClientRect(hWnd, &g_rcViewport);
    	GetClientRect(hWnd, &g_rcScreen);
    	ClientToScreen(hWnd, (POINT*)&g_rcScreen.left);
    	ClientToScreen(hWnd, (POINT*)&g_rcScreen.right);

		// Create the primary surface
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS ; 
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		g_pDD->CreateSurface(&ddsd,&g_pDDSPrimary,NULL);

		hRet=g_pDD->CreateClipper(0, &pClipper, NULL);
		
		if(hRet != DD_OK)
		return InitFail(hWnd,hRet,"CreateClipper() : FAILED");

		pClipper->SetHWnd(0, hWnd);
//	MessageBox(hWnd, "hi","hi", MB_OK);

		g_pDDSPrimary->SetClipper(pClipper);

		pClipper->Release();
		pClipper=NULL;

        ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        ddsd.dwWidth        = 640;
        ddsd.dwHeight       = 480;
		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;

		// Get a pointer to the back buffer
		//ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
        hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBack, NULL);
		if(hRet != DD_OK)
		{
			return InitFail(hWnd,hRet,"Cannot Attached surface");
		}
	}
	return	hRet;
}

HRESULT KLoadImage(void)
{
	// Loading image here
	
	SmallFont=DDLoadBitmap(g_pDD,"images\\sfont.bmp",0,0);
	if(SmallFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(SmallFont,CLR_INVALID);

	DisplayMessage(0,0,"Loading Image");

	GameTITLE = DDLoadBitmap(g_pDD,"images\\Title.bmp",0,0);
	if(GameTITLE == NULL)
	{
		MessageBox(hWnd,"Cannot Load GAMETITLE","ERROR",0);
		return FALSE;
	}

	Background = DDLoadBitmap(g_pDD,"images\\back.bmp",0,0);
	if(Background==NULL)
	{
		MessageBox(hWnd,"Cannot Load Background.","ERROR",0);
		return FALSE;
	}

	SelectBack = DDLoadBitmap(g_pDD,"images\\SelectBack.bmp",0,0);
	if(SelectBack==NULL)
	{
		MessageBox(hWnd,"Cannot Load Select Background.","ERROR",0);
		return FALSE;
	}

	JudgeFont = DDLoadBitmap(g_pDD,"images\\Judgement.bmp",0,0);
	if(JudgeFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Judgement Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(JudgeFont,CLR_INVALID);
	
	NumberFont = DDLoadBitmap(g_pDD,"images\\Number.bmp",0,0);
	if(NumberFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Number Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(NumberFont,CLR_INVALID);

	ComboFont = DDLoadBitmap(g_pDD,"images\\Combo.bmp",0,0);
	if(ComboFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Combo Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(ComboFont,CLR_INVALID);

	NoDISC=DDLoadBitmap(g_pDD,"images\\nodisc.bmp",0,0);
	if(NoDISC==NULL)
	{
		MessageBox(hWnd,"Cannot Load NoDISC.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(NoDISC,CLR_INVALID);

	ShiftLeft=DDLoadBitmap(g_pDD,"images\\Shiftl.bmp",0,0);
	if(ShiftLeft==NULL)
	{
		MessageBox(hWnd,"Cannot Load Shiftl.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(ShiftLeft,CLR_INVALID);

	ShiftRight=DDLoadBitmap(g_pDD,"images\\Shiftr.bmp",0,0);
	if(ShiftRight==NULL)
	{
		MessageBox(hWnd,"Cannot Load Shiftr.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(ShiftRight,CLR_INVALID);

	GaugeWaku=DDLoadBitmap(g_pDD,"images\\GaugeWaku.bmp",0,0);
	if(GaugeWaku==NULL)
	{
		MessageBox(hWnd,"Cannot Load GaugeWaku.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(GaugeWaku,CLR_INVALID);

	Gauge=DDLoadBitmap(g_pDD,"images\\Gauge.bmp",0,0);
	if(Gauge==NULL)
	{
		MessageBox(hWnd,"Cannot Load Gauge.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Gauge,CLR_INVALID);
	
	Arrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_ARROW1),0,0);
	if(Arrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load Arrow1.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Arrow1,CLR_INVALID);
	Arrow2=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_ARROW2),0,0);
	if(Arrow2==NULL)
	{
		MessageBox(hWnd,"Cannot Load arrow2.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Arrow2,CLR_INVALID);

	wArrow=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(ID_ARROW),0,0);
	if(wArrow==NULL)
	{
		MessageBox(hWnd,"Cannot Load arrow.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(wArrow,CLR_INVALID);
	CKey_Arr=DDColorMatch(wArrow,CLR_INVALID);

	pArrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW1),0,0);
	if(pArrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow1,CLR_INVALID);

	pArrow3=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW3),0,0);
	if(pArrow3==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow3,CLR_INVALID);

	pArrow5=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW5),0,0);
	if(pArrow5==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow5.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow5,CLR_INVALID);

	pArrow7=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW7),0,0);
	if(pArrow7==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow7.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow7,CLR_INVALID);
	pArrow9=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW9),0,0);
	if(pArrow9==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow9.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow9,CLR_INVALID);

	cArrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW1),0,0);
	if(cArrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow1,CLR_INVALID);
	cArrow3=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW3),0,0);
	if(cArrow3==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow3,CLR_INVALID);

	cArrow5=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW5),0,0);
	if(cArrow5==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow5.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow5,CLR_INVALID);

	cArrow7=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW7),0,0);
	if(cArrow7==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow7.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow7,CLR_INVALID);
	cArrow9=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW9),0,0);
	if(cArrow9==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow9.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow9,CLR_INVALID);
	ModeIcon=DDLoadBitmap(g_pDD, "images\\ModeIcon.bmp",0,0);
	if(ModeIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load ModeIcon.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(ModeIcon, CLR_INVALID);
	g_cFont=DDLoadBitmap(g_pDD, "images\\CFont.bmp",0,0);
	if(g_cFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load CFont.bmp",0,0);
		return	FALSE;
	}
	DDSetColorKey(g_cFont, CLR_INVALID);
	CKey_CFont=DDColorMatch(g_cFont,CLR_INVALID);

	ResultFont=DDLoadBitmap(g_pDD, "images\\ResFont.bmp",0,0);
	if(ResultFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load ResFont.bmp",0,0);
		return	FALSE;
	}
	DDSetColorKey(ResultFont, CLR_INVALID);
	ResultBack=DDLoadBitmap(g_pDD, "images\\ResBack.bmp",0,0);
	if(ResultBack==NULL)
	{
		MessageBox(hWnd,"Cannot Load ResBack.bmp",0,0);
		return FALSE;
	}
	StageCount=DDLoadBitmap(g_pDD, "images\\StageCount.bmp",0,0);
	if(StageCount==NULL)
	{
		MessageBox(hWnd,"Cannot Load StageCount.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(StageCount, CLR_INVALID);

	Score=DDLoadBitmap(g_pDD,"images\\Score.bmp",0,0);
	if(Score==NULL)
	{
		MessageBox(hWnd,"Cannot Load Score.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Score,CLR_INVALID);

	DeadScreen=DDLoadBitmap(g_pDD,"images\\Dead.bmp",0,0);
	if(DeadScreen==NULL)
	{
		MessageBox(hWnd,"Cannot Load DeadScreen.bmp",0,0);
		return FALSE;
	}
	GameOver=DDLoadBitmap(g_pDD,"images\\GameOver.bmp",0,0);
	if(GameOver==NULL)
	{
		MessageBox(hWnd,"Cannot Load GameOver.bmp",0,0);
		return FALSE;
	}
	Logo=DDLoadBitmap(g_pDD,"images\\Logo.bmp",0,0);
	if(Logo==NULL)
	{
		MessageBox(hWnd,"Cannot Load Logo.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Logo,CLR_INVALID);
	Diff=DDLoadBitmap(g_pDD,"images\\Diff.bmp",0,0);
	if(Diff==NULL)
	{
		MessageBox(hWnd,"Cannot Load Diff.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(Diff,CLR_INVALID);
	DoubleIcon=DDLoadBitmap(g_pDD,"images\\DoubleIcon.bmp",0,0);
	if(DoubleIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load DoubleIcon.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(DoubleIcon,CLR_INVALID);
	CrazyIcon=DDLoadBitmap(g_pDD,"images\\CrazyIcon.bmp",0,0);
	if(CrazyIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load CrazyIcon.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(CrazyIcon,CLR_INVALID);
	EasyIcon=DDLoadBitmap(g_pDD,"images\\EasyIcon.bmp",0,0);
	if(EasyIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load EasyIcon.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(EasyIcon,CLR_INVALID);
	HardIcon=DDLoadBitmap(g_pDD,"images\\HardIcon.bmp",0,0);
	if(HardIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load HardIcon.bmp",0,0);
		return FALSE;
	}
	DDSetColorKey(HardIcon,CLR_INVALID);

	return DD_OK;
}

int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
    MSG							msg;

// Debug File name definition
	time_t	ltime;
	struct tm *today;

	time(&ltime);
	today=localtime(&ltime);
	sprintf(g_szDebugName,"%d%d%d.txt",today->tm_year+1900, today->tm_mon+1, today->tm_mday);
// define end :)

	CoInitialize(NULL);
	
	if(FAILED(InitWin(hInstance, 640, 480, nCmdShow)))return FALSE;

	if(FAILED(InitDD()))return FALSE;
	if(FAILED(KLoadImage()))return FALSE;

	Read();
 	if(FAILED(InitDSound(hWnd,22050,8,2)))return FALSE;
	
	WaveSet_Loading();
 	if(FAILED(InitDI(hInstance)))return FALSE;

	CFGInitialize();

	while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive)
        {
            UpdateFrame();
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }

}

HRESULT TransAlphaImproved(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest, 
				   LONG lDestX, LONG lDestY, RECT srcRect, WORD ALPHA, DWORD ColorKey, WORD BPP)
{
	int register i,j;
	int height,width;
	BYTE* lpSprite;
	BYTE* lpDest;
	WORD dPitch, SpritePitch;
	DWORD sColorKey;
	DWORD sTemp,dTemp;
	DWORD sr,sg,sb,dr,dg,db;
	WORD sbuf,dbuf;
	DWORD Result;
	BOOL oddWidth = FALSE;
	DDSURFACEDESC srcDDSD, destDDSD;
	DWORD REDC,GREENC, BLUEC;
	DWORD PLUS64;
	DWORD ALPHABY4;
	DWORD doubleColorKey;


	// Check the ALPHA value
	if (ALPHA < 0)
		ALPHA = 0;
	else if (ALPHA > 256)
		ALPHA = 256;

	// Set height and width of SPRITE
    height = srcRect.bottom - srcRect.top;
    width = srcRect.right - srcRect.left; 

	// Lock down both surfaces for read and write
	memset(&srcDDSD, 0, sizeof(srcDDSD));
	srcDDSD.dwSize = sizeof(srcDDSD);
	src->Lock(NULL, &srcDDSD, DDLOCK_WAIT, NULL);
    
	memset(&destDDSD, 0, sizeof(destDDSD));
	destDDSD.dwSize = sizeof(destDDSD);
	dest->Lock(NULL, &destDDSD, DDLOCK_WAIT, NULL);

    // Get the color key for sprite surface
    sColorKey = ColorKey;

	// Set the pitch for both surfaces
    SpritePitch = (WORD)srcDDSD.lPitch;
    dPitch      = (WORD)destDDSD.lPitch;

    // Initialize the pointers to the upper left hand corner of surface
    lpSprite = (BYTE*)srcDDSD.lpSurface;
    lpDest   = (BYTE*)destDDSD.lpSurface;

	// Do some work outside of the loop
	PLUS64         = 64 | (64 << 16);
	ALPHABY4      = (ALPHA / 4) | ((ALPHA / 4) << 16);
	doubleColorKey = ColorKey | (ColorKey << 16);

	switch(BPP)
	{
	case 8:
		// IMHO paletized modes are a thing of the past please feel free to 
		// implement this if you so desire.
		break;

	case 15:  ////////////////////////////////////////////////////////////////////////
		      //
		      //  WARNING!!!: I do not have a video card that uses the 5-5-5 format 
		      //              this barnch of code has not be tested.
		      //
		      ////////////////////////////////////////////////////////////////////////
		
		// Initialize the pointers to the first pixel in the rectangle
		lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 2);
		lpDest   += (lDestY * dPitch) + (lDestX * 2);

		// Set the horizontal padding
		sbuf = (WORD)(SpritePitch - (2 * width));
		dbuf = (WORD)(dPitch - (2 * width));

		// Is the Sprite width odd or even?
		if (width % 2 == 1)
		{
			oddWidth = TRUE;
			width = (width - 1) / 2; //div by 2, processing 2 pixels at a time.
		}
		else
			width = width / 2;  //div by 2, processing 2 pixels at a time.

		i = height;
		do
		{
			if (oddWidth)
			{
				sTemp = *((WORD*)lpSprite);

				if (sTemp != sColorKey)
				{
					dTemp = *((WORD*)lpDest);
					sb = sTemp & 0x1f;
					db = dTemp & 0x1f;
					sg = (sTemp >> 5) & 0x1f;
					dg = (dTemp >> 5) & 0x1f;
					sr = (sTemp >> 10) & 0x1f;
					dr = (dTemp >> 10) & 0x1f;

					*((WORD*)lpDest) = (WORD)((ALPHA * (db - sb) >> 8) + sb |
						((ALPHA * (dg - sg) >> 8) + sg) << 5 |
						((ALPHA * (dr - sr) >> 8) + sr) << 10);
				}

				lpDest += 2;
				lpSprite += 2;
			}
			j = width;
			do
			{
				sTemp = *((DWORD*)lpSprite);

				if ( sTemp != doubleColorKey )
				{
					dTemp = *((DWORD*)lpDest);

					sb = sTemp & 0x001F001F;
					db = dTemp & 0x001F001F;
					sg = (sTemp >> 5)  & 0x001F001F;
					dg = (dTemp >> 5)  & 0x001F001F;
					sr = (sTemp >> 10) & 0x001F001F;
					dr = (dTemp >> 10) & 0x001F001F;

					BLUEC  = ((((ALPHA * ((sb + PLUS64) - db)) >> 8) + db) - ALPHABY4) & 0x001F001F;
					GREENC = (((((ALPHA * ((sg + PLUS64) - dg)) >> 8) + dg) - ALPHABY4) & 0x001F001F) << 5;
					REDC   = (((((ALPHA * ((sr + PLUS64) - dr)) >> 8) + dr) - ALPHABY4) & 0x001F001F) << 10;

					Result = BLUEC | GREENC | REDC;

					if ( (sTemp >> 16) == ColorKey )
							Result = (Result & 0xFFFF) | (dTemp & 0xFFFF0000);
					else if ( (sTemp & 0xFFFF) == ColorKey )
							Result = (Result & 0xFFFF0000) | (dTemp & 0xFFFF);

					*((DWORD*)lpDest) = Result;
				}
				lpDest    += 4;
				lpSprite  += 4;

			}while (--j > 0);

			lpDest   += dbuf;
			lpSprite += sbuf;

		}while (--i > 0);

		break;

	case 16:

		// Initialize the pointers to the first pixel in the rectangle
		lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 2);
		lpDest   += (lDestY * dPitch) + (lDestX * 2);

		// Set the horizontal padding
		sbuf = (WORD)(SpritePitch - (2 * width));
		dbuf = (WORD)(dPitch - (2 * width));

		// Is the Sprite width odd or even?
		if (width % 2 == 1)
		{
			oddWidth = TRUE;
			width = (width - 1) / 2; //div by 2, processing 2 pixels at a time.
		}
		else
			width = width / 2;  //div by 2, processing 2 pixels at a time.


		i = height;
		do
		{
			if (oddWidth)
			{
				sTemp = *((WORD*)lpSprite);

				if (sTemp != ColorKey)
				{
					dTemp = *((WORD*)lpDest);
					sb = sTemp & 0x1f;
					db = dTemp & 0x1f;
					sg = (sTemp >> 5) & 0x3f;
					dg = (dTemp >> 5) & 0x3f;
					sr = (sTemp >> 11) & 0x1f;
					dr = (dTemp >> 11) & 0x1f;

					*((WORD*)lpDest) = (WORD)((ALPHA * (sb - db) >> 8) + db |
						((ALPHA * (sg - dg) >> 8) + dg) << 5 |
						((ALPHA * (sr - dr) >> 8) + dr) << 11);
				}

				lpDest   += 2;
				lpSprite += 2;
			}
			j = width;
			do
			{
				sTemp = *((DWORD*)lpSprite);

				if ( sTemp != doubleColorKey )
				{
					dTemp = *((DWORD*)lpDest);

					sb = sTemp & 0x001F001F;
					db = dTemp & 0x001F001F;
					sg = (sTemp >> 5)  & 0x003F003F;
					dg = (dTemp >> 5)  & 0x003F003F;
					sr = (sTemp >> 11) & 0x001F001F;
					dr = (dTemp >> 11) & 0x001F001F;

					BLUEC  = ((((ALPHA * ((sb + PLUS64) - db)) >> 8) + db) - ALPHABY4) & 0x001F001F;
					GREENC = (((((ALPHA * ((sg + PLUS64) - dg)) >> 8) + dg) - ALPHABY4) & 0x003F003F) << 5;
					REDC   = (((((ALPHA * ((sr + PLUS64) - dr)) >> 8) + dr) - ALPHABY4) & 0x001F001F) << 11;

					Result = BLUEC | GREENC | REDC;

					if ( (sTemp >> 16) == ColorKey )
							Result = (Result & 0xFFFF) | (dTemp & 0xFFFF0000);
					else if ( (sTemp & 0xFFFF) == ColorKey )
							Result = (Result & 0xFFFF0000) | (dTemp & 0xFFFF);

					*((DWORD*)lpDest) = Result;
				}
				lpDest    += 4;
				lpSprite  += 4;

			}while (--j > 0);

			lpDest   += dbuf;
			lpSprite += sbuf;

		}while (--i > 0);

		break;

	case 24:  ////////////////////////////////////////////////////////////////////////
		      //
		      //  WARNING!!!: I do not have a video card capable of 24bit rendering 
		      //              this barnch of code has not be tested.
		      //
		      ////////////////////////////////////////////////////////////////////////

		// Initialize the pointers to the first pixel in the rectangle
		lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 3);
		lpDest   += (lDestY * dPitch) + (lDestX * 3);

		// Set the horizontal padding
		sbuf = (WORD)(SpritePitch - (3 * width));
		dbuf = (WORD)(dPitch - (3 * width));

		i = height;
		do
		{
			j = width;
			do
			{
				sTemp = *((DWORD*)lpSprite);

				if ((sTemp & 0xFFFFFF) != sColorKey)
				{
					dTemp = *((DWORD*)lpDest);
					sb = sTemp & 0xFF;
					db = dTemp & 0xFF;
					sg = (sTemp >> 8) & 0xFF;
					dg = (dTemp >> 8) & 0xFF;
					sr = (sTemp >> 16) & 0xFF;
					dr = (dTemp >> 16) & 0xFF;

					Result = (DWORD)((ALPHA * (db - sb) >> 8) + sb |
						((ALPHA * (dg - sg) >> 8) + sg) << 8 |
						((ALPHA * (dr - sr) >> 8) + sr) << 16);

					*((WORD*)lpDest) = (WORD)(Result & 0xFFFF);
					lpDest += 2;
					*lpDest = (BYTE)(Result >> 16);
					lpDest++;
				}
				else
				{
					lpDest += 3;
				}

				lpSprite += 3;

			}while (--j > 0);
			lpDest   += dbuf;
			lpSprite += sbuf;

		}while (--i > 0);
		break;

	case 32:

		// Initialize the pointers to the first pixel in the rectangle
		lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 4);
		lpDest   += (lDestY * dPitch) + (lDestX * 4);

		// Set the horizontal padding
		sbuf = (WORD)(SpritePitch - (4 * width));
		dbuf = (WORD)(dPitch - (4 * width));

		i = height;
		do
		{
			j = width;
			do
			{
				sTemp = *((DWORD*)lpSprite);

				if ((sTemp & 0xFFFFFF) != sColorKey)
				{
					dTemp = *((DWORD*)lpDest);
					sb = sTemp & 0xFF;
					db = dTemp & 0xFF;
					sg = (sTemp >> 8) & 0xFF;
					dg = (dTemp >> 8) & 0xFF;
					sr = (sTemp >> 16) & 0xFF;
					dr = (dTemp >> 16) & 0xFF;

					Result = (DWORD)((ALPHA * (db - sb) >> 8) + sb |
						((ALPHA * (dg - sg) >> 8) + sg) << 8 |
						((ALPHA * (dr - sr) >> 8) + sr) << 16);

					*((WORD*)lpDest) = (WORD)(Result & 0xFFFF);
					lpDest += 2;
					*lpDest = (BYTE)(Result >> 16);
					lpDest += 2;
				}
				else
				{
					lpDest += 4;
				}

				lpSprite += 4;

			}while (--j > 0);
			lpDest   += dbuf;
			lpSprite += sbuf;

		}while (--i > 0);
		break;
	} // End RGB Format switch statement


	src->Unlock(NULL);
	dest->Unlock(NULL);

	return DD_OK;
}

