#ifndef _KICKITUP_INPUT_H
#define _KICKITUP_INPUT_H

extern void (*ReadGameInput)(void); // = ReadKeyboardInput;
// extern void (*ReadJoystickInput2)(void);

void	ReadKeyboardInput(void);
//void	ReadFusionInput(void);
//void	ReadKoinsInput(void);
//void	ReadDDRInput(void);

#endif	// _INPUT_H
