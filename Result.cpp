/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

#include "Surface.h"
#include "Main.h"
#include "Input.h"
#include <stdio.h>

extern	Surface	ResultBack;
extern	Surface	ResultFont;
extern	Surface	gNumberFont;

extern	char First;
extern	char	Double;
extern	int dwGameCount;

char JudgeAnaly1p(void)
{
	Uint32	cTotal1p;

	cTotal1p=cPerfect1p+cGreat1p+cGood1p+cBad1p+cMiss1p;

	if(Gauge1p<0)return	'F';

	if(cGood1p==0 && cBad1p==0 && cMiss1p==0)return	'S';

	if((((double)(cPerfect1p+cGreat1p)/cTotal1p)*100)>95)return	'A';

	if((((double)(cPerfect1p+cGreat1p)/(double)cTotal1p)*100)>80)return	'B';

	if((((double)(cPerfect1p+cGreat1p)/(double)cTotal1p)*100)>60)return	'C';

	return	'F';
}

char JudgeAnaly2p(void)
{
	Uint32	cTotal2p;

	cTotal2p=cPerfect2p+cGreat2p+cGood2p+cBad2p+cMiss2p;

	if(Gauge2p<0)if(!Double)return	'F';

	if(cGood2p==0 && cBad2p==0 && cMiss2p==0)return	'S';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>95)return	'A';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>80)return	'B';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>60)return	'C';

	return	'F';
}

void DisplayJudge(int x, int y, char s, Uint32 ColorKey)
{
	SDL_Rect    sRect;

	switch(s)
	{
		case	'A':
			sRect.y=2;
			sRect.x=7;
			sRect.w=146;
			sRect.h=218;

			break;
		case	'B':
			sRect.y=2;
			sRect.x=155;
			sRect.w=135;
			sRect.h=218;

			break;
		case	'C':
			sRect.y=2;
			sRect.x=295;
			sRect.w=143;
			sRect.h=218;

			break;
		case	'D':
			sRect.y=2;
			sRect.x=450;
			sRect.w=125;
			sRect.h=218;

			break;
		case	'F':
			sRect.y=225;
			sRect.x=127;
			sRect.w=98;
			sRect.h=218;

			break;
		case	'S':
			sRect.y=225;
			sRect.x=0;
			sRect.w=127;
			sRect.h=219;
			break;
	}

    ResultFont.SetAlpha( 150 );
    ResultFont.BltFast( x, y, gScreen, &sRect );
}


void	DisplayNumber(int x, int y, char *message)
{
	for(int Loop=0;;Loop++)
	{
		if(message[Loop]==0)
            break;
		message[Loop]-=48;

        SDL_Rect    cRect;
		cRect.x = message[Loop]*30;
        cRect.y = 0;

        cRect.w = 30;
        cRect.h = 38;

        gNumberFont.BltFast( x+Loop*30, y, gScreen, &cRect );
	}
}

extern bool	ClpBlt3(int x ,int y ,Surface & surface, const SDL_Rect & srect);

void Result(void)
{
	SDL_Rect	sRect;

	static	int	FontColorkey;

	char	Number[10];
	
	static	int Count,Phase;

	static	Uint32	Perfect, Great, Good, Bad, Miss;
	
	static	Uint32		i;

	static	char	Judge1p,Judge2p;		// 1p, 2p --'S' 'A' 'B' 'C' 'F'

	if(First==0)
	{
		Phase=0;

		sRect.x=0;
		sRect.y=0;
		sRect.w=640;
		sRect.h=480;

		for(Count=480;Count>0;Count-=24)
		{
			ClpBlt3( Count,Count,ResultBack, sRect );
		}
		Judge1p=JudgeAnaly1p();
		Judge2p=JudgeAnaly2p();

		First++;
	}

	ReadGameInput();
	
	if(PressedKey1p[5])if(Phase!=7 && Start1p)Phase=6;
	if(PressedKey2p[5])if(Phase!=7 && Start2p)Phase=6;

    ResultBack.BltFast( 0, 0, gScreen );
		
	if(Phase>=0)
	{
		sRect.y=225;
		sRect.x=240;
		sRect.w=170;
		sRect.h=45;//Perfect
		
		if(Phase==0)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
        if(Phase>0) {
            ResultFont.SetAlpha( 255 );
            ResultFont.BltFast( 235, 115, gScreen, &sRect );
        }
        else {
            ResultFont.SetAlpha( Count );
            ResultFont.BltFast( 235, 115, gScreen, &sRect );
        }
    }

	if(Phase>=1)
	{
		sRect.y=225;
		sRect.x=425;
		sRect.w=150;
		sRect.h=45;//Great
		
		if(Phase==1)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
        if(Phase>1) {
            ResultFont.SetAlpha( 255 );
            ResultFont.BltFast( 235, 166, gScreen, &sRect );
        }
        else {
            ResultFont.SetAlpha( Count );
            ResultFont.BltFast( 235, 166, gScreen, &sRect );
        }
	}
	
	if(Phase>=2)
	{
		sRect.y=275;
		sRect.x=235;
		sRect.w=145;
		sRect.h=45;//Good
		
		if(Phase==2)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
        if(Phase>2) {
            ResultFont.SetAlpha( 255 );
            ResultFont.BltFast( 250, 220, gScreen, &sRect );
        }
        else {
            ResultFont.SetAlpha( Count );
            ResultFont.BltFast( 250, 220, gScreen, &sRect );
        }
	}

	if(Phase>=3)
	{
		sRect.y=275;
		sRect.x=430;
		sRect.w=85;
		sRect.h=45;//Bad
		
		if(Phase==3)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}

        if(Phase>3) {
            ResultFont.SetAlpha( 255 );
        }
        else {
            ResultFont.SetAlpha( Count );
        }
        ResultFont.BltFast( 275, 268, gScreen, &sRect );
	}

	if(Phase>=4)
	{
		sRect.y=328;
		sRect.x=240;
		sRect.w=110;
		sRect.h=45;//Miss
		
		if(Phase==4)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}

        if(Phase>4) {
            ResultFont.SetAlpha( 255 );
        }
        else {
            ResultFont.SetAlpha( Count );
        }
        ResultFont.BltFast( 263, 323, gScreen, &sRect );
	}

	if(Phase>=5)
	{
		sRect.y=380;
		sRect.x=242;
		sRect.w=268;
		sRect.h=45;//MAX Combo
		
		if(Phase==5)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}

        if(Phase>5) {
            ResultFont.SetAlpha( 255 );
        }
        else {
            ResultFont.SetAlpha( Count );
        }
        ResultFont.BltFast( 190, 373, gScreen, &sRect );
	}

	if(Phase==0)
	{
		if(Start1p)
		{
			if(cPerfect1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cPerfect1p);
			DisplayNumber(80,120,Number);
		}
		if(Start2p)
		{
			if(cPerfect2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cPerfect2p);
			DisplayNumber(480,120,Number);
		}
		i++;
		if(cPerfect1p<i && cPerfect2p<i)
		{
			Phase=1;
			i=0;
			Count=0;
		}
	}
	if(Phase==1)
	{
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
		
			if(cGreat1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);

			if(cGreat2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
		}
		i++;
		if(cGreat1p<i && cGreat2p<i)
		{
			Phase=2;
			i=0;
			Count=0;
		}
	}
	if(Phase==2)
	{

		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);

			if(cGood1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);

			if(cGood2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);
		}
		i++;
		if(cGood1p<i && cGood2p<i)
		{
			Phase=3;
			i=0;
			Count=0;
		}
	}

	if(Phase==3)
	{
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);

			if(cBad1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cBad1p);
			DisplayNumber(80,270,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);

			if(cBad2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cBad2p);
			DisplayNumber(480,270,Number);
		}
		i++;
		if(cBad1p<i && cBad2p<i)
		{
			Phase=4;
			i=0;
			Count=0;
		}
	}

	if(Phase==4)
	{
		
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",cBad1p);
			DisplayNumber(80,270,Number);
			
			if(cMiss1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cMiss1p);
			DisplayNumber(80,320,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",cBad2p);
			DisplayNumber(480,270,Number);

			if(cMiss2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cMiss2p);
			DisplayNumber(480,320,Number);
		}
		i++;
		if(cMiss1p<i && cMiss2p<i)
		{
			Phase=5;
			i=0;
			Count=0;
		}
	}
	if(Phase==5)
	{
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",cBad1p);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",cMiss1p);
			DisplayNumber(80,320,Number);
			
			if(cMaxCombo1p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cMaxCombo1p);
			DisplayNumber(80,375,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",cBad2p);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",cMiss2p);
			DisplayNumber(480,320,Number);

			if(cMaxCombo2p>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",cMaxCombo2p);
			DisplayNumber(480,375,Number);
		}
		i++;
		if(cMaxCombo1p<i && cMaxCombo2p<i)
		{
			Phase=6;
			i=0;
			Count=0;
		}
	}

	if(Phase==6)
	{
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",cBad1p);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",cMiss1p);
			DisplayNumber(80,320,Number);
			sprintf(Number,"%03d",cMaxCombo1p);
			DisplayNumber(80,375,Number);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",cBad2p);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",cMiss2p);
			DisplayNumber(480,320,Number);
			sprintf(Number,"%03d",cMaxCombo2p);
			DisplayNumber(480,375,Number);
		}
		
		i++;

		if(i>=60)
		{
			Phase=7;
			i=0;
			Count=0;
		}
	}

	if(Phase==7)
	{
		if(Start1p)
		{
			sprintf(Number, "%03d", cPerfect1p);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",cGreat1p);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",cGood1p);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",cBad1p);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",cMiss1p);
			DisplayNumber(80,320,Number);
			sprintf(Number,"%03d",cMaxCombo1p);
			DisplayNumber(80,375,Number);
			DisplayJudge(70,140,Judge1p,FontColorkey);
		}
		if(Start2p)
		{
			sprintf(Number, "%03d", cPerfect2p);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",cGreat2p);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",cGood2p);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",cBad2p);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",cMiss2p);
			DisplayNumber(480,320,Number);
			sprintf(Number,"%03d",cMaxCombo2p);
			DisplayNumber(480,375,Number);
			DisplayJudge(470,140,Judge2p,FontColorkey);
		}

		i++;
		if(i==60)
		{
			First=0;
			// MaxState is 4
			if(dwGameCount==3 || (Judge1p=='F' && Judge2p=='F') ) {
				g_ProgramState=GAMEOVER;
				dwGameCount=0;
			}
			else
			{
				dwGameCount++;
				g_ProgramState=SELECTSONG;
			}
		}
	}
}
