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

#include "Main.h"
#include "Result.h"
#include "Config.h"
#include "Dead.h"
#include "Double.h"
#include "Song.h"
#include "Select.h"
#include "Input.h"
#include "Common.h"
#include "Surface.h"
#include "Timer.h"
#include "Chunk.h"
#include "Music.h"
#include <stdio.h>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fmodex/fmod_errors.h>

using std::max;
using std::min;

#define VER_NUM	"0.4a"
char	TITLE[PATH_LEN];

#define	PRGNAME		"Kick It UP!"

FMOD_SYSTEM  *	gpSystem;

Surface gScreen;
Surface	gGameTitle;
Surface gStateComment;
Surface gSmallFont;
Surface gSelectBack;
Surface gSongBack;
Surface gSongTitle;
Surface gNumberFont;
Surface gNoDISC;
Surface gShiftLeft;
Surface gShiftRight;
Surface	gModeIcon;
Surface gDoubleIcon;
Surface gCrazyIcon;
Surface gEasyIcon;
Surface gHardIcon;
Surface	gStageCount;
Surface gArrow1;
Surface gArrow2;

Surface gWArrow;

Surface	pArrow1;
Surface	pArrow3;
Surface	pArrow5;
Surface	pArrow7;
Surface	pArrow9;

Surface	cArrow1;
Surface	cArrow3;
Surface	cArrow5;
Surface	cArrow7;
Surface	cArrow9;
Surface	JudgeFont;
Surface ComboFont;
Surface	GaugeWaku;
Surface Gauge;
Surface DeadScreen;
Surface GameOver;
Surface Score;
Surface	ResultFont;
Surface	ResultBack;


Chunk	gOpening;
Chunk	gDead;
Chunk	gMode;
Chunk	gCancel;
Chunk	gMove;
Chunk	gBeat;
Chunk	gSelectSong;

Music   gIntro;
Music   gSong;


st_HighSpeed	gSpeed[2];

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


char				SongName[PATH_LEN+1];
char				SongName2[PATH_LEN+1];
char				SongName3[PATH_LEN+1];

char				LogoTitleName[PATH_LEN];

double				bpm;
double				bpm2;
double				bpm3;

int					start,start2,start3;
Uint32					bunki,bunki2;

int					tick;

char				Title[PATH_LEN+1];

char				g_ProgramState=GAMETITLE;
char				ArrowState1p[10];
char				ArrowState2p[10];

char				ArrowState_Joy[10];

Uint32				PressedKey1p[10];
Uint32				PressedKey2p[10];
Uint32				PressedKey_Joy[10];

char				Judgement1p;
char				Judgement2p;
Uint32				Combo1p;
Uint32				Combo2p;
Uint32				dwState;
Uint32				dwState2;
bool				g_bActive;

long				Score1p;
long				Score2p;

int					Gauge1p=10;
int					Gauge2p=10;

char				GameMode=MODE_HARD;

char				Couple=false;
char				Double=false;

char				First;
int start1;

bool				Start1p;
bool				Start2p;

Song				CSONG[512];

bool	SongFlag;
bool	IntroFlag;

int		MaxSpeed;
int		MinSpeed;

int		JudgeArray[110];

bool	bModeMirror1p;
bool	bModeNonstep1p;
bool	bModeSynchro;
bool	bModeUnion1p;
bool	bModeRandom1p;
bool	b4dMix1p;			// 1p 4DMIX mode.
bool	bModeVanish1p;
bool	bModeSuddenR1p;
bool	bModeRandomS1p;

bool	bModeMirror2p;
bool	bModeNonstep2p;
bool	bModeUnion2p;
bool	bModeRandom2p;
bool	b4dMix2p;
bool	bModeVanish2p;
bool	bModeSuddenR2p;
bool	bModeRandomS2p;

int	ALPHA=0;
int	inc=20;
Uint32	CKey_Arr;

Uint32	cPerfect1p;
Uint32	cGreat1p;
Uint32	cGood1p;
Uint32	cBad1p;
Uint32	cMiss1p;
Uint32	cMaxCombo1p;

Uint32	cPerfect2p;
Uint32	cGreat2p;
Uint32	cGood2p;
Uint32	cBad2p;
Uint32	cMiss2p;
Uint32	cMaxCombo2p;

// Data of configuration
KIUCONFIG	KCFG;

int		dwGameCount;

bool debugflag=true;
char g_szDebugName[PATH_LEN];

bool	g_fullscreen=false;

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

void	DisplayStageCount(const int count)
{
    SDL_Rect    sRect = {0, };

    sRect.x = count * 80;
    sRect.y = 0;
    sRect.w = 80;
    sRect.h = 46;

    gStageCount.BltFast( 280, 0, gScreen, &sRect );
}



void ClearMode(void)
{
	bModeMirror1p=false;
	bModeNonstep1p=false;
	bModeSynchro=false;
	bModeUnion1p=false;
	bModeRandom1p=false;
	b4dMix1p=false;
	bModeVanish1p=false;
	bModeRandomS1p=false;
	bModeSuddenR1p=false;

	bModeMirror2p=false;
	bModeNonstep2p=false;
	bModeUnion2p=false;
	bModeRandom2p=false;
	b4dMix2p=false;

	bModeVanish2p=false;
	Double=false;
	bModeRandomS2p=false;
	bModeSuddenR2p=false;

	// TODO:
	gSpeed[0].step = 1;
	gSpeed[0].step1=1;
	gSpeed[0].step3=1;
	gSpeed[0].step5=1;
	gSpeed[0].step7=1;
	gSpeed[0].step9=1;

	gSpeed[1].step=1;
	gSpeed[1].step1=1;
	gSpeed[1].step3=1;
	gSpeed[1].step5=1;
	gSpeed[1].step7=1;
	gSpeed[1].step9=1;

}

void	GameOver1(void)
{
	static int count;

	if(First==0) {
		count=0;
		First++;
	}
	count++;
    GameOver.BltFast( 0, 0, gScreen );
    if(count==60) {
        First=0;
        g_ProgramState=GAMETITLE;
    }
}

void DrawScore1p(void)
{
	SDL_Rect	cRect;
	char chScore[11];

	sprintf(chScore,"%07ld",Score1p);
			
	for(int i=0;;i++) {
		if(chScore[i]==0)
			break;
		chScore[i]-=48;
		cRect.x=chScore[i]*22;
		cRect.w=21;
		cRect.y=0;
		cRect.h=35;

        Score.BltFast( 20+i*22,444, gScreen, &cRect );
	}
}

void DrawScore2p(void)
{
	SDL_Rect	cRect;
	char chScore[11];

	sprintf(chScore,"%07ld",Score2p);
			
	for(int i=0;;i++) {
		if(chScore[i]==0)
			break;
		chScore[i]-=48;
		cRect.x=chScore[i]*22;
		cRect.w=21;
		cRect.y=0;
		cRect.h=35;
        Score.BltFast( 463+i*22, 444, gScreen, &cRect );
	}
}

void DrawGauge1p(void)
{
	int CurG;
	SDL_Rect sRect;

	CurG=Gauge1p;
	
	if(CurG<0)CurG=0;

	// g_pDDSBack->BltFast(32,0,GaugeWaku,NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    GaugeWaku.BltFast(32, 0, gScreen );
	
	sRect.y=0;
	sRect.x=0;
	sRect.w=6;
	sRect.h=20;

	for(int i=0;i<7;i++)
	{
		if(i>CurG)break;
        //	g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 280-(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=6;
	sRect.w=6;
	sRect.h=20;

	for(int i=7;i<21;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 280-(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=12;
	sRect.w = 6;
	sRect.h=20;

	for(int i=21;i<42;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(280-(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 280-(i*6), 20, gScreen, &sRect );
	}

}

void DrawGauge2p(void)
{
	int CurG;
	int i;
	SDL_Rect sRect;

	CurG=Gauge2p;
	
	if(CurG<0)
		CurG=0;

	// g_pDDSBack->BltFast(352,0,GaugeWaku,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    GaugeWaku.BltFast( 352, 0, gScreen );

	sRect.y=0;
	sRect.x=0;
	sRect.w = 6;
	sRect.h=20;

	for(i=0;i<7;i++) {
		if(i>CurG)
			break;
        Gauge.BltFast( 352+(i*6), 20, gScreen, &sRect );
	}

	sRect.y = 0;
	sRect.x = 6;
	sRect.w = 6;
	sRect.h = 20;

	for(i=7;i<21;i++) {
		if(i>CurG)
			break;
        Gauge.BltFast( 352+(i*6), 20, gScreen, &sRect );
	}

	sRect.y = 0;
	sRect.x = 12;
	sRect.w = 6;
	sRect.h = 20;

	for(i=21;i<42;i++) {
		if(i>CurG)
			break;
        Gauge.BltFast( 352+(i*6), 20, gScreen, &sRect );
	}

}

void KIU_STAGE(void)
{
	static int temp;
	static int i;
	static Uint32 cur,last,sec;
	static Uint32 starttime, curtime;

	static SDL_Rect rect1[7],rect3[7],rect5[7],rect7[7],rect9[7];
	int k;
	Uint32 delta;

	static int sta;

	static double tail;

	static time_t t;

	char s[50];

	double bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

    gSongBack.BltFast( 0, 0, gScreen );
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
	sprintf(s, "bunki :%uld", bunki);
	DisplayMessage(0,96,s);
	sprintf(s, "bunki2:%uld", bunki2);
	DisplayMessage(0,112,s);

	if(start1==0)
	{
		if(b4dMix1p==true)
		{
			MaxSpeed = MinSpeed = gSpeed[0].step1;

			MaxSpeed = max( MaxSpeed, gSpeed[0].step3);
			MaxSpeed = max( MaxSpeed, gSpeed[0].step5);
			MaxSpeed = max( MaxSpeed, gSpeed[0].step7);
			MaxSpeed = max( MaxSpeed, gSpeed[0].step9);
			
			MinSpeed = min( MinSpeed, gSpeed[0].step3);
			MinSpeed = min( MinSpeed, gSpeed[0].step5);
			MinSpeed = min( MinSpeed, gSpeed[0].step7);
			MinSpeed = min( MinSpeed, gSpeed[0].step9);
		}
		else 
		{
			MaxSpeed = MinSpeed = gSpeed[0].step;
			gSpeed[0].step1 = gSpeed[0].step3 = gSpeed[0].step5 = gSpeed[0].step7 = gSpeed[0].step9 = gSpeed[0].step;
		}

		if(b4dMix2p)
		{
			MaxSpeed=MinSpeed=gSpeed[1].step1;

			MaxSpeed = max( MaxSpeed, gSpeed[1].step3);
			MaxSpeed = max( MaxSpeed, gSpeed[1].step5);
			MaxSpeed = max( MaxSpeed, gSpeed[1].step7);
			MaxSpeed = max( MaxSpeed, gSpeed[1].step9);
			
			MinSpeed = min( MinSpeed, gSpeed[1].step3);
			MinSpeed = min( MinSpeed, gSpeed[1].step5);
			MinSpeed = min( MinSpeed, gSpeed[1].step7);
			MinSpeed = min( MinSpeed, gSpeed[1].step9);
		}
		else 
		{
			MaxSpeed = MinSpeed = gSpeed[1].step;
			gSpeed[1].step1 = gSpeed[1].step3 = gSpeed[1].step5 = gSpeed[1].step7 = gSpeed[1].step9 = gSpeed[1].step;
		}

		for(sta=0;sta<6;sta++) {
			rect7[sta].y = 0;
			rect7[sta].x = PUMP_SPRITE_NEW*sta;
			rect7[sta].w = PUMP_SPRITE_NEW;
			rect7[sta].h = PUMP_SPRITE_NEW;

			rect9[sta].y = PUMP_SPRITE_NEW;
			rect9[sta].x = PUMP_SPRITE_NEW*sta;
			rect9[sta].w = PUMP_SPRITE_NEW;
			rect9[sta].h = PUMP_SPRITE_NEW;

			rect5[sta].y = PUMP_SPRITE_NEW*2;
			rect5[sta].x = PUMP_SPRITE_NEW*sta;
			rect5[sta].w = PUMP_SPRITE_NEW;
			rect5[sta].h = PUMP_SPRITE_NEW;

			rect3[sta].y = PUMP_SPRITE_NEW*3;
			rect3[sta].x = PUMP_SPRITE_NEW*sta;
			rect3[sta].w = PUMP_SPRITE_NEW;
			rect3[sta].h = PUMP_SPRITE_NEW;

			rect1[sta].y = PUMP_SPRITE_NEW*4;
			rect1[sta].x = PUMP_SPRITE_NEW*sta;
			rect1[sta].w = PUMP_SPRITE_NEW;
			rect1[sta].h = PUMP_SPRITE_NEW;
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

        gSongBack.BltFast( 0, 0, gScreen );

		if(SongFlag==true)
		{
            gSong.Play();
		}

		start*=10;
		start2*=10;start3*=10;
		bunki*=10;bunki2*=10;
		
		last=cur=SDL_GetTicks();
		tail=0;
		i=0;
		temp=0;
		start1++;
		temp+=55;
		starttime=start;
		curtime=0;
	}
	
	cur=SDL_GetTicks();        // 130/ 600000
	delta=cur-last;
	last=cur;

	if(Start1p)
        DrawArrow1p(i); //회색 화살표를 말합니다.

	if(Start2p)
        DrawArrow2p(i);

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

			curtime = gSong.GetCurrentPosition();

			if(curtime > starttime) 
			    delta=(Uint32)curtime-starttime;
			else
                delta=(Uint32)curtime;
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
	if(SDL_GetTicks()-sec>50)
	{
		sec=SDL_GetTicks();
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
				gSong.Halt();
				SongFlag=false;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data[i+k][0]=='1')
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow, rect1[sta] );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow, rect7[sta] );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow, rect5[sta] );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow, rect9[sta] );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow, rect3[sta] );
			
			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow, rect1[sta] );
 			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow, rect7[sta] );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow, rect5[sta] );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow, rect9[sta] );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow, rect3[sta] );

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
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow,rect1[sta] );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow,rect7[sta] );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow,rect5[sta] );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow,rect9[sta] );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow,rect3[sta] );

			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow,rect1[sta] );
			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow,rect7[sta] );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow,rect5[sta] );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow,rect9[sta] );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow,rect3[sta] );

			Data_y[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+2][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow,rect1[sta] );
			if(Data[i+k+2][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow,rect7[sta] );
			if(Data[i+k+2][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow,rect5[sta] );
			if(Data[i+k+2][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow,rect9[sta] );
			if(Data[i+k+2][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow,rect3[sta] );

			Data_y[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data[i+k+3][0]=='1')
				ClpBlt(LP1_X,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step1-(PUMP_SPRITE_Y)*(gSpeed[0].step1-1),gWArrow,rect1[sta] );
			if(Data[i+k+3][1]=='1')
				ClpBlt(LP7_X,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step7-(PUMP_SPRITE_Y)*(gSpeed[0].step7-1),gWArrow,rect7[sta] );
			if(Data[i+k+3][2]=='1')
				ClpBlt(LP5_X,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step5-(PUMP_SPRITE_Y)*(gSpeed[0].step5-1),gWArrow,rect5[sta] );
			if(Data[i+k+3][3]=='1')
				ClpBlt(LP9_X,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step9-(PUMP_SPRITE_Y)*(gSpeed[0].step9-1),gWArrow,rect9[sta] );
			if(Data[i+k+3][4]=='1')
				ClpBlt(LP3_X,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[0].step3-(PUMP_SPRITE_Y)*(gSpeed[0].step3-1),gWArrow,rect3[sta] );

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
				gSong.Halt();
				SongFlag=false;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data1[i+k][5]=='1')
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );
			
			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/2)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );

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
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );

			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(12+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );

			Data_y1[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+2][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k+2][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k+2][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k+2][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k+2][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );

			Data_y1[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data1[i+k+3][5]=='1')
				ClpBlt(LP1_X1,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step1-(PUMP_SPRITE_Y)*(gSpeed[1].step1-1),gWArrow,rect1[sta] );
			if(Data1[i+k+3][6]=='1')
				ClpBlt(LP7_X1,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step7-(PUMP_SPRITE_Y)*(gSpeed[1].step7-1),gWArrow,rect7[sta] );
			if(Data1[i+k+3][7]=='1')
				ClpBlt(LP5_X1,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step5-(PUMP_SPRITE_Y)*(gSpeed[1].step5-1),gWArrow,rect5[sta] );
			if(Data1[i+k+3][8]=='1')
				ClpBlt(LP9_X1,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step9-(PUMP_SPRITE_Y)*(gSpeed[1].step9-1),gWArrow,rect9[sta] );
			if(Data1[i+k+3][9]=='1')
				ClpBlt(LP3_X1,(38+temp+PUMP_SPRITE_Y*k/4)*gSpeed[1].step3-(PUMP_SPRITE_Y)*(gSpeed[1].step3-1),gWArrow,rect3[sta] );

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

	if(gSpeed[0].step>1)DrawMode(0,160,HMODE_2X);

	if(bModeMirror2p)DrawMode(600,200,HMODE_MIRROR);
	if(bModeNonstep2p)DrawMode(600,240,HMODE_NONSTEP);
	if(bModeUnion2p)DrawMode(600,320,HMODE_UNION);
	if(bModeRandom2p)DrawMode(600,360,HMODE_RANDOM);
	if(bModeVanish2p)DrawMode(600,400,HMODE_VANISH);

	if(gSpeed[1].step>1)DrawMode(600,160,HMODE_2X);

	// Flipp();
}


void WaveSet_Loading(void)
{
    gOpening.Load( "wave/opening.wav" );
    gDead.Load( "wave/dead.wav" );
    gMode.Load( "wave/mode.wav" );
    gCancel.Load( "wave/cancel.wav" );
    gMove.Load( "wave/move.wav" );
    gBeat.Load( "wave/beat.wav" );
    gSelectSong.Load( "wave/music_select.wav" );
}

void DisplayMessage(int x, int y, const char * message)
{
#define FONT_SIZE	8
#define FONT_HEIGHT	16
	SDL_Rect	sRect;
	
	for(int i = 0 ; ; i++) {		
		if(message[i] == 0)
			break;	
	
		char msg1 = toupper(message[i]);
		
		sRect.x = FONT_SIZE * (msg1-' ');
		sRect.y = 0;
		sRect.w = FONT_SIZE;
		sRect.h = FONT_HEIGHT;

		gSmallFont.BltFast(x, y, gScreen, &sRect );

		x += FONT_SIZE;
	}
}

			
bool ClpBlt(int x ,int y ,Surface & surface, const SDL_Rect & srect)
{
    SDL_Rect sRect;

    sRect = srect;

    if(640 < x || 480 < y)
        return true;

    if( sRect.h < -y || sRect.w < -x )
        return true;

    if(480 < (y + sRect.h))
        sRect.h = 480 - y;

    if(y < 0) {
        sRect.y -= y;
        sRect.h += y;
        y = 0;
    }

    if(640 < x + sRect.w )
        sRect.w = 640 - x;

    if(x < 0 ) {
        sRect.x -= x;
        sRect.w += x;
        x = 0;
    }

	if(g_ProgramState == DOUBLEST) {
		if(bModeVanish1p || bModeVanish2p) {
			if(y<150)
				return true;

            if(y<250)
                surface.SetAlpha( (y-150) * 2 );
		}
		else if(bModeSuddenR1p || bModeSuddenR2p) {
            if(100 < y && y <= 200)
                surface.SetAlpha( (200-y)*2 );
			else if(200 < y && y < 320)
				return true;
            else if(y < 420)
                surface.SetAlpha( (y-320)*2 );
		}
	}
	else if(x < 320) {
		if(bModeVanish1p) {
			if(y < 150)
                return true;
            if(y < 250)
                surface.SetAlpha( (y-150) * 2 );
		}
		else if(bModeSuddenR1p) {
            if(100 < y && y <= 200)
                surface.SetAlpha( (200-y)*2 );
			else if(200 < y && y < 320)
				return true;
            else if(y < 420)
                surface.SetAlpha( (y-320)*2 );
		}
	}
	else if(x>320) {
		if(bModeVanish2p) {
			if(y<150)
                return true;
            if(y<250)
                surface.SetAlpha( (y-150) * 2 );
		}
		else if(bModeSuddenR2p) {
            if(y>100 && y<=200)
                surface.SetAlpha( (200-y)*2 );
            else if(y>200 && y<320)
                return true;
            else if(y<420)
                surface.SetAlpha( (y-320)*2 );
		}
	}
    surface.BltFast( x, y, gScreen, &sRect );

	return true;
}

void StageTitle(void)
{
	if(First==0)
	{
		ClearMode();

		Start1p=false;
		Start2p=false;
		First++;
		gOpening.Play( true );
	}

	ReadGameInput();

	if(PressedKey1p[5]==true)
		Start1p=true;

	if(PressedKey2p[5]==true)
		Start2p=true;
	
	// Draw Background image. "KICK IT UP"
	gGameTitle.BltFast(0, 0, gScreen );

	// Check Start.
	if(Start1p || Start2p) {
		if(Start1p && Start2p) {
			PressedKey2p[0]=3;
		} else if(Start1p) {
			if(PressedKey1p[5]==true)
				PressedKey2p[0]=3;
		} else if(Start2p) {
			if(PressedKey2p[5]==true)
				PressedKey2p[0]=3;
		}
	}

	// if start button is pressed.
	if(PressedKey2p[0]==3) {
		Couple = Start1p && Start2p;

		First=0;
		gOpening.Halt();
		PressedKey2p[0]=0;

		// Change ProgramState to SelectSong Stage
		g_ProgramState=SELECTSONG;

	}

	// Draw to screen "FREE PLAY!"
	SDL_Rect	sRect;
	sRect.x = 0;
	sRect.y = 46;
	sRect.w = 220;
	sRect.h = 69-46;
	gStateComment.SetAlpha( 255 );
	gStateComment.BltFast( 210, 450, gScreen, &sRect );
	
	if(Start1p==false) {
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;
		gStateComment.SetAlpha( ALPHA );
		gStateComment.BltFast( 10, 450, gScreen, &sRect );
	}

	if(Start2p==false) {
		// Draw to screen (410, 450) "PRESS CENTER BUTTON"
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		gStateComment.SetAlpha( ALPHA );
		gStateComment.BltFast( 410, 450, gScreen, &sRect );
	}

	ALPHA += inc;
	if (ALPHA > 256) {
		ALPHA = 256;
		inc = -10;
	} else if (ALPHA < 0) {
		ALPHA = 0;
		inc = 10;
	}
}

void DrawJudge1p(void)
{
	static char LastJudge;

	char chCombo1p[255];

	static Uint32 sec,delta;

	SDL_Rect rRect, cRect, destRect;
	int Loop;

	if(Judgement1p) {
		dwState=0;
	} else if(dwState) {
		Judgement1p=LastJudge;

		delta=SDL_GetTicks()-sec;
		if(delta>16) {
			sec=SDL_GetTicks();
			if(dwState>=40) {
				dwState=0;
				Judgement1p=NONE;
			} else {
				if(delta >16 && delta <32)dwState+=2;
				else if(delta >=32)dwState+=4;
			}
		}
	}

	switch(Judgement1p) {
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.y=0;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.y=JUDGE_SIZE_Y;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.y=JUDGE_SIZE_Y*2;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.y=JUDGE_SIZE_Y*3;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.y=JUDGE_SIZE_Y*4;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState==0)dwState++;
			break;
	}

	if(dwState>15) {
		destRect.y=200;
		destRect.x=40;
		destRect.w = JUDGE_SIZE_X;
		destRect.h=JUDGE_SIZE_Y;
	} else {
		destRect.y = static_cast<int>(200-30+(dwState*2));
		destRect.x = static_cast<int>(40-60+(dwState*4));
		destRect.w = JUDGE_SIZE_X;
		destRect.h = JUDGE_SIZE_Y;
	}

	if(Judgement1p) {
		// g_pDDSBack->Blt(&destRect, JudgeFont, &rRect,DDBLT_WAIT | DDBLT_KEYSRC , NULL);
        JudgeFont.BltFast( destRect.x, destRect.y, gScreen, &rRect );

        /* 콤보 출력부 입니다. */
		if((Judgement1p==PERFECT || Judgement1p==GREAT) && Combo1p>3) {
			sprintf(chCombo1p,"%03d",Combo1p);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo1p[Loop]==0)break;
				chCombo1p[Loop]-=48;
				cRect.x=chCombo1p[Loop]*50;
				cRect.w=50;
				cRect.y=0;
				cRect.h=65;
				if(dwState>10)  ComboFont.BltFast( 80+Loop*50, 250+dwState*2-dwState*2, gScreen, &cRect );
				else            ComboFont.BltFast( 80+Loop*50, 250+dwState*2, gScreen, &cRect );

				cRect.x=0;
				cRect.w=150;
				cRect.y=65;
				cRect.h=100-65;
				
				if(dwState>10)  ComboFont.BltFast( 80,320+dwState*2-dwState*2, gScreen, &cRect );
				else            ComboFont.BltFast( 80,320+dwState*2, gScreen, &cRect );
			}
		}
	}

	Judgement1p=NONE;
}

void DrawJudge2p(void)
{
	static char LastJudge;

	char chCombo2p[255];

	static Uint32 sec,delta;

	SDL_Rect rRect, cRect, destRect;
	int Loop;

	if(Judgement2p) {
		dwState2=0;
	} else if(dwState2) {
		Judgement2p=LastJudge;

		delta=SDL_GetTicks()-sec;
		
		if(delta>16) {
			sec=SDL_GetTicks();
			if(dwState2>=40) {
				dwState2=0;
				Judgement2p=NONE;
			} else {
				if(delta >16 && delta <32)dwState2+=2;
				else if(delta >=32)dwState2+=4;
			}
		}
	}

	switch(Judgement2p) {
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.y=0;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.y=JUDGE_SIZE_Y;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.y=JUDGE_SIZE_Y*2;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.y=JUDGE_SIZE_Y*3;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.y=JUDGE_SIZE_Y*4;
			rRect.w=JUDGE_SIZE_X;
			rRect.x=0;
			rRect.h=JUDGE_SIZE_Y;
			if(dwState2==0)dwState2++;
			break;
	}

	if(dwState2>15) {
		destRect.y=200;
		destRect.x=350;
		destRect.w=JUDGE_SIZE_X;
		destRect.h=JUDGE_SIZE_Y;
	} else {
		destRect.y = static_cast<int>( 200-30+(dwState2*2) );
		destRect.x = static_cast<int>( 350-60+(dwState2*4) );
		destRect.w=JUDGE_SIZE_X;
		destRect.h=JUDGE_SIZE_Y;
	}

	if(Judgement2p) {
        JudgeFont.BltFast( destRect.x, destRect.y, gScreen, &rRect );

        /* 콤보 출력부 입니다. */
		if((Judgement2p==PERFECT || Judgement2p==GREAT) && Combo2p>3) {
			sprintf(chCombo2p,"%03d",Combo2p);
			
			for(Loop=0;;Loop++) {
				if(chCombo2p[Loop]==0)break;
				chCombo2p[Loop]-=48;
				cRect.x=chCombo2p[Loop]*50;
				cRect.w=50;
				cRect.y=0;
				cRect.h=65;
                if(dwState2>10)
					ComboFont.BltFast( 400+Loop*50, 250+dwState2*2-dwState2*2, gScreen, &cRect );
				else
					ComboFont.BltFast( 400+Loop*50,250+dwState2*2, gScreen, &cRect );

				cRect.x=0;
				cRect.w=150;
				cRect.y=65;
				cRect.h=100;
				
				if(dwState2>10)
					ComboFont.BltFast( 400,320+dwState2*2-dwState2*2, gScreen, &cRect );
				else
					ComboFont.BltFast( 400,320+dwState2*2, gScreen, &cRect );
			}
		}
	}

	Judgement2p=NONE;
}


void DrawArrow1p(Uint32 cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720}; // DELETE ME

	static int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};    // DELETE ME

	static Uint8 s1,s3,s5,s7,s9;
	static Uint32 stat1,stat3,stat5,stat7,stat9;
	static Uint32 cur2;
	static int beat;

	static bool Crash1, Crash3, Crash5, Crash7, Crash9;
	static bool	On1, On3, On5, On7, On9;

	static SDL_Rect pArr1,pArr3,pArr5,pArr7,pArr9;
	static SDL_Rect cArr1,cArr3,cArr5,cArr7,cArr9;


	Uint8 JudgeTemp=0;
	Uint8	count;

	if(cur2!=cur)
	{
		if(bModeRandomS1p == true)
		{
			gSpeed[0].step1 = gSpeed[0].step3 = gSpeed[0].step5 = gSpeed[0].step7 = gSpeed[0].step9 = 1 + rand() % 8 ;
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
	if(KCFG.auto1_1p==true)
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
					Crash1=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_1p==true)
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
					Crash7=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_1p==true)
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
					Crash5=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_1p==true)
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
					Crash9=true;
					break;
				}
			}
		}
	}

	if(KCFG.auto3_1p==true)
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
					Crash3=true;
					break;
				}
			}
			
		}
	}

	if(s1 || (PressedKey1p[1]==true) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=false;
		}
		else
		{
			s1++;
		}
		if(PressedKey1p[1]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[0].step1 < Data_y[cur+count] &&
				ZONE_D*gSpeed[0].step1 > Data_y[cur+count]  )
			{
				if( PERFECT_ZONE_U*gSpeed[0].step1 < Data_y[cur+count] &&
					PERFECT_ZONE_D*gSpeed[0].step1 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=true;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[0].step1 < Data_y[cur+count] &&
						 GREAT_ZONE_D*gSpeed[0].step1 > Data_y[cur+count]  )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=true;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[0].step1 < Data_y[cur+count] &&
					     GOOD_ZONE_D*gSpeed[0].step1 > Data_y[cur+count] )
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
				else if( BAD_ZONE_U*gSpeed[0].step1 < Data_y[cur+count] &&
						 BAD_ZONE_D*gSpeed[0].step1 > Data_y[cur+count] )
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

	if(s3 || (PressedKey1p[3]==true))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=false;
		}
		else
		{
			s3++;
		}
		if(PressedKey1p[3]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[0].step3 < Data_y[cur+count] &&
				ZONE_D*gSpeed[0].step3 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[0].step3 < Data_y[cur+count] &&
				    PERFECT_ZONE_D*gSpeed[0].step3 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=true;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[0].step3<Data_y[cur+count] &&
					     GREAT_ZONE_D*gSpeed[0].step3>Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=true;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[0].step3 < Data_y[cur+count] &&
						 GOOD_ZONE_D*gSpeed[0].step3 > Data_y[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[0].step3<Data_y[cur+count] &&
						 BAD_ZONE_D*gSpeed[0].step3>Data_y[cur+count])
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

	if(s5 || (PressedKey1p[5]==true))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=false;
		}
		else
		{
			s5++;
		}
		if(PressedKey1p[5]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[0].step5<Data_y[cur+count] &&
				ZONE_D*gSpeed[0].step5>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[0].step5<Data_y[cur+count] &&
					PERFECT_ZONE_D*gSpeed[0].step5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=true;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[0].step5<Data_y[cur+count] &&
					     GREAT_ZONE_D*gSpeed[0].step5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=true;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[0].step5<Data_y[cur+count] &&
					     GOOD_ZONE_D*gSpeed[0].step5>Data_y[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[0].step5<Data_y[cur+count] &&
					     BAD_ZONE_D*gSpeed[0].step5>Data_y[cur+count])
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

	if(s7 || (PressedKey1p[7]==true) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=false;
		}
		else
		{
			s7++;
		}
		if(PressedKey1p[7]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[0].step7 < Data_y[cur+count] &&
				ZONE_D*gSpeed[0].step7 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[0].step7 < Data_y[cur+count] &&
					PERFECT_ZONE_D*gSpeed[0].step7 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=true;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[0].step7<Data_y[cur+count] &&
					     GREAT_ZONE_D*gSpeed[0].step7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=true;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[0].step7<Data_y[cur+count] &&
					     GOOD_ZONE_D*gSpeed[0].step7>Data_y[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[0].step7<Data_y[cur+count] &&
					     BAD_ZONE_D*gSpeed[0].step7>Data_y[cur+count])
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

	if(s9 || (PressedKey1p[9]==true))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=false;
		}
		else
		{
			s9++;
		}
		if(PressedKey1p[9]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[0].step9<Data_y[cur+count] &&
				ZONE_D*gSpeed[0].step9>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[0].step9<Data_y[cur+count] &&
					PERFECT_ZONE_D*gSpeed[0].step9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=true;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[0].step9<Data_y[cur+count] &&
					     GREAT_ZONE_D*gSpeed[0].step9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=true;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[0].step9<Data_y[cur+count] &&
					     GOOD_ZONE_D*gSpeed[0].step9>Data_y[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[0].step9<Data_y[cur+count] &&
					     BAD_ZONE_D*gSpeed[0].step9>Data_y[cur+count])
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


	if(Crash1 && s1==2) {
		if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' ) {
			Data[stat1][0]=Data[stat1][1]=Data[stat1][2]=Data[stat1][3]=Data[stat1][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crash7 && s7==2)	{
		if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )	{
			Data[stat7][0]=Data[stat7][1]=Data[stat7][2]=Data[stat7][3]=Data[stat7][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}
	if(Crash5 && s5==2)	{
		if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' ) {
			Data[stat5][0]=Data[stat5][1]=Data[stat5][2]=Data[stat5][3]=Data[stat5][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}
	if(Crash9 && s9==2)	{
		if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' ) {
			Data[stat9][0]=Data[stat9][1]=Data[stat9][2]=Data[stat9][3]=Data[stat9][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}
	if(Crash3 && s3==2)
	{
		if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' ) {
			Data[stat3][0]=Data[stat3][1]=Data[stat3][2]=Data[stat3][3]=Data[stat3][4]='0';
			//Judgement1p=PERFECT;
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Judgement1p==PERFECT || Judgement1p==GREAT)	{
        gBeat.Halt();
        gBeat.Play();

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

		On1=On3=On5=On7=On9=false;
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
					gSong.Halt();
					SongFlag=false;
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
					gSong.Halt();
					SongFlag=false;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(Score1p<0)Score1p=0;

	if (beat)
        gArrow2.BltFast( 32, 50, gScreen );
	else
        gArrow1.BltFast( 32, 50, gScreen );

	pArr1.y=0;
	pArr1.x=arrow_l[s1];
    pArr1.w=arrow_r[0];
	pArr1.h=70;

	pArr3.y=0;
	pArr3.x=arrow_l[s3];
	pArr3.w=arrow_r[0];
	pArr3.h=70;

	pArr5.y=0;
	pArr5.x=arrow_l[s5];
	pArr5.w=arrow_r[0];
	pArr5.h=70;

	pArr7.y=0;
	pArr7.x=arrow_l[s7];
	pArr7.w=arrow_r[0];
	pArr7.h=70;

	pArr9.y=0;
	pArr9.x=arrow_l[s9];
	pArr9.w=arrow_r[0];
	pArr9.h=70;

	cArr1.y=0;
	cArr1.x=Carrow_l[s1];
    cArr1.w=Carrow_r[0];
	cArr1.h=80;

	cArr3.y=0;
	cArr3.x=Carrow_l[s3];
	cArr3.w=Carrow_r[0];
	cArr3.h=80;

	cArr5.y=0;
	cArr5.x=Carrow_l[s5];
	cArr5.w=Carrow_r[0];
	cArr5.h=80;

	cArr7.y=0;
	cArr7.x=Carrow_l[s7];
	cArr7.w=Carrow_r[0];
	cArr7.h=80;

	cArr9.y=0;
	cArr9.x=Carrow_l[s9];
	cArr9.w=Carrow_r[0];
	cArr9.h=80;
   
    
	if(Crash1)cArrow1.BltFast( 25, 43, gScreen, &cArr1 );
	else if(s1)pArrow1.BltFast( 27, 45, gScreen, &pArr1 );
	
 	if(Crash7)cArrow7.BltFast( 75, 43, gScreen, &cArr7 );
	else if(s7)pArrow7.BltFast( 77, 45, gScreen, &pArr7 );
	
	if(Crash5)cArrow5.BltFast( 125, 43, gScreen, &cArr5 );
	else if(s5)pArrow5.BltFast( 127, 45, gScreen, &pArr5 );

	if(Crash9)cArrow9.BltFast( 175, 43, gScreen, &cArr9 );
	else if(s9)pArrow9.BltFast( 177, 45, gScreen, &pArr9 );
	
	if(Crash3)cArrow3.BltFast( 225, 43, gScreen, &cArr3 );
	else if(s3)pArrow3.BltFast( 227, 45, gScreen, &pArr3 );
 
}

void DrawArrow2p(Uint32 cur)
{
	static const int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static const int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720}; // DELETE ME

	static const int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static const int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};        // DELETE ME

	static Uint8 s1,s3,s5,s7,s9;
	static Uint32 stat1,stat3,stat5,stat7,stat9;
	static Uint32 cur2;
	static int beat;

	static bool Crash1, Crash3, Crash5, Crash7, Crash9;
	static bool	On1, On3, On5, On7, On9;

	static SDL_Rect pArr1, pArr3,pArr5,pArr7,pArr9;
	static SDL_Rect cArr1, cArr3,cArr5,cArr7,cArr9;


	Uint8 JudgeTemp=0;
	Uint8	count;

	
	if(Start2p==true && Start1p==false)ReadGameInput();
	if(cur2!=cur)
	{
		if(bModeRandomS2p == true)
		{
			gSpeed[1].step1 = gSpeed[1].step3 = gSpeed[1].step5 = gSpeed[1].step7 = gSpeed[1].step9 = 1 + rand() % 8 ;
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
	if(KCFG.auto1_2p==true)
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
					Crash1=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_2p==true)
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
					Crash7=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_2p==true)
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
					Crash5=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_2p==true)
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
					Crash9=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto3_2p==true)
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
					Crash3=true;
					break;
				}
			}
			
		}
	}

	if(s1 || (PressedKey2p[1]==true) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=false;
		}
		else
		{
			s1++;
		}
		if(PressedKey2p[1]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[1].step1 < Data_y1[cur+count] &&
				ZONE_D*gSpeed[1].step1 > Data_y1[cur+count]  )
			{
				if( PERFECT_ZONE_U*gSpeed[1].step1 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*gSpeed[1].step1 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=true;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[1].step1 < Data_y1[cur+count] &&
						 GREAT_ZONE_D*gSpeed[1].step1 > Data_y1[cur+count]  )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=true;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[1].step1 < Data_y1[cur+count] &&
					     GOOD_ZONE_D*gSpeed[1].step1 > Data_y1[cur+count] )
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
				else if( BAD_ZONE_U*gSpeed[1].step1 < Data_y1[cur+count] &&
						 BAD_ZONE_D*gSpeed[1].step1 > Data_y1[cur+count] )
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

	if(s3 || (PressedKey2p[3]==true))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=false;
		}
		else
		{
			s3++;
		}
		if(PressedKey2p[3]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[1].step3 < Data_y1[cur+count] &&
				ZONE_D*gSpeed[1].step3 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[1].step3 < Data_y1[cur+count] &&
				    PERFECT_ZONE_D*gSpeed[1].step3 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=true;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[1].step3<Data_y1[cur+count] &&
					     GREAT_ZONE_D*gSpeed[1].step3>Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=true;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[1].step3 < Data_y1[cur+count] &&
						 GOOD_ZONE_D*gSpeed[1].step3 > Data_y1[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[1].step3<Data_y1[cur+count] &&
						 BAD_ZONE_D*gSpeed[1].step3>Data_y1[cur+count])
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

	if(s5 || (PressedKey2p[5]==true))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=false;
		}
		else
		{
			s5++;
		}
		if(PressedKey2p[5]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[1].step5<Data_y1[cur+count] &&
				ZONE_D*gSpeed[1].step5>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[1].step5<Data_y1[cur+count] &&
					PERFECT_ZONE_D*gSpeed[1].step5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=true;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[1].step5<Data_y1[cur+count] &&
					     GREAT_ZONE_D*gSpeed[1].step5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=true;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[1].step5<Data_y1[cur+count] &&
					     GOOD_ZONE_D*gSpeed[1].step5>Data_y1[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[1].step5<Data_y1[cur+count] &&
					     BAD_ZONE_D*gSpeed[1].step5>Data_y1[cur+count])
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

	if(s7 || (PressedKey2p[7]==true) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=false;
		}
		else
		{
			s7++;
		}
		if(PressedKey2p[7]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[1].step7 < Data_y1[cur+count] &&
				ZONE_D*gSpeed[1].step7 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[1].step7 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*gSpeed[1].step7 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=true;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[1].step7<Data_y1[cur+count] &&
					     GREAT_ZONE_D*gSpeed[1].step7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=true;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[1].step7<Data_y1[cur+count] &&
					     GOOD_ZONE_D*gSpeed[1].step7>Data_y1[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[1].step7<Data_y1[cur+count] &&
					     BAD_ZONE_D*gSpeed[1].step7>Data_y1[cur+count])
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

	if(s9 || (PressedKey2p[9]==true))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=false;
		}
		else
		{
			s9++;
		}
		if(PressedKey2p[9]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*gSpeed[1].step9<Data_y1[cur+count] &&
				ZONE_D*gSpeed[1].step9>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*gSpeed[1].step9<Data_y1[cur+count] &&
					PERFECT_ZONE_D*gSpeed[1].step9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=true;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*gSpeed[1].step9<Data_y1[cur+count] &&
					     GREAT_ZONE_D*gSpeed[1].step9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=true;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*gSpeed[1].step9<Data_y1[cur+count] &&
					     GOOD_ZONE_D*gSpeed[1].step9>Data_y1[cur+count])
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
				else if( BAD_ZONE_U*gSpeed[1].step9<Data_y1[cur+count] &&
					     BAD_ZONE_D*gSpeed[1].step9>Data_y1[cur+count])
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

	if(Crash1 && s1==2)	{
		if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )	{
			Data1[stat1][5]=Data1[stat1][6]=Data1[stat1][7]=Data1[stat1][8]=Data1[stat1][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		} else {
			Judgement2p=NONE;
		}
	}

	if(Crash7 && s7==2)	{
		if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
		{
			Data1[stat7][5]=Data1[stat7][6]=Data1[stat7][7]=Data1[stat7][8]=Data1[stat7][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		} else {
			Judgement2p=NONE;
		}
	}

	if(Crash5 && s5==2) {
		if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )	{
			Data1[stat5][5]=Data1[stat5][6]=Data1[stat5][7]=Data1[stat5][8]=Data1[stat5][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		} else {
			Judgement2p=NONE;
		}
	}

	if(Crash9 && s9==2)	{
		if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' ) {
			Data1[stat9][5]=Data1[stat9][6]=Data1[stat9][7]=Data1[stat9][8]=Data1[stat9][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		} else {
			Judgement2p=NONE;
		}
	}

	if(Crash3 && s3==2) {
		if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' ) {
			Data1[stat3][5]=Data1[stat3][6]=Data1[stat3][7]=Data1[stat3][8]=Data1[stat3][9]='0';
			//Judgement2p=PERFECT;
			Judgement2p=JudgeTemp;
		} else {
			Judgement2p=NONE;
		}
	}

	if(Judgement2p==PERFECT || Judgement2p==GREAT) {
        gBeat.Halt();
        gBeat.Play();

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

		On1=On3=On5=On7=On9=false;
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
					gSong.Halt();
					SongFlag=false;
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
					gSong.Halt();
					SongFlag=false;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(Score2p<0)Score2p=0;

	if (beat)
        gArrow2.BltFast( 352, 50, gScreen );
	else
        gArrow1.BltFast( 352, 50, gScreen );

	pArr1.y=0;
	pArr1.x=arrow_l[s1];
	pArr1.w=arrow_r[0];
	pArr1.h=70;

 	pArr3.y=0;
	pArr3.x=arrow_l[s3];
	pArr3.w=arrow_r[0];
	pArr3.h=70;

	pArr5.y=0;
	pArr5.x=arrow_l[s5];
	pArr5.w=arrow_r[0];
	pArr5.h=70;

	pArr7.y=0;
	pArr7.x=arrow_l[s7];
	pArr7.w=arrow_r[0];
	pArr7.h=70;

	pArr9.y=0;
	pArr9.x=arrow_l[s9];
	pArr9.w=arrow_r[0];
	pArr9.h=70;

	cArr1.y=0;
	cArr1.x=Carrow_l[s1];
	cArr1.w=Carrow_r[0];
	cArr1.h=80;

	cArr3.y=0;
	cArr3.x=Carrow_l[s3];
	cArr3.w=Carrow_r[0];
	cArr3.h=80;

	cArr5.y=0;
	cArr5.x=Carrow_l[s5];
	cArr5.w=Carrow_r[0];
	cArr5.h=80;

	cArr7.y=0;
	cArr7.x=Carrow_l[s7];
	cArr7.w=Carrow_r[0];
	cArr7.h=80;

	cArr9.y=0;
	cArr9.x=Carrow_l[s9];
	cArr9.w=Carrow_r[0];
	cArr9.h=80;

	if(Crash1)  cArrow1.BltFast( 345, 43,  gScreen, &cArr1 );
	else if(s1) pArrow1.BltFast( 347, 45,  gScreen, &pArr1 );

	if(Crash7)  cArrow7.BltFast( 395, 43,  gScreen, &cArr7 );
	else if(s7) pArrow7.BltFast( 397, 45,  gScreen, &pArr7 );
	
	if(Crash5)  cArrow5.BltFast( 445, 43,  gScreen, &cArr5 );
	else if(s5) pArrow5.BltFast( 447, 45,  gScreen, &pArr5 );

	if(Crash9)  cArrow9.BltFast( 495, 43,  gScreen, &cArr9 );
	else if(s9) pArrow9.BltFast( 497, 45,  gScreen, &pArr9 );
	
	if(Crash3)  cArrow3.BltFast( 545, 43,  gScreen, &cArr3 );
	else if(s3) pArrow3.BltFast( 547, 45,  gScreen, &pArr3 );
}

void UpdateFrame(void)
{
	// FPS count start
	static Uint32 lastTime, fpsTime,framesRendered,fps;

	char	buff[50];
	Uint32	cur = SDL_GetTicks();
	Uint32	deltaTime = cur - lastTime;
	lastTime = cur;

	fpsTime += deltaTime;

	++framesRendered;

	if(fpsTime>1000) {
		fps = framesRendered;
		framesRendered = 0;
		fpsTime = 0;
	}

	// FPS count & print end
	sprintf(buff,"FPS:%3d",fps);
	DisplayMessage(583,463,buff);

	FMOD_System_Update(gpSystem);
	// gpSystem->update();
	switch(g_ProgramState) {
		case GAMETITLE:
			StageTitle();
			break;
		case SELECTSONG:
			SelectSong();
			break;
		case STAGE1:
			KIU_STAGE();
			break;
		case DOUBLEST:
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
            SDL_Event   event;
            event.type = SDL_QUIT;
            SDL_PushEvent( &event );
			break;
		default:
			break;
	}

}

bool KLoadImage(void)
{
	// Loading image here
	
	if( !gGameTitle.LoadBmp( "images/title.bmp" ) )
		return false;
	
	if( !gStateComment.LoadBmp( "images/c_font.bmp" ) )
		return false;
	gStateComment.SetColorKey();
	
	if( !gSmallFont.LoadBmp( "images/s_font.bmp" ) )
		return false;
	gSmallFont.SetColorKey();
	
	if( !gSelectBack.LoadBmp( "images/select_back.bmp" ) )
	return false;
	
	DisplayMessage(0,0,"Loading Image");
	
	if( !JudgeFont.LoadBmp( "images/judgement.bmp" ) ) {
		MsgBox("Cannot Load judgement Font.","ERROR",0);
		return false;
	}
	JudgeFont.SetColorKey();
	
	if( !gNumberFont.LoadBmp( "images/number.bmp" ) ) {
		MsgBox("Cannot Load number Font.","ERROR",0);
		return false;
	}
	gNumberFont.SetColorKey();
	
	if( !ComboFont.LoadBmp( "images/combo.bmp" ) ) {
		MsgBox("Cannot Load combo Font.","ERROR",0);
		return false;
	}
	ComboFont.SetColorKey();
	
	if( !gNoDISC.LoadBmp( "images/no_disc.bmp" ) ) {
		MsgBox("Cannot Load nodisc.bmp",0,0);
		return false;
	}
	gNoDISC.SetColorKey();
	
	if( !gShiftLeft.LoadBmp( "images/shift_l.bmp" ) ) {
		MsgBox("Cannot Load shift_l.bmp",0,0);
		return false;
	}
	gShiftLeft.SetColorKey();
	
	if( !gShiftRight.LoadBmp( "images/shift_r.bmp" ) ) {
		MsgBox("Cannot Load shift_r.bmp",0,0);
		return false;
	}
	gShiftRight.SetColorKey();
	
	if( !GaugeWaku.LoadBmp( "images/gauge_waku.bmp" ) ) {
		MsgBox("Cannot Load gauge_waku.bmp",0,0);
		return false;
	}
	GaugeWaku.SetColorKey();
	
	if( !Gauge.LoadBmp( "images/gauge.bmp" ) ) {
		MsgBox("Cannot Load gauge.bmp",0,0);
		return false;
	}
	Gauge.SetColorKey();
	
	if( !gArrow1.LoadBmp( "images/arrow1.bmp" ) ) {
		MsgBox("Cannot Load arrow1.bmp",0,0);
		return false;
	}
	gArrow1.SetColorKey();
	
	if( !gArrow2.LoadBmp( "images/arrow2.bmp" ) ) {
		MsgBox("Cannot Load Arrow2.bmp",0,0);
		return false;
	}
	gArrow2.SetColorKey();
	
	if( !gWArrow.LoadBmp( "images/arrow.bmp" ) ) {
		MsgBox("Cannot Load arrow.bmp",0,0);
		return false;
	}
	gWArrow.SetColorKey();
	
	if( !pArrow1.LoadBmp( "images/p_arrow1.bmp" ) ) {
		MsgBox("Cannot Load p_arrow1.bmp",0,0);
		return false;
	}
	pArrow1.SetColorKey();
	
	if( !pArrow3.LoadBmp( "images/p_arrow3.bmp" ) ) {
		MsgBox("Cannot Load p_arrow3.bmp",0,0);
		return false;
	}
	pArrow3.SetColorKey();
	
	if( !pArrow5.LoadBmp( "images/p_arrow5.bmp" ) ) {
		MsgBox("Cannot Load p_arrow5.bmp",0,0);
		return false;
	}
	pArrow5.SetColorKey();
	
	if( !pArrow7.LoadBmp( "images/p_arrow7.bmp" ) ) {
		MsgBox("Cannot Load p_arrow7.bmp",0,0);
		return false;
	}
	pArrow7.SetColorKey();
	
	if( !pArrow9.LoadBmp( "images/p_arrow9.bmp" ) ) {
		MsgBox("Cannot Load p_arrow9.bmp",0,0);
		return false;
	}
	pArrow9.SetColorKey();
	
	if( !cArrow1.LoadBmp( "images/c_arrow1.bmp" ) ) {
		MsgBox("Cannot Load c_arrow.bmp",0,0);
		return false;
	}
	cArrow1.SetColorKey();
	
	if( !cArrow3.LoadBmp( "images/c_arrow3.bmp" ) ) {
		MsgBox("Cannot Load c_arrow3.bmp",0,0);
		return false;
	}
	cArrow3.SetColorKey();
	
	if( !cArrow5.LoadBmp( "images/c_arrow5.bmp" ) ) {
		MsgBox("Cannot Load c_arrow5.bmp",0,0);
		return false;
	}
	cArrow5.SetColorKey();
	
	if( !cArrow7.LoadBmp( "images/c_arrow7.bmp" ) ) {
		MsgBox("Cannot Load c_arrow7.bmp",0,0);
		return false;
	}
	cArrow7.SetColorKey();
	
	if( !cArrow9.LoadBmp( "images/c_arrow9.bmp" ) ) {
	MsgBox("Cannot Load c_arrow9.bmp",0,0);
	return false;
	}
	cArrow9.SetColorKey();
	
	
	if( !gModeIcon.LoadBmp( "images/mode_icon.bmp" ) ) {
		MsgBox("Cannot Load mode_Icon.bmp",0,0);
		return false;
	}
	gModeIcon.SetColorKey();
	
	if( !ResultFont.LoadBmp( "images/res_font.bmp" ) ) {
		MsgBox("Cannot Load res_font.bmp",0,0);
		return	false;
	}
	ResultFont.SetColorKey();
	
	if( !ResultBack.LoadBmp( "images/res_back.bmp" ) ) {
		MsgBox("Cannot Load res_back.bmp",0,0);
		return false;
	}
	if( !gStageCount.LoadBmp( "images/stage_count.bmp" ) ) {
		MsgBox("Cannot Load stage_count.bmp",0,0);
		return false;
	}
	gStageCount.SetColorKey();
	
	if( !DeadScreen.LoadBmp( "images/dead.bmp" ) ) {
		MsgBox("Cannot Load dead.bmp",0,0);
		return false;
	}
	
	if( !GameOver.LoadBmp( "images/game_over.bmp" ) ) {
		MsgBox("Cannot Load game_over.bmp",0,0);
		return false;
	 }
	
	 if( !Score.LoadBmp( "images/score.bmp" ) ) {
		MsgBox("Cannot Load score.bmp",0,0);
		return false;
	}
	Score.SetColorKey();

	if( !gDoubleIcon.LoadBmp("images/double_icon.bmp") ) {
		MsgBox( "Cannot Load double_icon.bmp", 0, 0 );
	}
	gDoubleIcon.SetColorKey();

	if( !gCrazyIcon.LoadBmp("images/crazy_icon.bmp") )  {
		MsgBox( "Cannot Load crazy_icon.bmp", 0, 0 );
	}
    	gCrazyIcon.SetColorKey();

    	if( !gEasyIcon.LoadBmp("images/easy_icon.bmp") ) {
		MsgBox( "Cannot Load crazy_icon.bmp", 0, 0 );
	}
    	gEasyIcon.SetColorKey();

	if( !gHardIcon.LoadBmp("images/hard_icon.bmp") ) {
		MsgBox( "Cannot Load hard_icon.bmp", 0, 0 );
	}
    	gHardIcon.SetColorKey();

	return true;
}

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}


bool init()
{
    // Start SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 || !SDL_GetVideoInfo() )
        return false;

	if( ! gScreen.SetVideoMode( 640, 480, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_RESIZABLE ) )
		return false;

    // initialize Sound
	
	ERRCHECK(  FMOD_System_Create(&gpSystem) );
	unsigned int      version;
	ERRCHECK( FMOD_System_GetVersion(gpSystem, &version) );

	if (version < FMOD_VERSION)
	{
		char	buff[128];
		sprintf(buff, "Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		MsgBox( buff, "error", 0 );
		return 0;
	}

	ERRCHECK(
			FMOD_System_Init(gpSystem, 32, FMOD_INIT_NORMAL, 0)
			);
	// ERRCHECK(gpSystem->init(32, FMOD_INIT_NORMAL, 0));


    sprintf(TITLE, "KICK IT UP! beta version v%s - Minor release / Compiled at %s / %s", VER_NUM, __DATE__, __TIME__ );
    
    SDL_WM_SetCaption( TITLE, NULL );

    if( !KLoadImage() )
        return false;

    Read();

    WaveSet_Loading();

    CFGInitialize();
    g_bActive = true;

    return true;
}

void clean_up()
{
    gOpening.Free();
    gDead.Free();
    gMode.Free();
    gCancel.Free();
    gMove.Free();
    gBeat.Free();
    gSelectSong.Free();

    gIntro.Free();
	gSong.Free();

    gSongTitle.Free();
    gSongBack.Free();
    gSelectBack.Free();
    gNumberFont.Free();
    gSmallFont.Free();
    gStateComment.Free();
    gGameTitle.Free();
    gScreen.Free();
    gNoDISC.Free();
    JudgeFont.Free();
    ComboFont.Free();
    GaugeWaku.Free();
    Gauge.Free();

    gArrow1.Free();
    gArrow2.Free();
    gWArrow.Free();
    pArrow1.Free();
    pArrow3.Free();
    pArrow5.Free();
    pArrow7.Free();
    pArrow9.Free();
    cArrow1.Free();
    cArrow3.Free();
    cArrow5.Free();
    cArrow7.Free();
    cArrow9.Free();
    gModeIcon.Free();

    gShiftLeft.Free();
    gShiftRight.Free();
    gDoubleIcon.Free();
    gCrazyIcon.Free();
    gEasyIcon.Free();
    gHardIcon.Free();
    GameOver.Free();
    DeadScreen.Free();
    ResultFont.Free();
    ResultBack.Free();
    gStageCount.Free();
    Score.Free();

    ERRCHECK(FMOD_System_Close(gpSystem));
    ERRCHECK(FMOD_System_Release(gpSystem));

//	ERRCHECK(gpSystem->close());
//	ERRCHECK(gpSystem->release());

    SDL_Quit();
}

void process_Keydown( const SDLKey & keyValue )
{
    if( keyValue == SDLK_ESCAPE ) {
        switch(g_ProgramState) {
            case GAMETITLE:
                CFGWrite();
                SDL_Event   event;
                event.type = SDL_QUIT;
                SDL_PushEvent( &event );
                break;

            case SELECTSONG:
                g_ProgramState=GAMETITLE;
                if(IntroFlag){
                    gIntro.Halt();
                    IntroFlag=false;
                }
                gSelectSong.Halt();
                First=0;
                break;

            case STAGE1:
                if(SongFlag) {
                    gSong.Halt();
                    SongFlag=false;
                }
                First=0;
                g_ProgramState=RESULT;
                break;
            case DOUBLEST:
                if(SongFlag){
                    gSong.Halt();
                    SongFlag=false;
                }
                First=0;
                g_ProgramState=RESULT;
                break;
            case COUPLE:
                if(SongFlag) {
                    gSong.Halt();
                    SongFlag=false;
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

int main( int argc, char * argv[] )
{

    if( init() == false )
        return 1;

	int		frame = 0;	// Keep track of the frame count
	Timer	fps;		// Timer used to calculate the frames per second
	Timer	update;		// Timer used to update the caption

	update.Start();
	fps.Start();

    bool quit = false;

    std::string strFps;

    while( !quit ) {
        SDL_Event   event;
		fps.Start();

        while( SDL_PollEvent( &event)) {
            switch( event.type ) {
                case SDL_KEYDOWN:
                    process_Keydown( event.key.keysym.sym );
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }            
		}

		if( g_bActive )
			UpdateFrame();
		else
			SDL_Delay( 0 );

		++frame;	// Increment the frame counter

		if( 1000 < update.GetTicks() ) {
			std::stringstream	buf;
			buf << "FPS :" << frame / (update.GetTicks() / 1000.0);
            strFps = buf.str();
			update.Start();
			frame = 0;
		}
        DisplayMessage( 10, 10, strFps.c_str() );

		gScreen.Flip();
		
		Uint32 curFps = static_cast<Uint32>( 1000 / 60 );
		if( fps.GetTicks() < curFps )	//60fps
			SDL_Delay( curFps - fps.GetTicks() );
    }

    clean_up();
    
    return 0;
}
