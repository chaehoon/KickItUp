/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	2000.05.xx. Work for Koins pad(5-way joystick port) support. NOT completly implemented.
*/

#include "Input.h"
#include "Main.h"
#include <SDL/SDL.h>

extern	void ReadKeyboardInput();
//extern	void ReadFusionInput();
//extern	void ReadDDRInput();
//extern	void ReadKoinsInput();
void (*ReadGameInput)(void)=ReadKeyboardInput;

// void (*ReadJoystickInput2)(void)=ReadDDRInput;

/*--------------------------------------------------------------------------
| ReadKeyboardInput
|
| Requests keyboard data and performs any needed processing.
*-------------------------------------------------------------------------*/
void ReadKeyboardInput(void)
{
    Uint8 *keystates = SDL_GetKeyState( NULL );

    // 2player
	if(keystates[SDLK_KP1]) {
		if(ArrowState2p[1]==PRESS) {
			PressedKey2p[1]=false;
		} else {
			PressedKey2p[1]=true;
		}
		ArrowState2p[1]=PRESS;
	} else {
		ArrowState2p[1]=NORMAL;
		PressedKey2p[1]=false;
	}
	
	if(keystates[SDLK_KP3]) {
		if(ArrowState2p[3]==PRESS) {
			PressedKey2p[3]=false;
		} else {
			PressedKey2p[3]=true;
		}
		ArrowState2p[3]=PRESS;
	} else {
		ArrowState2p[3]=NORMAL;
		PressedKey2p[3]=false;
	}


	if(keystates[SDLK_KP5]) {
		if(ArrowState2p[5]==PRESS) {
			PressedKey2p[5]=false;
		} else {
			PressedKey2p[5]=true;
		}
		ArrowState2p[5]=PRESS;
	} else {
		ArrowState2p[5]=NORMAL;
		PressedKey2p[5]=false;
	}

	if(keystates[SDLK_KP7]) {
		if(ArrowState2p[7]==PRESS) {
			PressedKey2p[7]=false;
		} else {
			PressedKey2p[7]=true;
		}
		ArrowState2p[7]=PRESS;
	} else {
		ArrowState2p[7]=NORMAL;
		PressedKey2p[7]=false;
	}

	if(keystates[SDLK_KP9]) {
		if(ArrowState2p[9]==PRESS) {
			PressedKey2p[9]=false;
		} else {
			PressedKey2p[9]=true;
		}
		ArrowState2p[9]=PRESS;
	} else {
		ArrowState2p[9]=NORMAL;
		PressedKey2p[9]=false;
	}

	// 1 player
	if(keystates[SDLK_z]) {
		if(ArrowState1p[1]==PRESS) {
			PressedKey1p[1]=false;
		} else {
			PressedKey1p[1]=true;
		}
		ArrowState1p[1]=PRESS;
	} else {
		ArrowState1p[1]=NORMAL;
		PressedKey1p[1]=false;
	}
	

	if(keystates[SDLK_c]) {
		if(ArrowState1p[3]==PRESS) {
			PressedKey1p[3]=false;
		} else {
			PressedKey1p[3]=true;
		}
		ArrowState1p[3]=PRESS;
	} else {
		ArrowState1p[3]=NORMAL;
		PressedKey1p[3]=false;
	}

	if(keystates[SDLK_s]) {
		if(ArrowState1p[5]==PRESS) {
			PressedKey1p[5]=false;
		} else {
			PressedKey1p[5]=true;
		}
		ArrowState1p[5]=PRESS;
	} else {
		ArrowState1p[5]=NORMAL;
		PressedKey1p[5]=false;
	}

	if(keystates[SDLK_q]) {
		if(ArrowState1p[7]==PRESS) {
			PressedKey1p[7]=false;
		} else {
			PressedKey1p[7]=true;
		}
		ArrowState1p[7]=PRESS;
	} else {
		ArrowState1p[7]=NORMAL;
		PressedKey1p[7]=false;
	}

	if(keystates[SDLK_e]) {
		if(ArrowState1p[9]==PRESS) {
			PressedKey1p[9]=false;
		} else {
			PressedKey1p[9]=true;
		}
		ArrowState1p[9]=PRESS;
	} else {
		ArrowState1p[9]=NORMAL;
		PressedKey1p[9]=false;
	}

    /*
	if(bJoy==true) {
		ReadJoystickInput2();

		if(PressedKey1p[1] || PressedKey_Joy[1])PressedKey1p[1]=true;
		if(PressedKey1p[3] || PressedKey_Joy[3])PressedKey1p[3]=true;
		if(PressedKey1p[5] || PressedKey_Joy[5])PressedKey1p[5]=true;
		if(PressedKey1p[7] || PressedKey_Joy[7])PressedKey1p[7]=true;
		if(PressedKey1p[9] || PressedKey_Joy[9])PressedKey1p[9]=true;
	}
    */
}
