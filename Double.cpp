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

#include <stdio.h>
#include <time.h>

#include "Common.h"
#include "Select.h"
#include "Double.h"
#include "Main.h"
#include "Input.h"
#include "Config.h"
#include "Surface.h"
#include "Chunk.h"

#define LP1DB1_X		62
#define LP7DB1_X		62+PUMP_SPRITE
#define LP5DB1_X		63+PUMP_SPRITE*2
#define LP9DB1_X		67+PUMP_SPRITE*3
#define	LP3DB1_X		67+PUMP_SPRITE*4

#define LP1DB2_X		318
#define LP7DB2_X		318+PUMP_SPRITE
#define LP5DB2_X		320+PUMP_SPRITE*2
#define LP9DB2_X		323+PUMP_SPRITE*3
#define	LP3DB2_X		323+PUMP_SPRITE*4


extern	int					start,start2,start3,tick;
extern	Uint32				bunki,bunki2;
extern	int					start1;
extern	double				bpm;
extern	double				bpm2;
extern	double				bpm3;

extern	Surface gWArrow;

extern	Surface	cArrow1;
extern	Surface	cArrow3;
extern	Surface	cArrow5;
extern	Surface	cArrow7;
extern	Surface	cArrow9;
extern	Surface	pArrow1;
extern	Surface	pArrow3;
extern	Surface	pArrow5;
extern	Surface	pArrow7;
extern	Surface	pArrow9;

extern	Surface 	gArrow1;
extern	Surface 	gArrow2;

extern Surface      gSongBack;
extern Surface      JudgeFont;
extern	Surface	ComboFont;

extern Surface	GaugeWaku;
extern Surface	Gauge;

extern Chunk    gBeat;


int		HighSpeed;

int		HighSpeed_1;
int		HighSpeed_3;
int		HighSpeed_5;
int		HighSpeed_7;
int		HighSpeed_9;

bool	bModeMirror;
bool	bModeNonstep;
bool	bModeUnion;
bool	bModeRandom;
bool	b4dMix;
bool	bModeVanish;

extern int	dwGameCount;
extern	KIUCONFIG	KCFG;

void DrawGaugeDB_1p(void)
{
	int CurG;
	int i;
	SDL_Rect sRect;

	CurG=Gauge1p;
	
	if(CurG<0)CurG=0;

	// g_pDDSBack->BltFast(64,0,GaugeWaku,NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    GaugeWaku.BltFast( 64, 0, gScreen );
	
	sRect.y=0;
	sRect.x=0;
	sRect.w=6;
	sRect.h=20;

	for(i=0;i<7;i++)
	{
		if(i>CurG)break;
        //		g_pDDSBack->BltFast(312-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 312-(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=6;
	sRect.w=6;
	sRect.h=20;

	for(i=7;i<21;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(312-(i*6),20,Gauge,&sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 312-(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=12;
	sRect.w=6;
	sRect.h=20;

	for(i=21;i<42;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(312-(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 312-(i*6), 20, gScreen, &sRect );
	}

}

void DrawGaugeDB_2p(void)
{
	int CurG;
	int i;
	SDL_Rect sRect;

	CurG=Gauge1p;
	
	if(CurG<0)CurG=0;

	// g_pDDSBack->BltFast(322,0,GaugeWaku,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    GaugeWaku.BltFast( 322, 0, gScreen );
	
	sRect.y=0;
	sRect.x=0;
	sRect.w=6;
	sRect.h=20;

	for(i=0;i<7;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(322+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 322+(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=6;
	sRect.w=6;
	sRect.h=20;

	for(i=7;i<21;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(322+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 322+(i*6), 20, gScreen, &sRect );
	}

	sRect.y=0;
	sRect.x=12;
	sRect.w=6;
	sRect.h=20;

	for(i=21;i<42;i++)
	{
		if(i>CurG)break;
		// g_pDDSBack->BltFast(322+(i*6),20,Gauge,&sRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
        Gauge.BltFast( 322+(i*6), 20, gScreen, &sRect );
	}

}
void DrawJudgeDB(void)
{
	static char LastJudge;

	char chCombo1p[255];

	static Uint32 sec,delta;

	SDL_Rect rRect, cRect, destRect;
	int Loop;

	if(Judgement1p)
	{
		dwState=0;
	}
	else if(dwState)
	{
		Judgement1p=LastJudge;

		delta=SDL_GetTicks()-sec;
		if(delta>16)
		{
			sec=SDL_GetTicks();
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

	if(dwState>15)
	{
		destRect.y=200;
		destRect.x=200;
		destRect.w=JUDGE_SIZE_X;
		destRect.h=JUDGE_SIZE_Y;
	}
	else
	{
		destRect.y= static_cast<int>( 200-30+(dwState*2) );
		destRect.x= static_cast<int>( 200-60+(dwState*4) );
		destRect.w=JUDGE_SIZE_X;
		destRect.h=JUDGE_SIZE_Y;
	}

	if(Judgement1p)
	{
        JudgeFont.BltFast( destRect.x, destRect.y, gScreen, &rRect );

		/* 콤보 출력부 입니다. */
		if((Judgement1p==PERFECT || Judgement1p==GREAT) && Combo1p>3)
		{
			sprintf(chCombo1p,"%03d",Combo1p);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo1p[Loop]==0)break;
				chCombo1p[Loop]-=48;
				cRect.x=chCombo1p[Loop]*50;
				cRect.w=50;
				cRect.y=0;
				cRect.h=65;

				if(dwState>10)  ComboFont.BltFast( 250+Loop*50, 250+dwState*2-dwState*2, gScreen, &cRect );
				else            ComboFont.BltFast( 250+Loop*50, 250+dwState*2, gScreen, &cRect );

				cRect.x=0;
				cRect.w=150;
				cRect.y=65;
				cRect.h=100-65;
				if(dwState>10)  ComboFont.BltFast( 250,320+dwState*2-dwState*2, gScreen, &cRect );
				else            ComboFont.BltFast( 250,320+dwState*2,gScreen, &cRect );
			}
		}
	}

	Judgement1p=NONE;
}

void DrawArrow_DB(Uint32 cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720};

	static int cArrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int cArrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};

	static Uint8 sl1,sl3,sl5,sl7,sl9;
	static Uint8 sr1,sr3,sr5,sr7,sr9;

	static Uint32 statl1,statl3,statl5,statl7,statl9;
	static Uint32 statr1,statr3,statr5,statr7,statr9;

	static bool	Onl1, Onl3, Onl5, Onl7, Onl9;
	static bool	Onr1, Onr3, Onr5, Onr7, Onr9;

	static Uint32 cur2;
	static int beat;

	static bool Crashl1, Crashl3, Crashl5, Crashl7, Crashl9;
	static bool Crashr1, Crashr3, Crashr5, Crashr7, Crashr9;

    static SDL_Rect pArrl1;
    static SDL_Rect cArrl1;

	static SDL_Rect pArrl3,pArrl5,pArrl7,pArrl9;
	static SDL_Rect cArrl3,cArrl5,cArrl7,cArrl9;

	/*
	static SDL_Rect pArrr1, pArrr3,pArrr5,pArrr7,pArrr9;
	static SDL_Rect cArrr1, cArrr3,cArrr5,cArrr7,cArrr9;
	*/

//	KCFG.auto1_2p = KCFG.auto3_2p = KCFG.auto5_2p = KCFG.auto7_2p = KCFG.auto9_2p = true;

/*	int KCFG.auto1,KCFG.auto3,KCFG.auto5,KCFG.auto7,KCFG.auto9;
		KCFG.auto1=KCFG.auto3=KCFG.auto5=KCFG.auto7=KCFG.auto9=false;
*/
	Uint8 JudgeTemp=0;
	Uint8	count;

	if(cur2!=cur)
	{
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
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][0]=='1')
				{
					Data_Double_Judge[cur+count][0]='0';
					statl1=cur+count;
					sl1=1;
					Crashl1=true;
					break;
				}
			}
		}
	}

	if(KCFG.auto7_1p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][1]=='1')
				{
					Data_Double_Judge[cur+count][1]='0';
					statl7=cur+count;
					sl7=1;
					Crashl7=true;
					break;
				}
			}
		}
	}

	if(KCFG.auto5_1p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][2]=='1')
				{
					Data_Double_Judge[cur+count][2]='0';
					statl5=cur+count;
					sl5=1;
					Crashl5=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_1p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][3]=='1')
				{
					Data_Double_Judge[cur+count][3]='0';
					statl9=cur+count;
					sl9=1;
					Crashl9=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto3_1p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][4]=='1')
				{
					Data_Double_Judge[cur+count][4]='0';
					statl3=cur+count;
					sl3=1;
					Crashl3=true;
					break;
				}
			}
			
		}
	}

// 2 player
	if(KCFG.auto1_2p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][5]=='1')
				{
					Data_Double_Judge[cur+count][5]='0';
					statr1=cur+count;
					sr1=1;
					Crashr1=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_2p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][6]=='1')
				{
					Data_Double_Judge[cur+count][6]='0';
					statr7=cur+count;
					sr7=1;
					Crashr7=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_2p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][7]=='1')
				{
					Data_Double_Judge[cur+count][7]='0';
					statr5=cur+count;
					sr5=1;
					Crashr5=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_2p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][8]=='1')
				{
					Data_Double_Judge[cur+count][8]='0';
					statr9=cur+count;
					sr9=1;
					Crashr9=true;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto3_2p==true)
	{
		for(count=0;count<10;count++)
		{
			if(Data_Double_y[cur+count]<50)
			{
				if(Data_Double_Judge[cur+count][9]=='1')
				{
					Data_Double_Judge[cur+count][9]='0';
					statr3=cur+count;
					sr3=1;
					Crashr3=true;
					break;
				}
			}
			
		}
	}

	if(sl1 || (PressedKey1p[1]==true) )
	{
		if(sl1==20)
		{
			sl1=0;
			Crashl1=false;
		}
		else
		{
			sl1++;
		}
		if(PressedKey1p[1]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][0]=='1')
					{
						Data_Double_Judge[cur+count][0]='0';
						statl1=cur+count;
						Onl1=true;
						sl1=1;
						if(Data_Double_Judge[statl1][0]=='0' && Data_Double_Judge[statl1][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl1][3]=='0' && Data_Double_Judge[statl1][4]=='0'
							&& Data_Double_Judge[statl1][5]=='0' && Data_Double_Judge[statl1][6]=='0' && Data_Double_Judge[statl1][7]=='0' && Data_Double_Judge[statl1][8]=='0' && Data_Double_Judge[statl1][9]=='0')
						{
							Crashl1=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][0]=='1')
					{
						Data_Double_Judge[cur+count][0]='0';
						statl1=cur+count;
						Onl1=true;
						sl1=1;
						if(Data_Double_Judge[statl1][0]=='0' && Data_Double_Judge[statl1][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl1][3]=='0' && Data_Double_Judge[statl1][4]=='0'
							&& Data_Double_Judge[statl1][5]=='0' && Data_Double_Judge[statl1][6]=='0' && Data_Double_Judge[statl1][7]=='0' && Data_Double_Judge[statl1][8]=='0' && Data_Double_Judge[statl1][9]=='0')
						{
							Crashl1=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_1<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_1>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][0]=='1')
					{
						Data_Double_Judge[cur+count][0]='0';
						statl1=cur+count;
						sl1=1;
						if(Data_Double_Judge[statl1][0]=='0' && Data_Double_Judge[statl1][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl1][3]=='0' && Data_Double_Judge[statl1][4]=='0'
							&& Data_Double_Judge[statl1][5]=='0' && Data_Double_Judge[statl1][6]=='0' && Data_Double_Judge[statl1][7]=='0' && Data_Double_Judge[statl1][8]=='0' && Data_Double_Judge[statl1][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_1<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_1>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][0]=='1')
					{
						Data_Double_Judge[cur+count][0]='0';
						statl1=cur+count;
						sl1=1;
						if(Data_Double_Judge[statl1][0]=='0' && Data_Double_Judge[statl1][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl1][3]=='0' && Data_Double_Judge[statl1][4]=='0'
							&& Data_Double_Judge[statl1][5]=='0' && Data_Double_Judge[statl1][6]=='0' && Data_Double_Judge[statl1][7]=='0' && Data_Double_Judge[statl1][8]=='0' && Data_Double_Judge[statl1][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sl7 || (PressedKey1p[7]==true) )
	{
		if(sl7==20)
		{
			sl7=0;
			Crashl7=false;
		}
		else
		{
			sl7++;
		}
		if(PressedKey1p[7]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][1]=='1')
					{
						Data_Double_Judge[cur+count][1]='0';
						statl7=cur+count;
						Onl7=true;
						sl7=1;
						if(Data_Double_Judge[statl7][0]=='0' && Data_Double_Judge[statl7][1]=='0' && Data_Double_Judge[statl7][2]=='0' && Data_Double_Judge[statl7][3]=='0' && Data_Double_Judge[statl7][4]=='0'
							&& Data_Double_Judge[statl7][5]=='0' && Data_Double_Judge[statl7][6]=='0' && Data_Double_Judge[statl7][7]=='0' && Data_Double_Judge[statl7][8]=='0' && Data_Double_Judge[statl7][9]=='0')
						{
							Crashl7=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][1]=='1')
					{
						Data_Double_Judge[cur+count][1]='0';
						statl7=cur+count;
						Onl7=true;
						sl7=1;
						if(Data_Double_Judge[statl7][0]=='0' && Data_Double_Judge[statl7][1]=='0' && Data_Double_Judge[statl7][2]=='0' && Data_Double_Judge[statl7][3]=='0' && Data_Double_Judge[statl7][4]=='0'
							&& Data_Double_Judge[statl7][5]=='0' && Data_Double_Judge[statl7][6]=='0' && Data_Double_Judge[statl7][7]=='0' && Data_Double_Judge[statl7][8]=='0' && Data_Double_Judge[statl7][9]=='0')
						{
							Crashl7=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_7<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_7>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][1]=='1')
					{
						Data_Double_Judge[cur+count][1]='0';
						statl7=cur+count;
						sl7=1;
						if(Data_Double_Judge[statl7][0]=='0' && Data_Double_Judge[statl7][1]=='0' && Data_Double_Judge[statl7][2]=='0' && Data_Double_Judge[statl7][3]=='0' && Data_Double_Judge[statl7][4]=='0'
							&& Data_Double_Judge[statl7][5]=='0' && Data_Double_Judge[statl7][6]=='0' && Data_Double_Judge[statl7][7]=='0' && Data_Double_Judge[statl7][8]=='0' && Data_Double_Judge[statl7][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_7<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_7>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][1]=='1')
					{
						Data_Double_Judge[cur+count][1]='0';
						statl7=cur+count;
						sl7=1;
						if(Data_Double_Judge[statl7][0]=='0' && Data_Double_Judge[statl7][1]=='0' && Data_Double_Judge[statl7][2]=='0' && Data_Double_Judge[statl7][3]=='0' && Data_Double_Judge[statl7][4]=='0'
							&& Data_Double_Judge[statl7][5]=='0' && Data_Double_Judge[statl7][6]=='0' && Data_Double_Judge[statl7][7]=='0' && Data_Double_Judge[statl7][8]=='0' && Data_Double_Judge[statl7][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sl5 || (PressedKey1p[5]==true) )
	{
		if(sl5==20)
		{
			sl5=0;
			Crashl5=false;
		}
		else
		{
			sl5++;
		}
		if(PressedKey1p[5]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][2]=='1')
					{
						Data_Double_Judge[cur+count][2]='0';
						statl5=cur+count;
						Onl5=true;
						sl5=1;
						if(Data_Double_Judge[statl5][0]=='0' && Data_Double_Judge[statl5][1]=='0' && Data_Double_Judge[statl5][2]=='0' && Data_Double_Judge[statl5][3]=='0' && Data_Double_Judge[statl5][4]=='0'
							&& Data_Double_Judge[statl5][5]=='0' && Data_Double_Judge[statl5][6]=='0' && Data_Double_Judge[statl5][7]=='0' && Data_Double_Judge[statl5][8]=='0' && Data_Double_Judge[statl5][9]=='0')
						{
							Crashl5=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][2]=='1')
					{
						Data_Double_Judge[cur+count][2]='0';
						statl5=cur+count;
						Onl5=true;
						sl5=1;
						if(Data_Double_Judge[statl5][0]=='0' && Data_Double_Judge[statl5][1]=='0' && Data_Double_Judge[statl5][2]=='0' && Data_Double_Judge[statl5][3]=='0' && Data_Double_Judge[statl5][4]=='0'
							&& Data_Double_Judge[statl5][5]=='0' && Data_Double_Judge[statl5][6]=='0' && Data_Double_Judge[statl5][7]=='0' && Data_Double_Judge[statl5][8]=='0' && Data_Double_Judge[statl5][9]=='0')
						{
							Crashl5=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_5<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_5>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][2]=='1')
					{
						Data_Double_Judge[cur+count][2]='0';
						statl5=cur+count;
						sl5=1;
						if(Data_Double_Judge[statl5][0]=='0' && Data_Double_Judge[statl5][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl5][3]=='0' && Data_Double_Judge[statl5][4]=='0'
							&& Data_Double_Judge[statl5][5]=='0' && Data_Double_Judge[statl5][6]=='0' && Data_Double_Judge[statl5][7]=='0' && Data_Double_Judge[statl5][8]=='0' && Data_Double_Judge[statl5][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_5<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_5>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][2]=='1')
					{
						Data_Double_Judge[cur+count][2]='0';
						statl5=cur+count;
						sl5=1;
						if(Data_Double_Judge[statl5][0]=='0' && Data_Double_Judge[statl5][1]=='0' && Data_Double_Judge[statl5][2]=='0' && Data_Double_Judge[statl5][3]=='0' && Data_Double_Judge[statl5][4]=='0'
							&& Data_Double_Judge[statl5][5]=='0' && Data_Double_Judge[statl5][6]=='0' && Data_Double_Judge[statl5][7]=='0' && Data_Double_Judge[statl5][8]=='0' && Data_Double_Judge[statl5][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sl9 || (PressedKey1p[9]==true) )
	{
		if(sl9==20)
		{
			sl9=0;
			Crashl9=false;
		}
		else
		{
			sl9++;
		}
		if(PressedKey1p[9]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
				    PERFECT_ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][3]=='1')
					{
						Data_Double_Judge[cur+count][3]='0';
						statl9=cur+count;
						Onl9=true;
						sl9=1;
						if(Data_Double_Judge[statl9][0]=='0' && Data_Double_Judge[statl9][1]=='0' && Data_Double_Judge[statl9][2]=='0' && Data_Double_Judge[statl9][3]=='0' && Data_Double_Judge[statl9][4]=='0'
							&& Data_Double_Judge[statl9][5]=='0' && Data_Double_Judge[statl9][6]=='0' && Data_Double_Judge[statl9][7]=='0' && Data_Double_Judge[statl9][8]=='0' && Data_Double_Judge[statl9][9]=='0')
						{
							Crashl9=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][3]=='1')
					{
						Data_Double_Judge[cur+count][3]='0';
						statl9=cur+count;
						Onl9=true;
						sl9=1;
						if(Data_Double_Judge[statl9][0]=='0' && Data_Double_Judge[statl9][1]=='0' && Data_Double_Judge[statl9][2]=='0' && Data_Double_Judge[statl9][3]=='0' && Data_Double_Judge[statl9][4]=='0'
							&& Data_Double_Judge[statl9][5]=='0' && Data_Double_Judge[statl9][6]=='0' && Data_Double_Judge[statl9][7]=='0' && Data_Double_Judge[statl9][8]=='0' && Data_Double_Judge[statl9][9]=='0')
						{
							Crashl9=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_9<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_9>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][3]=='1')
					{
						Data_Double_Judge[cur+count][3]='0';
						statl9=cur+count;
						sl9=1;
						if(Data_Double_Judge[statl9][0]=='0' && Data_Double_Judge[statl9][1]=='0' && Data_Double_Judge[statl9][2]=='0' && Data_Double_Judge[statl9][3]=='0' && Data_Double_Judge[statl9][4]=='0'
							&& Data_Double_Judge[statl9][5]=='0' && Data_Double_Judge[statl9][6]=='0' && Data_Double_Judge[statl9][7]=='0' && Data_Double_Judge[statl9][8]=='0' && Data_Double_Judge[statl9][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_9<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_9>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][3]=='1')
					{
						Data_Double_Judge[cur+count][3]='0';
						statl9=cur+count;
						sl9=1;
						if(Data_Double_Judge[statl9][0]=='0' && Data_Double_Judge[statl9][1]=='0' && Data_Double_Judge[statl9][2]=='0' && Data_Double_Judge[statl9][3]=='0' && Data_Double_Judge[statl9][4]=='0'
							&& Data_Double_Judge[statl9][5]=='0' && Data_Double_Judge[statl9][6]=='0' && Data_Double_Judge[statl9][7]=='0' && Data_Double_Judge[statl9][8]=='0' && Data_Double_Judge[statl9][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sl3 || (PressedKey1p[3]==true) )
	{
		if(sl3==20)
		{
			sl3=0;
			Crashl3=false;
		}
		else
		{
			sl3++;
		}
		if(PressedKey1p[3]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][4]=='1')
					{
						Data_Double_Judge[cur+count][4]='0';
						statl3=cur+count;
						Onl3=true;
						sl3=1;
						if(Data_Double_Judge[statl3][0]=='0' && Data_Double_Judge[statl3][1]=='0' && Data_Double_Judge[statl3][2]=='0' && Data_Double_Judge[statl3][3]=='0' && Data_Double_Judge[statl3][4]=='0'
							&& Data_Double_Judge[statl3][5]=='0' && Data_Double_Judge[statl3][6]=='0' && Data_Double_Judge[statl3][7]=='0' && Data_Double_Judge[statl3][8]=='0' && Data_Double_Judge[statl3][9]=='0')
						{
							Crashl3=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][4]=='1')
					{
						Data_Double_Judge[cur+count][4]='0';
						statl3=cur+count;
						Onl3=true;
						sl3=1;
						if(Data_Double_Judge[statl3][0]=='0' && Data_Double_Judge[statl3][1]=='0' && Data_Double_Judge[statl3][2]=='0' && Data_Double_Judge[statl3][3]=='0' && Data_Double_Judge[statl3][4]=='0'
							&& Data_Double_Judge[statl3][5]=='0' && Data_Double_Judge[statl3][6]=='0' && Data_Double_Judge[statl3][7]=='0' && Data_Double_Judge[statl3][8]=='0' && Data_Double_Judge[statl3][9]=='0')
						{
							Crashl3=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U-8*HighSpeed_3<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_3>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][4]=='1')
					{
						Data_Double_Judge[cur+count][4]='0';
						statl3=cur+count;
						sl3=1;
						if(Data_Double_Judge[statl3][0]=='0' && Data_Double_Judge[statl3][1]=='0' && Data_Double_Judge[statl3][2]=='0' && Data_Double_Judge[statl3][3]=='0' && Data_Double_Judge[statl3][4]=='0'
							&& Data_Double_Judge[statl3][5]=='0' && Data_Double_Judge[statl3][6]=='0' && Data_Double_Judge[statl3][7]=='0' && Data_Double_Judge[statl3][8]=='0' && Data_Double_Judge[statl3][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_3<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_3>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][4]=='1')
					{
						Data_Double_Judge[cur+count][4]='0';
						statl3=cur+count;
						sl3=1;
						if(Data_Double_Judge[statl3][0]=='0' && Data_Double_Judge[statl3][1]=='0' && Data_Double_Judge[statl3][2]=='0' && Data_Double_Judge[statl3][3]=='0' && Data_Double_Judge[statl3][4]=='0'
							&& Data_Double_Judge[statl3][5]=='0' && Data_Double_Judge[statl3][6]=='0' && Data_Double_Judge[statl3][7]=='0' && Data_Double_Judge[statl3][8]=='0' && Data_Double_Judge[statl3][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	// 2 player
	if(sr1 || (PressedKey2p[1]==true) )
	{
		if(sr1==20)
		{
			sr1=0;
			Crashr1=false;
		}
		else
		{
			sr1++;
		}
		if(PressedKey2p[1]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][5]=='1')
					{
						Data_Double_Judge[cur+count][5]='0';
						statr1=cur+count;
						Onr1=true;
						sr1=1;
						if(Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0'
							&& Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0')
						{
							Crashr1=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_1<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_1>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][5]=='1')
					{
						Data_Double_Judge[cur+count][5]='0';
						statr1=cur+count;
						Onr1=true;
						sr1=1;
						if(Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0'
							&& Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0')
						{
							Crashr1=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_1<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_1>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][5]=='1')
					{
						Data_Double_Judge[cur+count][5]='0';
						statr1=cur+count;
						sr1=1;
						if(Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0'
							&& Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_1<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_1>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][5]=='1')
					{
						Data_Double_Judge[cur+count][5]='0';
						statr1=cur+count;
						sr1=1;
						if(Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0'
							&& Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sr7 || (PressedKey2p[7]==true) )
	{
		if(sr7==20)
		{
			sr7=0;
			Crashr7=false;
		}
		else
		{
			sr7++;
		}
		if(PressedKey2p[7]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][6]=='1')
					{
						Data_Double_Judge[cur+count][6]='0';
						statr7=cur+count;
						Onr7=true;
						sr7=1;
						if(Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0'
							&& Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0')
						{
							Crashr7=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_7<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_7>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][6]=='1')
					{
						Data_Double_Judge[cur+count][6]='0';
						statr7=cur+count;
						Onr7=true;
						sr7=1;
						if(Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0'
							&& Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0')
						{
							Crashr7=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_7<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_7>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][6]=='1')
					{
						Data_Double_Judge[cur+count][6]='0';
						statr7=cur+count;
						sr7=1;
						if(Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0'
							&& Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_7<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_7>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][6]=='1')
					{
						Data_Double_Judge[cur+count][6]='0';
						statr7=cur+count;
						sr7=1;
						if(Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0'
							&& Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sr5 || (PressedKey2p[5]==true) )
	{
		if(sr5==20)
		{
			sr5=0;
			Crashr5=false;
		}
		else
		{
			sr5++;
		}
		if(PressedKey2p[5]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][7]=='1')
					{
						Data_Double_Judge[cur+count][7]='0';
						statr5=cur+count;
						Onr5=true;
						sr5=1;
						if(Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0'
							&& Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0')
						{
							Crashr5=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_5<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_5>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][7]=='1')
					{
						Data_Double_Judge[cur+count][7]='0';
						statr5=cur+count;
						Onr5=true;
						sr5=1;
						if(Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0'
							&& Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0')
						{
							Crashr5=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_5<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_5>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][7]=='1')
					{
						Data_Double_Judge[cur+count][7]='0';
						statr5=cur+count;
						sr5=1;
						if(Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0'
							&& Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_5<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_5>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][7]=='1')
					{
						Data_Double_Judge[cur+count][7]='0';
						statr5=cur+count;
						sr5=1;
						if(Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0'
							&& Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sr9 || (PressedKey2p[9]==true) )
	{
		if(sr9==20)
		{
			sr9=0;
			Crashr9=false;
		}
		else
		{
			sr9++;
		}
		if(PressedKey2p[9]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
				    PERFECT_ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][8]=='1')
					{
						Data_Double_Judge[cur+count][8]='0';
						statr9=cur+count;
						Onr9=true;
						sr9=1;
						if(Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0'
							&& Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0')
						{
							Crashr9=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_9<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_9>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][8]=='1')
					{
						Data_Double_Judge[cur+count][8]='0';
						statr9=cur+count;
						Onr9=true;
						sr9=1;
						if(Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0'
							&& Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0')
						{
							Crashr9=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*HighSpeed_9<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_9>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][8]=='1')
					{
						Data_Double_Judge[cur+count][8]='0';
						statr9=cur+count;
						sr9=1;
						if(Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0'
							&& Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_9<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_9>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][8]=='1')
					{
						Data_Double_Judge[cur+count][8]='0';
						statr9=cur+count;
						sr9=1;
						if(Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0'
							&& Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr9][8]=='0' && Data_Double_Judge[statr9][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(sr3 || (PressedKey2p[3]==true) )
	{
		if(sr3==20)
		{
			sr3=0;
			Crashr3=false;
		}
		else
		{
			sr3++;
		}
		if(PressedKey2p[3]==true)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
				ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
			{
				if( PERFECT_ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
					PERFECT_ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][9]=='1')
					{
						Data_Double_Judge[cur+count][9]='0';
						statr3=cur+count;
						Onr3=true;
						sr3=1;
						if(Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0'
							&& Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0')
						{
							Crashr3=true;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*HighSpeed_3<Data_Double_y[cur+count] &&
					     GREAT_ZONE_D*HighSpeed_3>Data_Double_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][9]=='1')
					{
						Data_Double_Judge[cur+count][9]='0';
						statr3=cur+count;
						Onr3=true;
						sr3=1;
						if(Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0'
							&& Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0')
						{
							Crashr3=true;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U-8*HighSpeed_3<Data_y[cur+count] &&
					     GOOD_ZONE_D*HighSpeed_3>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][9]=='1')
					{
						Data_Double_Judge[cur+count][9]='0';
						statr3=cur+count;
						sr3=1;
						if(Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0'
							&& Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0')
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*HighSpeed_3<Data_y[cur+count] &&
					     BAD_ZONE_D*HighSpeed_3>Data_y[cur+count])
				{
					if(Data_Double_Judge[cur+count][9]=='1')
					{
						Data_Double_Judge[cur+count][9]='0';
						statr3=cur+count;
						sr3=1;
						if(Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0'
							&& Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0')
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	// 2 player routine end
	Judgement1p=JudgeTemp;
	
	// 미스처리입니다.
	for(count=0;count<10;count++)
	if(Data_Double_y[cur+count] < ZONE_U
		&& (Data_Double_Judge[cur+count][0]!='0' || Data_Double_Judge[cur+count][1]!='0' || Data_Double_Judge[cur+count][2]!='0' || Data_Double_Judge[cur+count][3]!='0' || Data_Double_Judge[cur+count][4]!='0' || Data_Double_Judge[cur+count][5]!='0' || Data_Double_Judge[cur+count][6]!='0' || Data_Double_Judge[cur+count][7]!='0' || Data_Double_Judge[cur+count][8]!='0' || Data_Double_Judge[cur+count][9]!='0')
		&& (Data_Double[cur+count][0]!='0' || Data_Double[cur+count][1]!='0' || Data_Double[cur+count][2]!='0' || Data_Double[cur+count][3]!='0' || Data_Double[cur+count][4]!='0' || Data_Double[cur+count][5]!='0' || Data_Double[cur+count][6]!='0' || Data_Double[cur+count][7]!='0' || Data_Double[cur+count][8]!='0' || Data_Double[cur+count][9]!='0'))
	{
		if(Data_Double[cur+count][0]=='2')break;
		Data_Double_Judge[cur+count][0]=Data_Double_Judge[cur+count][1]=Data_Double_Judge[cur+count][2]=Data_Double_Judge[cur+count][3]=Data_Double_Judge[cur+count][4]=Data_Double_Judge[cur+count][5]=Data_Double_Judge[cur+count][6]=Data_Double_Judge[cur+count][7]=Data_Double_Judge[cur+count][8]=Data_Double_Judge[cur+count][9]='0';
		Judgement1p=MISS;
		Combo1p=0;
	}

	if(Crashl1 && sl1==2)
	{
		if(Data_Double_Judge[statl1][0]=='0' && Data_Double_Judge[statl1][1]=='0' && Data_Double_Judge[statl1][2]=='0' && Data_Double_Judge[statl1][3]=='0' && Data_Double_Judge[statl1][4]=='0' && Data_Double_Judge[statl1][5]=='0' && Data_Double_Judge[statl1][6]=='0' && Data_Double_Judge[statl1][7]=='0' && Data_Double_Judge[statl1][8]=='0' && Data_Double_Judge[statl1][9]=='0')	{
			Data_Double[statl1][0]=Data_Double[statl1][1]=Data_Double[statl1][2]=Data_Double[statl1][3]=Data_Double[statl1][4]=Data_Double[statl1][5]=Data_Double[statl1][6]=Data_Double[statl1][7]=Data_Double[statl1][8]=Data_Double[statl1][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashl7 && sl7==2) {
		if(Data_Double_Judge[statl7][0]=='0' && Data_Double_Judge[statl7][1]=='0' && Data_Double_Judge[statl7][2]=='0' && Data_Double_Judge[statl7][3]=='0' && Data_Double_Judge[statl7][4]=='0' && Data_Double_Judge[statl7][5]=='0' && Data_Double_Judge[statl7][6]=='0' && Data_Double_Judge[statl7][7]=='0' && Data_Double_Judge[statl7][8]=='0' && Data_Double_Judge[statl7][9]=='0')	{
			Data_Double[statl7][0]=Data_Double[statl7][1]=Data_Double[statl7][2]=Data_Double[statl7][3]=Data_Double[statl7][4]=Data_Double[statl7][5]=Data_Double[statl7][6]=Data_Double[statl7][7]=Data_Double[statl7][8]=Data_Double[statl7][9]='0';
			Judgement1p=JudgeTemp;
		}
		else {
			Judgement1p=NONE;
		}
	}
	if(Crashl5 && sl5==2) {
		if(Data_Double_Judge[statl5][0]=='0' && Data_Double_Judge[statl5][1]=='0' && Data_Double_Judge[statl5][2]=='0' && Data_Double_Judge[statl5][3]=='0' && Data_Double_Judge[statl5][4]=='0' && Data_Double_Judge[statl5][5]=='0' && Data_Double_Judge[statl5][6]=='0' && Data_Double_Judge[statl5][7]=='0' && Data_Double_Judge[statl5][8]=='0' && Data_Double_Judge[statl5][9]=='0')	{
			Data_Double[statl5][0]=Data_Double[statl5][1]=Data_Double[statl5][2]=Data_Double[statl5][3]=Data_Double[statl5][4]=Data_Double[statl5][5]=Data_Double[statl5][6]=Data_Double[statl5][7]=Data_Double[statl5][8]=Data_Double[statl5][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashl9 && sl9==2) {
		if(Data_Double_Judge[statl9][0]=='0' && Data_Double_Judge[statl9][1]=='0' && Data_Double_Judge[statl9][2]=='0' && Data_Double_Judge[statl9][3]=='0' && Data_Double_Judge[statl9][4]=='0' && Data_Double_Judge[statl9][5]=='0' && Data_Double_Judge[statl9][6]=='0' && Data_Double_Judge[statl9][7]=='0' && Data_Double_Judge[statl9][8]=='0' && Data_Double_Judge[statl9][9]=='0') {
			Data_Double[statl9][0]=Data_Double[statl9][1]=Data_Double[statl9][2]=Data_Double[statl9][3]=Data_Double[statl9][4]=Data_Double[statl9][5]=Data_Double[statl9][6]=Data_Double[statl9][7]=Data_Double[statl9][8]=Data_Double[statl9][9]='0';
			Judgement1p=JudgeTemp;
		}
		else {
			Judgement1p=NONE;
		}
	}

	if(Crashl3 && sl3==2)
	{
		if(Data_Double_Judge[statl3][0]=='0' && Data_Double_Judge[statl3][1]=='0' && Data_Double_Judge[statl3][2]=='0' && Data_Double_Judge[statl3][3]=='0' && Data_Double_Judge[statl3][4]=='0' && Data_Double_Judge[statl3][5]=='0' && Data_Double_Judge[statl3][6]=='0' && Data_Double_Judge[statl3][7]=='0' && Data_Double_Judge[statl3][8]=='0' && Data_Double_Judge[statl3][9]=='0') {
			Data_Double[statl3][0]=Data_Double[statl3][1]=Data_Double[statl3][2]=Data_Double[statl3][3]=Data_Double[statl3][4]=Data_Double[statl3][5]=Data_Double[statl3][6]=Data_Double[statl3][7]=Data_Double[statl3][8]=Data_Double[statl3][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	// 2 player
	if(Crashr1 && sr1==2)
	{
		if(Data_Double_Judge[statr1][0]=='0' && Data_Double_Judge[statr1][1]=='0' && Data_Double_Judge[statr1][2]=='0' && Data_Double_Judge[statr1][3]=='0' && Data_Double_Judge[statr1][4]=='0' && Data_Double_Judge[statr1][5]=='0' && Data_Double_Judge[statr1][6]=='0' && Data_Double_Judge[statr1][7]=='0' && Data_Double_Judge[statr1][8]=='0' && Data_Double_Judge[statr1][9]=='0') {
			Data_Double[statr1][0]=Data_Double[statr1][1]=Data_Double[statr1][2]=Data_Double[statr1][3]=Data_Double[statr1][4]=Data_Double[statr1][5]=Data_Double[statr1][6]=Data_Double[statr1][7]=Data_Double[statr1][8]=Data_Double[statr1][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashr7 && sr7==2) {
		if(Data_Double_Judge[statr7][0]=='0' && Data_Double_Judge[statr7][1]=='0' && Data_Double_Judge[statr7][2]=='0' && Data_Double_Judge[statr7][3]=='0' && Data_Double_Judge[statr7][4]=='0' && Data_Double_Judge[statr7][5]=='0' && Data_Double_Judge[statr7][6]=='0' && Data_Double_Judge[statr7][7]=='0' && Data_Double_Judge[statr7][8]=='0' && Data_Double_Judge[statr7][9]=='0') {
			Data_Double[statr7][0]=Data_Double[statr7][1]=Data_Double[statr7][2]=Data_Double[statr7][3]=Data_Double[statr7][4]=Data_Double[statr7][5]=Data_Double[statr7][6]=Data_Double[statr7][7]=Data_Double[statr7][8]=Data_Double[statr7][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashr5 && sr5==2) {
		if(Data_Double_Judge[statr5][0]=='0' && Data_Double_Judge[statr5][1]=='0' && Data_Double_Judge[statr5][2]=='0' && Data_Double_Judge[statr5][3]=='0' && Data_Double_Judge[statr5][4]=='0' && Data_Double_Judge[statr5][5]=='0' && Data_Double_Judge[statr5][6]=='0' && Data_Double_Judge[statr5][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr5][9]=='0') {
			Data_Double[statr5][0]=Data_Double[statr5][1]=Data_Double[statr5][2]=Data_Double[statr5][3]=Data_Double[statr5][4]=Data_Double[statr5][5]=Data_Double[statr5][6]=Data_Double[statr5][7]=Data_Double[statr5][8]=Data_Double[statr5][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashr9 && sr9==2) {
		if(Data_Double_Judge[statr9][0]=='0' && Data_Double_Judge[statr9][1]=='0' && Data_Double_Judge[statr9][2]=='0' && Data_Double_Judge[statr9][3]=='0' && Data_Double_Judge[statr9][4]=='0' && Data_Double_Judge[statr9][5]=='0' && Data_Double_Judge[statr9][6]=='0' && Data_Double_Judge[statr9][7]=='0' && Data_Double_Judge[statr5][8]=='0' && Data_Double_Judge[statr9][9]=='0') {
			Data_Double[statr9][0]=Data_Double[statr9][1]=Data_Double[statr9][2]=Data_Double[statr9][3]=Data_Double[statr9][4]=Data_Double[statr9][5]=Data_Double[statr9][6]=Data_Double[statr9][7]=Data_Double[statr9][8]=Data_Double[statr9][9]='0';
			Judgement1p=JudgeTemp;
		} else {
			Judgement1p=NONE;
		}
	}

	if(Crashr3 && sr3==2) {
		if(Data_Double_Judge[statr3][0]=='0' && Data_Double_Judge[statr3][1]=='0' && Data_Double_Judge[statr3][2]=='0' && Data_Double_Judge[statr3][3]=='0' && Data_Double_Judge[statr3][4]=='0' && Data_Double_Judge[statr3][5]=='0' && Data_Double_Judge[statr3][6]=='0' && Data_Double_Judge[statr3][7]=='0' && Data_Double_Judge[statr3][8]=='0' && Data_Double_Judge[statr3][9]=='0') {
			Data_Double[statr3][0]=Data_Double[statr3][1]=Data_Double[statr3][2]=Data_Double[statr3][3]=Data_Double[statr3][4]=Data_Double[statr3][5]=Data_Double[statr3][6]=Data_Double[statr3][7]=Data_Double[statr3][8]=Data_Double[statr3][9]='0';
			Judgement1p=JudgeTemp;
		}
		else {
			Judgement1p=NONE;
		}
	}

	if(Judgement1p==PERFECT || Judgement1p==GREAT)
	{
        gBeat.Halt();
        gBeat.Play();

        Combo1p++;
		if(Start1p)
		{
			if(Judgement1p==PERFECT)cPerfect1p++;
			if(Judgement1p==GREAT)cGreat1p++;
			if(Combo1p>cMaxCombo1p)cMaxCombo1p=Combo1p;
		}
		if(Start2p)
		{
			if(Judgement1p==PERFECT)cPerfect2p++;
			if(Judgement1p==GREAT)cGreat2p++;
			if(Combo1p>cMaxCombo2p)cMaxCombo2p=Combo1p;
		}

		if(Combo1p>10)
		{
			if(Gauge1p<0)Gauge1p=1;
			else Gauge1p++;
			if(Gauge1p>42)Gauge1p=41;
		}
		Crashl1=Onl1;
		Crashl3=Onl3;
		Crashl5=Onl5;
		Crashl7=Onl7;
		Crashl9=Onl9;

		Crashr1=Onr1;
		Crashr3=Onr3;
		Crashr5=Onr5;
		Crashr7=Onr7;
		Crashr9=Onr9;

		Onl1=Onl3=Onl5=Onl7=Onl9=Onr1=Onr3=Onr5=Onr7=Onr9=false;
	}
	else if(Judgement1p==GOOD || Judgement1p==BAD || Judgement1p==MISS)
	{
		if(Start1p)
		{
			if(Judgement1p==GOOD)cGood1p++;
			if(Judgement1p==BAD)cBad1p++;
			if(Judgement1p==MISS)cMiss1p++;
		}
		if(Start2p)
		{
			if(Judgement1p==GOOD)cGood2p++;
			if(Judgement1p==BAD)cBad2p++;
			if(Judgement1p==MISS)cMiss2p++;
		}
		Combo1p=0;
	}

	if(Combo1p)
	{
		if(Judgement1p==PERFECT)
		{
			if(Start1p)Score1p+=2000;
			if(Start2p)Score2p+=2000;
		}
		else if(Judgement1p==GREAT)
		{
			if(Start1p)Score1p+=1500;
			if(Start2p)Score2p+=1500;
		}
	}
	else
	{
		if(Judgement1p==PERFECT)
		{
			if(Start1p)Score1p+=1000;
			if(Start2p)Score2p+=1000;
		}
		else if(Judgement1p==GREAT)
		{
			if(Start1p)Score1p+=500;
			if(Start2p)Score2p+=500;
		}
	}

	if(Judgement1p==GOOD)
	{
		if(Start1p)Score1p+=100;
		if(Start2p)Score2p+=100;
	}
	else if(Judgement1p==BAD)
	{
		if(Start1p)Score1p-=700;
		if(Start2p)Score2p-=700;
		Gauge1p-=2;
	}
	else if(Judgement1p==MISS)
	{
		if(Start1p)Score1p-=1000;
		if(Start2p)Score2p-=1000;
		Gauge1p-=5;
	}

	if(KCFG.bcDead)
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

	if(Start1p)if(Score1p<0)Score1p=0;
	if(Start2p)if(Score2p<0)Score2p=0;

    if(beat) {
        gArrow2.BltFast( 64, 50, gScreen );
        gArrow2.BltFast( 320, 50, gScreen );
	}
	else 
	{
        gArrow1.BltFast( 64, 50, gScreen );
        gArrow1.BltFast( 320, 50, gScreen );
	}

	pArrl1.y=0;
	pArrl1.x=arrow_l[sl1];
	pArrl1.w=arrow_r[0];
	pArrl1.h=70;

 	pArrl3.y=0;
	pArrl3.x=arrow_l[sl3];
	pArrl3.w=arrow_r[0];
	pArrl3.h=70;

	pArrl5.y=0;
	pArrl5.x=arrow_l[sl5];
	pArrl5.w=arrow_r[0];
	pArrl5.h=70;

	pArrl7.y=0;
	pArrl7.x=arrow_l[sl7];
	pArrl7.w=arrow_r[0];
	pArrl7.h=70;

	pArrl9.y=0;
	pArrl9.x=arrow_l[sl9];
	pArrl9.w=arrow_r[0];
	pArrl9.h=70;

	cArrl1.y=0;
	cArrl1.x=cArrow_l[sl1];
	cArrl1.w=cArrow_r[0];
	cArrl1.h=80;

	cArrl3.y=0;
	cArrl3.x=cArrow_l[sl3];
	cArrl3.w=cArrow_r[0];
	cArrl3.h=80;

	cArrl5.y=0;
	cArrl5.x=cArrow_l[sl5];
	cArrl5.w=cArrow_r[0];
	cArrl5.h=80;

	cArrl7.y=0;
	cArrl7.x=cArrow_l[sl7];
	cArrl7.w=cArrow_r[0];
	cArrl7.h=80;

	cArrl9.y=0;
	cArrl9.x=cArrow_l[sl9];
	cArrl9.w=cArrow_r[0];
	cArrl9.h=80;

	// 2player
	/*
	pArrr1.y=0;
	pArrr1.x=arrow_l[sr1];
	pArrr1.w=arrow_r[sr1];
	pArrr1.h=70;

	pArrr3.y=0;
	pArrr3.x=arrow_l[sr3];
	pArrr3.w=arrow_r[0];
	pArrr3.h=70;

	pArrr5.y=0;
	pArrr5.x=arrow_l[sr5];
	pArrr5.w=arrow_r[0];
	pArrr5.h=70;

	pArrr7.y=0;
	pArrr7.x=arrow_l[sr7];
	pArrr7.w=arrow_r[0];
	pArrr7.h=70;

	pArrr9.y=0;
	pArrr9.x=arrow_l[sr9];
	pArrr9.w=arrow_r[0];
	pArrr9.h=70;

	cArrr1.y=0;
	cArrr1.x=cArrow_l[sr1];
	cArrr1.w=cArrow_r[0];
	cArrr1.h=80;

	cArrr3.y=0;
	cArrr3.x=cArrow_l[sr3];
	cArrr3.w=cArrow_r[0];
	cArrr3.h=80;

	cArrr5.y=0;
	cArrr5.x=cArrow_l[sr5];
	cArrr5.w=cArrow_r[0];
	cArrr5.h=80;

	cArrr7.y=0;
	cArrr7.x=cArrow_l[sr7];
	cArrr7.w=cArrow_r[0];
	cArrr7.h=80;

	cArrr9.y=0;
	cArrr9.x=cArrow_l[sr9];
	cArrr9.w=cArrow_r[0];
	cArrr9.h=80;
*/
	if(Crashl1)
        cArrow1.BltFast( 57, 43, gScreen, &cArrl1 );
	else if(sl1)
        pArrow1.BltFast( 59, 45, gScreen, &pArrl1 );

	if(Crashl7) cArrow7.BltFast( 107, 43, gScreen, &cArrl7 );
	else if(sl7)pArrow7.BltFast( 109, 45, gScreen, &pArrl7 );
	
	if(Crashl5) cArrow5.BltFast( 157, 43, gScreen, &cArrl5 );
	else if(sl5)pArrow5.BltFast( 159, 45, gScreen, &pArrl5 );

	if(Crashl9) cArrow9.BltFast( 207, 43, gScreen, &cArrl9 );
	else if(sl9)pArrow9.BltFast( 209, 45, gScreen, &pArrl9 );
	
	if(Crashl3) cArrow3.BltFast( 257, 43, gScreen, &cArrl3 );
	else if(sl3)pArrow3.BltFast( 259, 45, gScreen, &pArrl3 );

// 2 player
	if(Crashr1) cArrow1.BltFast( 313, 43, gScreen, &cArrl1 );
	else if(sr1)pArrow1.BltFast( 315, 45, gScreen, &pArrl1 );
	
	if(Crashr7) cArrow7.BltFast( 363, 43, gScreen, &cArrl7 );
	else if(sr7)pArrow7.BltFast( 365, 45, gScreen, &pArrl7 );
	
	if(Crashr5) cArrow5.BltFast( 413, 43, gScreen, &cArrl5 );
	else if(sr5)pArrow5.BltFast( 415, 45, gScreen, &pArrl5 );

	if(Crashr9) cArrow9.BltFast( 463, 43, gScreen, &cArrl9 );
	else if(sr9)pArrow9.BltFast( 465, 45, gScreen, &pArrl9 );
	
	if(Crashr3) cArrow3.BltFast( 513, 43, gScreen, &cArrl3 );
	else if(sr3)pArrow3.BltFast( 515, 45, gScreen, &pArrl3 );
}

void KIU_STAGE_DOUBLE(void)
{
	static int temp;
	static Uint32 i;
	static Uint32 cur,last,sec;
	static Uint32 starttime, curtime;

	int k;
	static SDL_Rect rect1[7],rect3[7],rect5[7],rect7[7],rect9[7];
	
	Uint32 delta;

	static int sta;

	static double tail;

		
	double bpmpix=PUMP_SPRITE_Y*(bpm/60)/1000;

    gSongBack.BltFast( 0, 0, gScreen );

	DisplayStageCount(dwGameCount);

	if(start1==0)
	{
		if(Start1p)
		{

			HighSpeed=gSpeed[0].step;
			HighSpeed_1=gSpeed[0].step1;
			HighSpeed_3=gSpeed[0].step3;
			HighSpeed_5=gSpeed[0].step5;
			HighSpeed_7=gSpeed[0].step7;
			HighSpeed_9=gSpeed[0].step9;

			bModeMirror=bModeMirror1p;
			bModeNonstep=bModeNonstep1p;
			bModeUnion=bModeUnion1p;
			bModeRandom=bModeRandom1p;
			b4dMix=b4dMix1p;
			//bModeVanish=bModeVanish;
		}

		else if(Start2p)
		{
			HighSpeed=gSpeed[1].step;
			HighSpeed_1=gSpeed[1].step1;
			HighSpeed_3=gSpeed[1].step3;
			HighSpeed_5=gSpeed[1].step5;
			HighSpeed_7=gSpeed[1].step7;
			HighSpeed_9=gSpeed[1].step9;

			bModeMirror=bModeMirror2p;
			bModeNonstep=bModeNonstep2p;
			bModeUnion=bModeUnion2p;
			bModeRandom=bModeRandom2p;
			b4dMix=b4dMix2p;
			//bModeVanish=bModeVanish;
		}

		if(b4dMix)
		{
			MaxSpeed=MinSpeed=HighSpeed_1;

			if(HighSpeed_3 > MaxSpeed)MaxSpeed=HighSpeed_3;
			if(HighSpeed_5 > MaxSpeed)MaxSpeed=HighSpeed_5;
			if(HighSpeed_7 > MaxSpeed)MaxSpeed=HighSpeed_7;
			if(HighSpeed_9 > MaxSpeed)MaxSpeed=HighSpeed_9;

			if(HighSpeed_3 < MinSpeed)MinSpeed=HighSpeed_3;
			if(HighSpeed_5 < MinSpeed)MinSpeed=HighSpeed_5;
			if(HighSpeed_7 < MinSpeed)MinSpeed=HighSpeed_7;
			if(HighSpeed_9 < MinSpeed)MinSpeed=HighSpeed_9;
		}
		else 
		{
			MaxSpeed=MinSpeed=HighSpeed;
			HighSpeed_1=HighSpeed_3=HighSpeed_5=HighSpeed_7=HighSpeed_9=HighSpeed;
		}

		for(sta=0;sta<6;sta++)
		{
			rect7[sta].y=0;
			rect7[sta].x=0+PUMP_SPRITE_NEW*sta;
			rect7[sta].w=PUMP_SPRITE_NEW;
			rect7[sta].h=PUMP_SPRITE_NEW;

			rect9[sta].y=PUMP_SPRITE_NEW;
			rect9[sta].x=0+PUMP_SPRITE_NEW*sta;
			rect9[sta].w=PUMP_SPRITE_NEW;
			rect9[sta].h=PUMP_SPRITE_NEW;

			rect5[sta].y=PUMP_SPRITE_NEW*2;
			rect5[sta].x=0+PUMP_SPRITE_NEW*sta;
			rect5[sta].w=PUMP_SPRITE_NEW;
			rect5[sta].h=PUMP_SPRITE_NEW;

			rect3[sta].y=PUMP_SPRITE_NEW*3;
			rect3[sta].x=0+PUMP_SPRITE_NEW*sta;
			rect3[sta].w=PUMP_SPRITE_NEW;
			rect3[sta].h=PUMP_SPRITE_NEW;

			rect1[sta].y=PUMP_SPRITE_NEW*4;
			rect1[sta].x=0+PUMP_SPRITE_NEW*sta;
			rect1[sta].w=PUMP_SPRITE_NEW;
			rect1[sta].h=PUMP_SPRITE_NEW;
		}
		sta=0;
		
		if(bModeMirror)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				Data_Double[MAX_DATA][0]=Data_Double[i][0];
				Data_Double[MAX_DATA][1]=Data_Double[i][1];
				Data_Double[MAX_DATA][2]=Data_Double[i][2];
				Data_Double[MAX_DATA][3]=Data_Double[i][3];
				Data_Double[MAX_DATA][4]=Data_Double[i][4];
				Data_Double[MAX_DATA][5]=Data_Double[i][5];
				Data_Double[MAX_DATA][6]=Data_Double[i][6];
				Data_Double[MAX_DATA][7]=Data_Double[i][7];
				Data_Double[MAX_DATA][8]=Data_Double[i][8];
				Data_Double[MAX_DATA][9]=Data_Double[i][9];
				
				if(Data_Double[MAX_DATA][0]=='2')break;
				
				Data_Double[i][0]=Data_Double[i][1]=Data_Double[i][2]=Data_Double[i][3]=Data_Double[i][4]='0';
				Data_Double[i][5]=Data_Double[i][6]=Data_Double[i][7]=Data_Double[i][8]=Data_Double[i][9]='0';

				if(Data_Double[MAX_DATA][0]=='1')Data_Double[i][8]='1';
				if(Data_Double[MAX_DATA][1]=='1')Data_Double[i][9]='1';
				if(Data_Double[MAX_DATA][2]=='1')Data_Double[i][7]='1';
				if(Data_Double[MAX_DATA][3]=='1')Data_Double[i][5]='1';
				if(Data_Double[MAX_DATA][4]=='1')Data_Double[i][6]='1';
				if(Data_Double[MAX_DATA][5]=='1')Data_Double[i][3]='1';
				if(Data_Double[MAX_DATA][6]=='1')Data_Double[i][4]='1';
				if(Data_Double[MAX_DATA][7]=='1')Data_Double[i][2]='1';
				if(Data_Double[MAX_DATA][8]=='1')Data_Double[i][0]='1';
				if(Data_Double[MAX_DATA][9]=='1')Data_Double[i][1]='1';
			}
		}
			
		if(bModeRandom)
		{
			time_t t;

			srand((unsigned) time(&t));
			
			for(i=0;i<MAX_DATA;i++)
			{
				Data_Double[MAX_DATA][0]=Data_Double[i][0];
				Data_Double[MAX_DATA][1]=Data_Double[i][1];
				Data_Double[MAX_DATA][2]=Data_Double[i][2];
				Data_Double[MAX_DATA][3]=Data_Double[i][3];
				Data_Double[MAX_DATA][4]=Data_Double[i][4];
				Data_Double[MAX_DATA][5]=Data_Double[i][5];
				Data_Double[MAX_DATA][6]=Data_Double[i][6];
				Data_Double[MAX_DATA][7]=Data_Double[i][7];
				Data_Double[MAX_DATA][8]=Data_Double[i][8];
				Data_Double[MAX_DATA][9]=Data_Double[i][9];
				
				if(Data_Double[MAX_DATA][0]=='2')break;
				
				Data_Double[i][0]=Data_Double[i][1]=Data_Double[i][2]=Data_Double[i][3]=Data_Double[i][4]='0';
				Data_Double[i][5]=Data_Double[i][6]=Data_Double[i][7]=Data_Double[i][8]=Data_Double[i][9]='0';

				if(Data_Double[MAX_DATA][0]=='1')Data_Double[i][rand()%5]='1';
				if(Data_Double[MAX_DATA][1]=='1')Data_Double[i][rand()%5]='1';
				if(Data_Double[MAX_DATA][2]=='1')Data_Double[i][rand()%5]='1';
				if(Data_Double[MAX_DATA][3]=='1')Data_Double[i][rand()%5]='1';
				if(Data_Double[MAX_DATA][4]=='1')Data_Double[i][rand()%5]='1';
				if(Data_Double[MAX_DATA][5]=='1')Data_Double[i][rand()%5+5]='1';
				if(Data_Double[MAX_DATA][6]=='1')Data_Double[i][rand()%5+5]='1';
				if(Data_Double[MAX_DATA][7]=='1')Data_Double[i][rand()%5+5]='1';
				if(Data_Double[MAX_DATA][8]=='1')Data_Double[i][rand()%5+5]='1';
				if(Data_Double[MAX_DATA][9]=='1')Data_Double[i][rand()%5+5]='1';
			}
		}

		if(bModeUnion)
		{
			for(i=0;i<(MAX_DATA);i++)
			{
				Data_Double[MAX_DATA][0]=Data_Double[i][0];
				Data_Double[MAX_DATA][1]=Data_Double[i][1];
				Data_Double[MAX_DATA][2]=Data_Double[i][2];
				Data_Double[MAX_DATA][3]=Data_Double[i][3];
				Data_Double[MAX_DATA][4]=Data_Double[i][4];
				Data_Double[MAX_DATA][5]=Data_Double[i][5];
				Data_Double[MAX_DATA][6]=Data_Double[i][6];
				Data_Double[MAX_DATA][7]=Data_Double[i][7];
				Data_Double[MAX_DATA][8]=Data_Double[i][8];
				Data_Double[MAX_DATA][9]=Data_Double[i][9];

				if(Data_Double[MAX_DATA][0]=='2')break;

				if(Data_Double[MAX_DATA][0]=='1')Data_Double[i][8]='1';
				if(Data_Double[MAX_DATA][1]=='1')Data_Double[i][9]='1';
				if(Data_Double[MAX_DATA][2]=='1')Data_Double[i][7]='1';
				if(Data_Double[MAX_DATA][3]=='1')Data_Double[i][5]='1';
				if(Data_Double[MAX_DATA][4]=='1')Data_Double[i][6]='1';

				if(Data_Double[MAX_DATA][5]=='1')Data_Double[i][3]='1';
				if(Data_Double[MAX_DATA][6]=='1')Data_Double[i][4]='1';
				if(Data_Double[MAX_DATA][7]=='1')Data_Double[i][2]='1';
				if(Data_Double[MAX_DATA][8]=='1')Data_Double[i][0]='1';
				if(Data_Double[MAX_DATA][9]=='1')Data_Double[i][1]='1';
			}
		}

		memcpy(&Data_Double_Judge,&Data_Double,sizeof(Data_Double_Judge));

		if(bModeNonstep)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				if(!(Data_Double[i][0]=='0' && Data_Double[i][1]=='0' && Data_Double[i][2]=='0' && Data_Double[i][3]=='0' && Data_Double[i][4]=='0' && Data_Double[i][5]=='0' && Data_Double[i][6]=='0' && Data_Double[i][7]=='0' && Data_Double[i][8]=='0' && Data_Double[i][9]=='0'))
				{
					i++;
					for(;i<MAX_DATA;i++)
					{
						if(Data_Double[i][0]=='2')i=MAX_DATA;
						else
						{
							Data_Double[i][0]='3';
							Data_Double[i][1]='3';
							Data_Double[i][2]='3';
							Data_Double[i][3]='3';
							Data_Double[i][4]='3';

							Data_Double[i][5]='3';
							Data_Double[i][6]='3';
							Data_Double[i][7]='3';
							Data_Double[i][8]='3';
							Data_Double[i][9]='3';
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
		//if(tick==4)start/=2;
		last=cur=SDL_GetTicks();

		tail=0;

		i=0;
		
		Gauge1p=10;
		temp=0;
		start1++;
		temp+=55;
		starttime=start;
		curtime=0;
	}

	cur=SDL_GetTicks();        // 130/ 600000
	delta=cur-last;
	last=cur;

	DrawArrow_DB(i); //회색 화살표를 말합니다.

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
			else delta=(Uint32)curtime;
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

	if(SDL_GetTicks()-sec>100)
	{
		sec=SDL_GetTicks();
		if(sta==5)sta=0;
		else sta++;
	}
		

	for(k=0;k<48;k+=tick)
	{

		if(Data_Double[i][0]=='2' || Data_Double[i+1][0]=='2' || Data_Double[i+2][0]=='2' || Data_Double[i+3][0]=='2')
		{
			k=48;
			if(SongFlag)
			{
                gSong.Halt();
				SongFlag=false;
			}
			g_ProgramState=RESULT;
		}

		if(tick==2)
		{
			if(bModeSuddenR1p || bModeSuddenR2p)
			{
				if(Data_Double_y[i+k]>240 && Data_Double_y[i+k]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k][0]=Data_Double[i+k][1]=Data_Double[i+k][2]=Data_Double[i+k][3]=Data_Double[i+k][4]=Data_Double[i+k][5]=Data_Double[i+k][6]=Data_Double[i+k][7]=Data_Double[i+k][8]=Data_Double[i+k][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][5]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][6]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][7]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][8]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][9]=='1')Data_Double[i+k][rand()%10]='1';

						Data_Double_Judge[i+k][0]=Data_Double[i+k][0];
						Data_Double_Judge[i+k][1]=Data_Double[i+k][1];
						Data_Double_Judge[i+k][2]=Data_Double[i+k][2];
						Data_Double_Judge[i+k][3]=Data_Double[i+k][3];
						Data_Double_Judge[i+k][4]=Data_Double[i+k][4];
						Data_Double_Judge[i+k][5]=Data_Double[i+k][5];
						Data_Double_Judge[i+k][6]=Data_Double[i+k][6];
						Data_Double_Judge[i+k][7]=Data_Double[i+k][7];
						Data_Double_Judge[i+k][8]=Data_Double[i+k][8];
						Data_Double_Judge[i+k][9]=Data_Double[i+k][9];
					}
				}
				if(Data_Double_y[i+k+1]>240 && Data_Double_y[i+k+1]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k+1][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k+1][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k+1][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k+1][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k+1][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k+1][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k+1][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k+1][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k+1][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k+1][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k+1][0]=Data_Double[i+k+1][1]=Data_Double[i+k+1][2]=Data_Double[i+k+1][3]=Data_Double[i+k+1][4]=Data_Double[i+k+1][5]=Data_Double[i+k+1][6]=Data_Double[i+k+1][7]=Data_Double[i+k+1][8]=Data_Double[i+k+1][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k+1][rand()%10]='1';

						Data_Double_Judge[i+k+1][0]=Data_Double[i+k+1][0];
						Data_Double_Judge[i+k+1][1]=Data_Double[i+k+1][1];
						Data_Double_Judge[i+k+1][2]=Data_Double[i+k+1][2];
						Data_Double_Judge[i+k+1][3]=Data_Double[i+k+1][3];
						Data_Double_Judge[i+k+1][4]=Data_Double[i+k+1][4];
						Data_Double_Judge[i+k+1][5]=Data_Double[i+k+1][5];
						Data_Double_Judge[i+k+1][6]=Data_Double[i+k+1][6];
						Data_Double_Judge[i+k+1][7]=Data_Double[i+k+1][7];
						Data_Double_Judge[i+k+1][8]=Data_Double[i+k+1][8];
						Data_Double_Judge[i+k+1][9]=Data_Double[i+k+1][9];
					}
				}
			}
			if(Data_Double[i+k][0]=='1')
				ClpBlt(LP1DB1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k][1]=='1')
				ClpBlt(LP7DB1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k][2]=='1')
				ClpBlt(LP5DB1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k][3]=='1')
				ClpBlt(LP9DB1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k][4]=='1')
				ClpBlt(LP3DB1_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );
			Data_Double_y[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data_Double[i+k+1][0]=='1')
				ClpBlt(LP1DB1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+1][1]=='1')
				ClpBlt(LP7DB1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+1][2]=='1')
				ClpBlt(LP5DB1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+1][3]=='1')
				ClpBlt(LP9DB1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+1][4]=='1')
				ClpBlt(LP3DB1_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		}	
		else if(tick==4)
		{
			if(bModeSuddenR1p || bModeSuddenR2p)
			{
				if(Data_Double_y[i+k]>240 && Data_Double_y[i+k]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k][0]=Data_Double[i+k][1]=Data_Double[i+k][2]=Data_Double[i+k][3]=Data_Double[i+k][4]=Data_Double[i+k][5]=Data_Double[i+k][6]=Data_Double[i+k][7]=Data_Double[i+k][8]=Data_Double[i+k][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][5]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][6]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][7]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][8]=='1')Data_Double[i+k][rand()%10]='1';
						if(Data_Double[MAX_DATA][9]=='1')Data_Double[i+k][rand()%10]='1';

						Data_Double_Judge[i+k][0]=Data_Double[i+k][0];
						Data_Double_Judge[i+k][1]=Data_Double[i+k][1];
						Data_Double_Judge[i+k][2]=Data_Double[i+k][2];
						Data_Double_Judge[i+k][3]=Data_Double[i+k][3];
						Data_Double_Judge[i+k][4]=Data_Double[i+k][4];
						Data_Double_Judge[i+k][5]=Data_Double[i+k][5];
						Data_Double_Judge[i+k][6]=Data_Double[i+k][6];
						Data_Double_Judge[i+k][7]=Data_Double[i+k][7];
						Data_Double_Judge[i+k][8]=Data_Double[i+k][8];
						Data_Double_Judge[i+k][9]=Data_Double[i+k][9];
					}
				}
				if(Data_Double_y[i+k+1]>240 && Data_Double_y[i+k+1]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k+1][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k+1][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k+1][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k+1][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k+1][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k+1][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k+1][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k+1][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k+1][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k+1][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k+1][0]=Data_Double[i+k+1][1]=Data_Double[i+k+1][2]=Data_Double[i+k+1][3]=Data_Double[i+k+1][4]=Data_Double[i+k+1][5]=Data_Double[i+k+1][6]=Data_Double[i+k+1][7]=Data_Double[i+k+1][8]=Data_Double[i+k+1][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][5]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][6]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][7]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][8]=='1')Data_Double[i+k+1][rand()%10]='1';
						if(Data_Double[MAX_DATA][9]=='1')Data_Double[i+k+1][rand()%10]='1';

						Data_Double_Judge[i+k+1][0]=Data_Double[i+k+1][0];
						Data_Double_Judge[i+k+1][1]=Data_Double[i+k+1][1];
						Data_Double_Judge[i+k+1][2]=Data_Double[i+k+1][2];
						Data_Double_Judge[i+k+1][3]=Data_Double[i+k+1][3];
						Data_Double_Judge[i+k+1][4]=Data_Double[i+k+1][4];
						Data_Double_Judge[i+k+1][5]=Data_Double[i+k+1][5];
						Data_Double_Judge[i+k+1][6]=Data_Double[i+k+1][6];
						Data_Double_Judge[i+k+1][7]=Data_Double[i+k+1][7];
						Data_Double_Judge[i+k+1][8]=Data_Double[i+k+1][8];
						Data_Double_Judge[i+k+1][9]=Data_Double[i+k+1][9];
					}
				}
				if(Data_Double_y[i+k+2]>240 && Data_Double_y[i+k+2]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k+2][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k+2][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k+2][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k+2][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k+2][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k+2][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k+2][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k+2][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k+2][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k+2][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k+2][0]=Data_Double[i+k+2][1]=Data_Double[i+k+2][2]=Data_Double[i+k+2][3]=Data_Double[i+k+2][4]=Data_Double[i+k+2][5]=Data_Double[i+k+2][6]=Data_Double[i+k+2][7]=Data_Double[i+k+2][8]=Data_Double[i+k+2][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][5]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][6]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][7]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][8]=='1')Data_Double[i+k+2][rand()%10]='1';
						if(Data_Double[MAX_DATA][9]=='1')Data_Double[i+k+2][rand()%10]='1';

						Data_Double_Judge[i+k+2][0]=Data_Double[i+k+2][0];
						Data_Double_Judge[i+k+2][1]=Data_Double[i+k+2][1];
						Data_Double_Judge[i+k+2][2]=Data_Double[i+k+2][2];
						Data_Double_Judge[i+k+2][3]=Data_Double[i+k+2][3];
						Data_Double_Judge[i+k+2][4]=Data_Double[i+k+2][4];
						Data_Double_Judge[i+k+2][5]=Data_Double[i+k+2][5];
						Data_Double_Judge[i+k+2][6]=Data_Double[i+k+2][6];
						Data_Double_Judge[i+k+2][7]=Data_Double[i+k+2][7];
						Data_Double_Judge[i+k+2][8]=Data_Double[i+k+2][8];
						Data_Double_Judge[i+k+2][9]=Data_Double[i+k+2][9];
					}
				}
				if(Data_Double_y[i+k+3]>240 && Data_Double_y[i+k+3]<290)
				{
					Data_Double[MAX_DATA][0]=Data_Double[i+k+3][0];
					Data_Double[MAX_DATA][1]=Data_Double[i+k+3][1];
					Data_Double[MAX_DATA][2]=Data_Double[i+k+3][2];
					Data_Double[MAX_DATA][3]=Data_Double[i+k+3][3];
					Data_Double[MAX_DATA][4]=Data_Double[i+k+3][4];
					Data_Double[MAX_DATA][5]=Data_Double[i+k+3][5];
					Data_Double[MAX_DATA][6]=Data_Double[i+k+3][6];
					Data_Double[MAX_DATA][7]=Data_Double[i+k+3][7];
					Data_Double[MAX_DATA][8]=Data_Double[i+k+3][8];
					Data_Double[MAX_DATA][9]=Data_Double[i+k+3][9];

					if(Data_Double[MAX_DATA][0]!='2')
					{
						Data_Double[i+k+3][0]=Data_Double[i+k+3][1]=Data_Double[i+k+3][2]=Data_Double[i+k+3][3]=Data_Double[i+k+3][4]=Data_Double[i+k+3][5]=Data_Double[i+k+3][6]=Data_Double[i+k+3][7]=Data_Double[i+k+3][8]=Data_Double[i+k+3][9]='0';

						if(Data_Double[MAX_DATA][0]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][1]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][2]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][3]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][4]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][5]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][6]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][7]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][8]=='1')Data_Double[i+k+3][rand()%10]='1';
						if(Data_Double[MAX_DATA][9]=='1')Data_Double[i+k+3][rand()%10]='1';

						Data_Double_Judge[i+k+3][0]=Data_Double[i+k+3][0];
						Data_Double_Judge[i+k+3][1]=Data_Double[i+k+3][1];
						Data_Double_Judge[i+k+3][2]=Data_Double[i+k+3][2];
						Data_Double_Judge[i+k+3][3]=Data_Double[i+k+3][3];
						Data_Double_Judge[i+k+3][4]=Data_Double[i+k+3][4];
						Data_Double_Judge[i+k+3][5]=Data_Double[i+k+3][5];
						Data_Double_Judge[i+k+3][6]=Data_Double[i+k+3][6];
						Data_Double_Judge[i+k+3][7]=Data_Double[i+k+3][7];
						Data_Double_Judge[i+k+3][8]=Data_Double[i+k+3][8];
						Data_Double_Judge[i+k+3][9]=Data_Double[i+k+3][9];
					}
				}
			}
			if(Data_Double[i+k][0]=='1')
				ClpBlt(LP1DB1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k][1]=='1')
				ClpBlt(LP7DB1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k][2]=='1')
				ClpBlt(LP5DB1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k][3]=='1')
				ClpBlt(LP9DB1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k][4]=='1')
				ClpBlt(LP3DB1_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data_Double[i+k+1][0]=='1')
				ClpBlt(LP1DB1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+1][1]=='1')
				ClpBlt(LP7DB1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+1][2]=='1')
				ClpBlt(LP5DB1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+1][3]=='1')
				ClpBlt(LP9DB1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+1][4]=='1')
				ClpBlt(LP3DB1_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data_Double[i+k+2][0]=='1')
				ClpBlt(LP1DB1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+2][1]=='1')
				ClpBlt(LP7DB1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+2][2]=='1')
				ClpBlt(LP5DB1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+2][3]=='1')
				ClpBlt(LP9DB1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+2][4]=='1')
				ClpBlt(LP3DB1_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data_Double[i+k+3][0]=='1')
				ClpBlt(LP1DB1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+3][1]=='1')
				ClpBlt(LP7DB1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+3][2]=='1')
				ClpBlt(LP5DB1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+3][3]=='1')
				ClpBlt(LP9DB1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+3][4]=='1')
				ClpBlt(LP3DB1_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		}

// 오른쪽 
		if(tick==2)
		{
			if(Data_Double[i+k][5]=='1')
				ClpBlt(LP1DB2_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k][6]=='1')
				ClpBlt(LP7DB2_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k][7]=='1')
				ClpBlt(LP5DB2_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k][8]=='1')
				ClpBlt(LP9DB2_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k][9]=='1')
				ClpBlt(LP3DB2_X,(temp+PUMP_SPRITE_Y*k/2)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );
			Data_Double_y[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data_Double[i+k+1][5]=='1')
				ClpBlt(LP1DB2_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+1][6]=='1')
				ClpBlt(LP7DB2_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+1][7]=='1')
				ClpBlt(LP5DB2_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+1][8]=='1')
				ClpBlt(LP9DB2_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+1][9]=='1')
				ClpBlt(LP3DB2_X,(25+temp+PUMP_SPRITE_Y*k/2)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		}	
		else if(tick==4)
		{
			if(Data_Double[i+k][5]=='1')
				ClpBlt(LP1DB2_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k][6]=='1')
				ClpBlt(LP7DB2_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k][7]=='1')
				ClpBlt(LP5DB2_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k][8]=='1')
				ClpBlt(LP9DB2_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k][9]=='1')
				ClpBlt(LP3DB2_X,(temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data_Double[i+k+1][5]=='1')
				ClpBlt(LP1DB2_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+1][6]=='1')
				ClpBlt(LP7DB2_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+1][7]=='1')
				ClpBlt(LP5DB2_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+1][8]=='1')
				ClpBlt(LP9DB2_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+1][9]=='1')
				ClpBlt(LP3DB2_X,(12+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data_Double[i+k+2][5]=='1')
				ClpBlt(LP1DB2_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+2][6]=='1')
				ClpBlt(LP7DB2_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+2][7]=='1')
				ClpBlt(LP5DB2_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+2][8]=='1')
				ClpBlt(LP9DB2_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+2][9]=='1')
				ClpBlt(LP3DB2_X,(25+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data_Double[i+k+3][5]=='1')
				ClpBlt(LP1DB2_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_1-(PUMP_SPRITE_Y)*(HighSpeed_1-1),gWArrow, rect1[sta] );
			if(Data_Double[i+k+3][6]=='1')
				ClpBlt(LP7DB2_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_7-(PUMP_SPRITE_Y)*(HighSpeed_7-1),gWArrow, rect7[sta] );
			if(Data_Double[i+k+3][7]=='1')
				ClpBlt(LP5DB2_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_5-(PUMP_SPRITE_Y)*(HighSpeed_5-1),gWArrow, rect5[sta] );
			if(Data_Double[i+k+3][8]=='1')
				ClpBlt(LP9DB2_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_9-(PUMP_SPRITE_Y)*(HighSpeed_9-1),gWArrow, rect9[sta] );
			if(Data_Double[i+k+3][9]=='1')
				ClpBlt(LP3DB2_X,(38+temp+PUMP_SPRITE_Y*k/4)*HighSpeed_3-(PUMP_SPRITE_Y)*(HighSpeed_3-1),gWArrow, rect3[sta] );

			Data_Double_y[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		}

	}
	if(Start1p)
	{
		if(bModeMirror1p)DrawMode(0,200,HMODE_MIRROR);
		if(bModeNonstep1p)DrawMode(0,240,HMODE_NONSTEP);
		if(bModeSynchro)DrawMode(0,280,HMODE_SYNCHRO);
		if(bModeUnion1p)DrawMode(0,320,HMODE_UNION);
		if(bModeRandom1p)DrawMode(0,360,HMODE_RANDOM);
		if(bModeVanish1p)DrawMode(0,400,HMODE_VANISH);

		if(gSpeed[0].step>1)DrawMode(0,160,HMODE_2X);

		DrawGaugeDB_1p();
		DrawScore1p();
	}
	else if(Start2p)
	{
		if(bModeMirror2p)DrawMode(600,200,HMODE_MIRROR);
		if(bModeNonstep2p)DrawMode(600,240,HMODE_NONSTEP);
		if(bModeUnion2p)DrawMode(600,320,HMODE_UNION);
		if(bModeRandom2p)DrawMode(600,360,HMODE_RANDOM);
		if(bModeVanish2p)DrawMode(600,400,HMODE_VANISH);

		if(gSpeed[1].step>1)DrawMode(600,160,HMODE_2X);

		DrawGaugeDB_2p();
		DrawScore2p();
	}
	DrawJudgeDB();
}
