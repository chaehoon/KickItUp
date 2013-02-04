#ifndef __INPUT_INCLUDED__
#define __INPUT_INCLDUED__

#include <dinput.h>

#define KEYBUFSIZE		256

#define	IDC_DEVICES		200

extern void (*ReadGameInput)(void);//=ReadKeyboardInput;
extern void (*ReadJoystickInput2)(void);

void	ReadKeyboardInput(void);
void	ReadFusionInput(void);
void	ReadKoinsInput(void);
void	ReadDDRInput(void);

BOOL	PickInputDevice(int N);
BOOL InitKeyboard(LPDIRECTINPUT pdi);
HRESULT	InitDI(HINSTANCE hInstance);
void CleanupInput(void); 
BOOL	ReacquireInput(void);
BOOL	PickInputDevice2(int N);
BOOL	ReacquireInput2(void);

#endif