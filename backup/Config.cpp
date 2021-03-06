/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

#include "Main.h"
#include "Config.h"
#include "Input.h"

extern	KIUCONFIG			KCFG;

void ReadNullInput()
{
}

void	CFGInitialize(void)
{
	FILE *F;

	KCFG.bcDead = true;
	KCFG.auto1_1p = false;
	KCFG.auto3_1p = false;
	KCFG.auto5_1p = false;
	KCFG.auto7_1p = false;
	KCFG.auto9_1p = false;

	KCFG.auto1_2p = false;
	KCFG.auto3_2p = false;
	KCFG.auto5_2p = false;
	KCFG.auto7_2p = false;
	KCFG.auto9_2p = false;
	KCFG.KeySet   = NORMAL;
	KCFG.OptJudge = true;

	ReadGameInput = ReadKeyboardInput;

	F=fopen("kiu.cfg","rb");

	if(F==NULL)return;
	
	fread(&KCFG, sizeof(KCFG), 1, F);
}

void	CFGWrite(void)
{
	FILE *F;

	F=fopen("kiu.cfg","wb");

	if(F==NULL)return;

	fwrite(&KCFG, sizeof(KCFG), 1, F);

	fclose(F);
}

void Configuration(void)
{
	#define	FONT_SIZE_Y	16
	#define	START_X		0
	
	static	int	cur_y;
	
	gScreen.FillRect( 0, 0x000400 );

	DisplayMessage(40, START_X, "STAGE BREAK"); // 0
	if(KCFG.bcDead)
		DisplayMessage(500, START_X, "ON");
	else
		DisplayMessage(500,START_X, "OFF");

	DisplayMessage(40, START_X+FONT_SIZE_Y*1, "JUDGEMENT PICTURE"); // 1
	if(KCFG.OptJudge)
		DisplayMessage(500,START_X+FONT_SIZE_Y*1,"ON");
	else
		DisplayMessage(500,START_X+FONT_SIZE_Y*1,"OFF");
	
	DisplayMessage(40, START_X+FONT_SIZE_Y*2, "KEY SETTING : "); // 2
	if(KCFG.KeySet == NORMAL)
		DisplayMessage(500, START_X+FONT_SIZE_Y*2,"NORMAL");
	else if (KCFG.KeySet == FUSION)
		DisplayMessage(500, START_X+FONT_SIZE_Y*2,"FUSION");

	DisplayMessage(40, START_X+FONT_SIZE_Y*3, "JOY SETTING : "); // 3
	if(KCFG.JoySet == DDR)
		DisplayMessage(500, START_X+FONT_SIZE_Y*3, "DDR");
	else if(KCFG.JoySet == KOINS)
		DisplayMessage(500, START_X+FONT_SIZE_Y*3, "KOINS");
	
	DisplayMessage(40, START_X+FONT_SIZE_Y*4, "");
	DisplayMessage(40, START_X+FONT_SIZE_Y*5, "");

	DisplayMessage(40, START_X+FONT_SIZE_Y*6, "AUTO KEY FOR 1P"); // 6

	DisplayMessage(324,START_X+FONT_SIZE_Y*7, "DOWN LEFT :"); // 7
	if(KCFG.auto1_1p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*7, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*7, "OFF");
	
	DisplayMessage(324,START_X+FONT_SIZE_Y*8, "UP LEFT :"); // 8
	if(KCFG.auto7_1p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*8, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*8, "OFF");
	
	DisplayMessage(324,START_X+FONT_SIZE_Y*9, "CENTER :"); // 9
	if(KCFG.auto5_1p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*9, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*9, "OFF");
	
	DisplayMessage(324, START_X+FONT_SIZE_Y*10, "UP RIGHT :"); // 10
	if(KCFG.auto9_1p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*10, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*10, "OFF");
	
	DisplayMessage(324, START_X+FONT_SIZE_Y*11, "DOWN RIGHT :"); // 11
	if(KCFG.auto3_1p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*11, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*11, "OFF");

	DisplayMessage(40, START_X+FONT_SIZE_Y*12, "AUTO KEY FOR 2P");

	DisplayMessage(324,START_X+FONT_SIZE_Y*13, "DOWN LEFT :"); // 13
	if(KCFG.auto1_2p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*13, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*13, "OFF");
	
	DisplayMessage(324,START_X+FONT_SIZE_Y*14, "UP LEFT :"); // 14
	if(KCFG.auto7_2p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*14, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*14, "OFF");
	
	DisplayMessage(324,START_X+FONT_SIZE_Y*15, "CENTER :"); // 15
	if(KCFG.auto5_2p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*15, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*15, "OFF");
	
	DisplayMessage(324, START_X+FONT_SIZE_Y*16, "UP RIGHT :"); // 16
	if(KCFG.auto9_2p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*16, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*16, "OFF");
	
	DisplayMessage(324, START_X+FONT_SIZE_Y*17, "DOWN RIGHT :"); // 17
	if(KCFG.auto3_2p)
		DisplayMessage(500, START_X+FONT_SIZE_Y*17, "ON");
	else
		DisplayMessage(500, START_X+FONT_SIZE_Y*17, "OFF");
	
  	DisplayMessage(0,START_X+cur_y*16,">");

	switch(PressedKey2p[0]) {
		case 3:
			switch(cur_y) {
				case 0:
					if(KCFG.bcDead)
						KCFG.bcDead=false;
					else
						KCFG.bcDead=true;
					break;
				case 1:
					if(KCFG.OptJudge)
						KCFG.OptJudge=false;
					else
						KCFG.OptJudge=true;
					break;
				case 2:
					if(KCFG.KeySet == NORMAL)
						ReadGameInput = ReadKeyboardInput;
					//   KCFG.KeySet=FUSION, ReadGameInput = ReadFusionInput;
					// else 
					//if(	KCFG.KeySet == FUSION)KCFG.KeySet=NORMAL, ReadGameInput = ReadKeyboardInput;
					break;
				case 3:
					//if(KCFG.JoySet == DDR) KCFG.JoySet=KOINS, ReadJoystickInput2 = ReadKoinsInput;
					//else if(KCFG.JoySet == KOINS) KCFG.JoySet=DDR, ReadJoystickInput2 = ReadDDRInput;
					break;
				case 7:
					if(KCFG.auto1_1p)
						KCFG.auto1_1p=false;
					else
						KCFG.auto1_1p=true;
					break;
				case 8:
					if(KCFG.auto7_1p)
						KCFG.auto7_1p=false;
					else
						KCFG.auto7_1p=true;
					break;
				case 9:
					if(KCFG.auto5_1p)
						KCFG.auto5_1p=false;
					else
						KCFG.auto5_1p=true;
					break;
				case 10:
					if(KCFG.auto9_1p)
						KCFG.auto9_1p=false;
					else
						KCFG.auto9_1p=true;
					break;
				case 11:
					if(KCFG.auto3_1p)
						KCFG.auto3_1p=false;
					else
						KCFG.auto3_1p=true;
					break;
				case 13:
					if(KCFG.auto1_2p)
						KCFG.auto1_2p=false;
					else
						KCFG.auto1_2p=true;
					break;
				case 14:
					if(KCFG.auto7_2p)
						KCFG.auto7_2p=false;
					else
						KCFG.auto7_2p=true;
					break;
				case 15:
					if(KCFG.auto5_2p)
						KCFG.auto5_2p=false;
					else
						KCFG.auto5_2p=true;
					break;
				case 16:
					if(KCFG.auto9_2p)
						KCFG.auto9_2p=false;
					else
						KCFG.auto9_2p=true;
					break;
				case 17:
					if(KCFG.auto3_2p)
						KCFG.auto3_2p=false;
					else
						KCFG.auto3_2p=true;
					break;
			}
			PressedKey2p[0]=0;
			break;
		case 8:
			cur_y--;

			switch(cur_y) {
				case -1:
					cur_y=17;
					break;
				case 4:
				case 5:
				case 6:
					cur_y=3;
					break;
				case 12:
					cur_y=11;
					break;
			}
			PressedKey2p[0]=0;
			break;
		case 2:
			cur_y++;
			switch(cur_y) {
				case 18:
					cur_y=0;
					break;
				case 4:
				case 5:
				case 6:
					cur_y=7;
					break;
				case 12:
					cur_y=13;
					break;
			}
			PressedKey2p[0]=0;
			break;
	}
}

