/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

#include <ddraw.h>

#include "main.h"
#include "input.h"
#include <stdio.h>
#include "ddutil.h"

extern	LPDIRECTDRAWSURFACE	ResultBack;
extern	LPDIRECTDRAWSURFACE	ResultFont;
extern	LPDIRECTDRAWSURFACE	Background;
extern	LPDIRECTDRAWSURFACE	NumberFont;

extern	char First;
extern	char	Double;
extern	DWORD dwGameCount;

HRESULT	ClpBlt2(int x ,int y ,LPDIRECTDRAWSURFACE ds,LPRECT srect,DWORD mode)
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
	
	hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);

	memcpy(srect,&sRect,sizeof(sRect));
	
	return hRet;

}

char JudgeAnaly1p(void)
{
	DWORD	cTotal1p;

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
	DWORD	cTotal2p;

	cTotal2p=cPerfect2p+cGreat2p+cGood2p+cBad2p+cMiss2p;

	if(Gauge2p<0)if(!Double)return	'F';

	if(cGood2p==0 && cBad2p==0 && cMiss2p==0)return	'S';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>95)return	'A';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>80)return	'B';

	if((((double)(cPerfect2p+cGreat2p)/(double)cTotal2p)*100)>60)return	'C';

	return	'F';
}

void DisplayJudge(int x, int y, char s, DWORD ColorKey)
{
	RECT	sRect;

	switch(s)
	{
		case	'A':
			sRect.top=2;
			sRect.left=7;
			sRect.right=153;
			sRect.bottom=220;

			break;
		case	'B':
			sRect.top=2;
			sRect.left=155;
			sRect.right=290;
			sRect.bottom=220;

			break;
		case	'C':
			sRect.top=2;
			sRect.left=295;
			sRect.right=438;
			sRect.bottom=220;

			break;
		case	'D':
			sRect.top=2;
			sRect.left=450;
			sRect.right=585;
			sRect.bottom=220;

			break;
		case	'F':
			sRect.top=225;
			sRect.left=127;
			sRect.right=225;
			sRect.bottom=443;

			break;
		case	'S':
			sRect.top=225;
			sRect.left=0;
			sRect.right=127;
			sRect.bottom=446;
			break;
	}

	TransAlphaImproved(ResultFont, g_pDDSBack, x, y, sRect, 150, ColorKey, 16);
}


void	DisplayNumber(int x, int y, char *message)
{
	int Loop;
	RECT	cRect;
	
	for(Loop=0;;Loop++)
	{
		if(message[Loop]==NULL)break;
		message[Loop]-=48;
		cRect.left=message[Loop]*30;
		cRect.right=cRect.left+30;
		cRect.top=0;
		cRect.bottom=38;

		g_pDDSBack->BltFast(x+Loop*30,y,NumberFont,&cRect,DDBLTFAST_SRCCOLORKEY);
	}/* 여기까지 */
}

void Result(void)
{
	RECT	sRect;

	static	int	FontColorkey;

	char	Number[10];
	
	static	int Count,Phase;

	static	DWORD	Perfect, Great, Good, Bad, Miss;
	
	static	DWORD		i;

	static	char	Judge1p,Judge2p;		// 1p, 2p --'S' 'A' 'B' 'C' 'F'

	if(First==0)
	{
		FontColorkey=DDColorMatch(ResultFont,CLR_INVALID);
		Phase=0;

		sRect.top=0;
		sRect.left=0;
		sRect.right=640;
		sRect.bottom=480;

		for(Count=480;Count>0;Count-=24)
		{
			ClpBlt2(Count,Count,ResultBack,&sRect,DDBLTFAST_NOCOLORKEY);
			Flipp();
		}
		Judge1p=JudgeAnaly1p();
		Judge2p=JudgeAnaly2p();

		First++;
	}

	ReadGameInput();
	
	if(PressedKey1p[5])if(Phase!=7 && Start1p)Phase=6;
	if(PressedKey2p[5])if(Phase!=7 && Start2p)Phase=6;

	g_pDDSBack->BltFast(0,0,ResultBack, NULL,DDBLTFAST_NOCOLORKEY);
	
	if(Phase>=0)
	{
		sRect.top=225;
		sRect.left=240;
		sRect.right=410;
		sRect.bottom=270;//Perfect
		
		if(Phase==0)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>0)g_pDDSBack->BltFast(235, 115, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 235,115, sRect, Count, FontColorkey, 16);
		//if(Phase>0)Count=0;
	}

	if(Phase>=1)
	{
		sRect.top=225;
		sRect.left=425;
		sRect.right=575;
		sRect.bottom=270;//Great
		
		if(Phase==1)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>1)g_pDDSBack->BltFast(253, 166, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 253,166,sRect,Count, FontColorkey, 16);
		//if(Phase>1)Count=0;
	}
	
	if(Phase>=2)
	{
		sRect.top=275;
		sRect.left=235;
		sRect.right=380;
		sRect.bottom=325;//Good
		
		if(Phase==2)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>2)g_pDDSBack->BltFast(250, 220, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 250,220,sRect,Count ,FontColorkey, 16);
		//if(Phase>2)Count=0;
	}

	if(Phase>=3)
	{
		sRect.top=275;
		sRect.left=430;
		sRect.right=515;
		sRect.bottom=320;//Bad
		
		if(Phase==3)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>3)g_pDDSBack->BltFast(275, 268, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 275,268,sRect,Count ,FontColorkey, 16);
		//if(Phase>3)Count=0;
	}

	if(Phase>=4)
	{
		sRect.top=328;
		sRect.left=240;
		sRect.right=350;
		sRect.bottom=370;//Miss
		
		if(Phase==4)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>4)g_pDDSBack->BltFast(263, 323, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 263,323,sRect,Count ,FontColorkey, 16);
		//if(Phase>4)Count=0;
	}

	if(Phase>=5)
	{
		sRect.top=380;
		sRect.left=242;
		sRect.right=510;
		sRect.bottom=425;//MAX Combo
		
		if(Phase==5)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>5)g_pDDSBack->BltFast(190, 373, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 190,373,sRect,Count ,FontColorkey, 16);
		//if(Phase>5)Count=0;
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

	Flipp();
}
