/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

#include "Dead.h"
#include "Main.h"
#include "Chunk.h"
#include "Timer.h"

extern  Chunk   gSndDead;
extern	Surface     DeadScreen;
extern	Surface		GameOver;

extern	int 	dwGameCount;

void Dead(void)
{
	static Timer    timer;
	static SDL_Rect	UpRect, DownRect;

	static int Firs;

	Score1p=Score2p=0;

	if(Firs==0)
	{
		UpRect.y=240;
		UpRect.x=0;
		UpRect.w=640;
		UpRect.h=0;

		DownRect.y=240;
		DownRect.x=0;
		DownRect.w=640;
		DownRect.h=0;

		timer.Start();
		
		Firs++;
	}

	if(Firs==1)
	{
        gSndDead.Play();

        if( 50 < timer.GetTicks() )
        {
            timer.Start();
            if(UpRect.y==0) {
                Firs++;
            }
		    else
		    {
			    UpRect.y -= 40;
                UpRect.h += 40;
			    DownRect.h += 40;
		    }
        }

        DeadScreen.BltFast( 0, 0, gScreen, &UpRect );
        DeadScreen.BltFast( 0, UpRect.y+240, gScreen, &DownRect );
	}

	if(Firs==2)
	{
        DeadScreen.BltFast( 0, 0, gScreen, &UpRect );

		if(2500 < timer.GetTicks() )
		{
			Firs=0;
			Start1p=Start2p=false;
			dwGameCount=0;

			g_ProgramState=GAMEOVER;
		}
	}
}
