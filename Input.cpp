/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	2000.05.xx. Work for Koins pad(5-way joystick port) support. NOT completly implemented.
*/

#include "input.h"
#include "main.h"
#include <dinput.h>

#define	MAX_DINPUT_DEVICES	10
int	g_cpdevFound;

extern	void ReadKeyboardInput();
extern	void ReadFusionInput();
extern	void ReadDDRInput();
extern	void ReadKoinsInput();
void (*ReadGameInput)(void)=ReadKeyboardInput;

void (*ReadJoystickInput2)(void)=ReadDDRInput;

BOOL	bJoy;

LPDIRECTINPUTDEVICE2 g_rgpdevFound[MAX_DINPUT_DEVICES];
LPDIRECTINPUTDEVICE2 g_pdevCurrent;
LPDIRECTINPUTDEVICE2 g_pdevJoystick;

/*--------------------------------------------------------------------------
| AddInputDevice
|
| Records an input device in the array of found devices.
|
| In addition to stashing it in the array, we also add it to the device
| menu so the user can pick it.
|
*-------------------------------------------------------------------------*/

void AddInputDevice(LPDIRECTINPUTDEVICE8 pdev, LPCDIDEVICEINSTANCE pdi)
{

    if (g_cpdevFound < MAX_DINPUT_DEVICES) {

        HRESULT hRes;

        /*
         *  Convert it to a Device2 so we can Poll() it.
         */

        hRes = pdev->QueryInterface(
                     IID_IDirectInputDevice8,
                    (LPVOID *)&g_rgpdevFound[g_cpdevFound]);

        if (SUCCEEDED(hRes)) {

            HMENU hmenu;

            /*
             *  Add its description to the menu.
             */
            hmenu = GetSubMenu(GetMenu(hWnd), 0);

            InsertMenu(hmenu, g_cpdevFound, MF_BYPOSITION | MF_STRING,
                       IDC_DEVICES + g_cpdevFound,
                       pdi->tszInstanceName);

            g_cpdevFound++;
        }
    }
}

/*--------------------------------------------------------------------------
|
| SetDIDwordProperty
|
| Set a DWORD property on a DirectInputDevice.
|
*-------------------------------------------------------------------------*/

HRESULT
SetDIDwordProperty(LPDIRECTINPUTDEVICE8 pdev, REFGUID guidProperty,
                   DWORD dwObject, DWORD dwHow, DWORD dwValue)
{
   DIPROPDWORD dipdw;

   dipdw.diph.dwSize       = sizeof(dipdw);
   dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
   dipdw.diph.dwObj        = dwObject;
   dipdw.diph.dwHow        = dwHow;
   dipdw.dwData            = dwValue;

   return pdev->SetProperty(guidProperty, &dipdw.diph);

}

/*--------------------------------------------------------------------------
| InitKeyboardInput
|
| Initializes DirectInput for the keyboard.  Creates a keyboard device,
| sets the data format to our custom format, sets the cooperative level and
| adds it to the menu.
|
*-------------------------------------------------------------------------*/

BOOL InitKeyboardInput(LPDIRECTINPUT8 pdi)
{
   LPDIRECTINPUTDEVICE8 pdev;
   DIDEVICEINSTANCE di;

   // create the DirectInput keyboard device
   if(pdi->CreateDevice(GUID_SysKeyboard, &pdev, NULL) != DI_OK)
   {
      OutputDebugString("IDirectInput::CreateDevice FAILED\n");
      return FALSE;
   }

   // set the cooperative level
   if(pdev->SetCooperativeLevel( hWnd,
      DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetCooperativeLevel FAILED\n");
      pdev->Release();
      return FALSE;
   }

   // set keyboard data format
   if(pdev->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetDataFormat FAILED\n");
      pdev->Release();
      return FALSE;
   }

   // set buffer size
   if (SetDIDwordProperty(pdev, DIPROP_BUFFERSIZE, 0, DIPH_DEVICE, KEYBUFSIZE) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEVICE) FAILED\n");
      pdev->Release();
      return FALSE;
   }

   //
   // Get the name of the primary keyboard so we can add it to the menu.
   //
   di.dwSize = sizeof(di);
   if (pdev->GetDeviceInfo( &di) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::GetDeviceInfo FAILED\n");
      pdev->Release();
      return FALSE;
   }

   //
   // Add it to our list of devices.  If AddInputDevice succeeds,
   // he will do an AddRef.
   //
   AddInputDevice(pdev, &di);
   pdev->Release();

   return TRUE;
}

/*--------------------------------------------------------------------------
| InitJoystickInput
|
| Initializes DirectInput for an enumerated joystick device.
| Creates the device, device, sets the standard joystick data format,
| sets the cooperative level and adds it to the menu.
|
| If any step fails, just skip the device and go on to the next one.
|
*-------------------------------------------------------------------------*/

BOOL FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef)
{
   LPDIRECTINPUT8 pdi = (LPDIRECTINPUT8)pvRef;
   LPDIRECTINPUTDEVICE8 pdev;
   DIPROPRANGE diprg;

   // create the DirectInput joystick device
   if(pdi->CreateDevice( pdinst->guidInstance, &pdev, NULL) != DI_OK)
   {
      OutputDebugString("IDirectInput::CreateDevice FAILED\n");
      return DIENUM_CONTINUE;
   }

   // set joystick data format
   if(pdev->SetDataFormat( &c_dfDIJoystick) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetDataFormat FAILED\n");
      pdev->Release();
      return DIENUM_CONTINUE;
   }

   // set the cooperative level
   if(pdev->SetCooperativeLevel( hWnd,
      DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetCooperativeLevel FAILED\n");
      pdev->Release();
      return DIENUM_CONTINUE;
   }

   // set X-axis range to (-1000 ... +1000)
   // This lets us test against 0 to see which way the stick is pointed.
/*
   diprg.diph.dwSize       = sizeof(diprg);
   diprg.diph.dwHeaderSize = sizeof(diprg.diph);
   diprg.diph.dwObj        = DIJOFS_X;
   diprg.diph.dwHow        = DIPH_BYOFFSET;
   diprg.lMin              = -1000;
   diprg.lMax              = +1000;
*/
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = DIJOFS_X;
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = DIPH_BYOFFSET;
	diprg.lMax = +5000;

   if(pdev->SetProperty( DIPROP_RANGE, &diprg.diph) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");
      pdev->Release();
      return FALSE;
   }
/*
   //
   // And again for Y-axis range
   //
   diprg.diph.dwObj        = DIJOFS_Y;

   if(pdev->SetProperty( DIPROP_RANGE, &diprg.diph) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");
      pdev->Release();
      return FALSE;
   }

   // set X axis dead zone to 50% (to avoid accidental turning)
   // Units are ten thousandths, so 50% = 5000/10000.
   if (SetDIDwordProperty(pdev, DIPROP_DEADZONE, DIJOFS_X, DIPH_BYOFFSET, 5000) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEADZONE) FAILED\n");
      pdev->Release();
      return FALSE;
   }


   // set Y axis dead zone to 50% (to avoid accidental thrust)
   // Units are ten thousandths, so 50% = 5000/10000.
   if (SetDIDwordProperty(pdev, DIPROP_DEADZONE, DIJOFS_Y, DIPH_BYOFFSET, 5000) != DI_OK)
   {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEADZONE) FAILED\n");
      pdev->Release();
      return FALSE;
   }
*/

   //
   // Add it to our list of devices.  If AddInputDevice succeeds,
   // he will do an AddRef.
   //
   AddInputDevice(pdev, pdinst);
   pdev->Release();

   return DIENUM_CONTINUE;
}

/*--------------------------------------------------------------------------
| InitInput
|
| Initializes DirectInput for the keyboard and all joysticks.
|
| For each input device, add it to the menu.
|
*-------------------------------------------------------------------------*/

HRESULT InitDI(HINSTANCE hInst)
{
	LPDIRECTINPUT8	pdi;
	if(DirectInput8Create(hInst, 0x0800, IID_IDirectInput8, (LPVOID*)&pdi, NULL) != DI_OK) {
		OutputDebugString("DirectInputCreate FAILED\n");
		return FALSE;
	}
	
   BOOL fRc = InitKeyboardInput(pdi);
   // pdi->Release();       // Finished with DX 3.0

   if (!fRc) {
      return FALSE;
   }

  //
  // Enumerate the joystick devices.  If it doesn't work, oh well,
  // at least we got the keyboard.
  //

  pdi->EnumDevices(DI8DEVCLASS_GAMECTRL, InitJoystickInput, (LPVOID)pdi, DIEDFL_FORCEFEEDBACK |DIEDFL_ATTACHEDONLY);

  // pdi->Release();    // Finished with DX 5.0.

   // Default device is the keyboard
   PickInputDevice(0);
   bJoy=PickInputDevice2(1);

	/*
   LPDIRECTINPUT pdi;
   BOOL fRc;

   // Note: Joystick support is a DirectX 5.0 feature.
   // Since we also want to run on DirectX 3.0, we will start out
   // with DirectX 3.0 to make sure that at least we get the keyboard.

   // create the DirectInput interface object
   if(DirectInputCreate(hInst, 0x0300, &pdi, NULL) != DI_OK)
   {
      OutputDebugString("DirectInputCreate 3.0 FAILED\n");
      return FALSE;
   }

   fRc = InitKeyboardInput(pdi);
   pdi->Release();       // Finished with DX 3.0

   if (!fRc) {
      return FALSE;
   }

   // create the DirectInput 5.0 interface object
   if(DirectInputCreate(hInst, DIRECTINPUT_VERSION, &pdi, NULL) == DI_OK)
   {

      //
      // Enumerate the joystick devices.  If it doesn't work, oh well,
      // at least we got the keyboard.
      //

      pdi->EnumDevices(DIDEVTYPE_JOYSTICK,
                               InitJoystickInput, pdi, DIEDFL_ATTACHEDONLY);


      pdi->Release();    // Finished with DX 5.0.

   } else {
      OutputDebugString("DirectInputCreate 5.0 FAILED - no joystick support\n");
   }

   // Default device is the keyboard
   PickInputDevice(0);
   bJoy=PickInputDevice2(1);
   
	//if(DEBUGFLAG)fprintf(DEBUGFILE,"bJoy  %d\n", bJoy);

   // if we get here, we were successful
   */
   return TRUE;
}

/*--------------------------------------------------------------------------
| CleanupInput
|
| Cleans up all DirectInput objects.
*-------------------------------------------------------------------------*/
void CleanupInput(void)
{
   int idev;

   // make sure the device is unacquired
   // it doesn't harm to unacquire a device that isn't acquired

   if (g_pdevCurrent)
   {
      IDirectInputDevice_Unacquire(g_pdevCurrent);
   }

   // release all the devices we created
   for (idev = 0; idev < g_cpdevFound; idev++)
   {
      if (g_rgpdevFound[idev]) {
         IDirectInputDevice_Release(g_rgpdevFound[idev]);
         g_rgpdevFound[idev] = 0;
      }
   }

}


/*--------------------------------------------------------------------------
| ReacquireInput
|
| Reacquires the current input device.  If Acquire() returns S_FALSE,
| that means
| that we are already acquired and that DirectInput did nothing.
*-------------------------------------------------------------------------*/
BOOL ReacquireInput(void)
{
    HRESULT hRes;

    // if we have a current device
    if(g_pdevCurrent)
    {
       // acquire the device
       hRes = IDirectInputDevice_Acquire(g_pdevCurrent);
       if(SUCCEEDED(hRes))
       {
          // acquisition successful
          return TRUE;
       }
       else
       {
          // acquisition failed
          return FALSE;
       }
    }
    else
    {
       // we don't have a current device
       return FALSE;
    }

}


/*--------------------------------------------------------------------------
| ReadKeyboardInput
|
| Requests keyboard data and performs any needed processing.
*-------------------------------------------------------------------------*/
void ReadKeyboardInput(void)
{
	char	rgKeyData[KEYBUFSIZE];
   DWORD                   dwEvents;
//   DWORD                   dw;
   static DWORD            dwKeyState = 0;
   HRESULT                 hRes;

   // get data from the keyboard
   dwEvents = KEYBUFSIZE;

   hRes=g_pdevCurrent->GetDeviceState(sizeof(rgKeyData),(LPVOID)&rgKeyData);

   if(hRes != DI_OK)
   {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if(hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return ;
   }

// 2player
	if(rgKeyData[DIK_NUMPAD1] & 0x80)
	{
		if(ArrowState2p[1]==PRESS)
		{
			PressedKey2p[1]=FALSE;
		}
		else
		{
			PressedKey2p[1]=TRUE;
			ArrowState2p[1]=PRESS;
		}

	}
	else
	{
		ArrowState2p[1]=NORMAL;
		PressedKey2p[1]=FALSE;
	}
	

	if(rgKeyData[DIK_NUMPAD3] & 0x80)
	{
		if(ArrowState2p[3]==PRESS)
		{
			PressedKey2p[3]=FALSE;
		}
		else
		{
			PressedKey2p[3]=TRUE;
			ArrowState2p[3]=PRESS;
		}
	}
	else
	{
		ArrowState2p[3]=NORMAL;
		PressedKey2p[3]=FALSE;
	}


	if(rgKeyData[DIK_NUMPAD5] & 0x80)
	{
		if(ArrowState2p[5]==PRESS)
		{
			PressedKey2p[5]=FALSE;
		}
		else
		{
			PressedKey2p[5]=TRUE;
			ArrowState2p[5]=PRESS;
		}
	}
	else
	{
		ArrowState2p[5]=NORMAL;
		PressedKey2p[5]=FALSE;
	}

	if(rgKeyData[DIK_NUMPAD7] & 0x80)
	{
		if(ArrowState2p[7]==PRESS)
		{
			PressedKey2p[7]=FALSE;
		}
		else
		{
			PressedKey2p[7]=TRUE;
			ArrowState2p[7]=PRESS;
		}
	}
	else
	{
		ArrowState2p[7]=NORMAL;
		PressedKey2p[7]=FALSE;
	}

	if(rgKeyData[DIK_NUMPAD9] & 0x80)
	{
		if(ArrowState2p[9]==PRESS)
		{
			PressedKey2p[9]=FALSE;
		}
		else
		{
			PressedKey2p[9]=TRUE;
			ArrowState2p[9]=PRESS;
		}
	}
	else
	{
		ArrowState2p[9]=NORMAL;
		PressedKey2p[9]=FALSE;
	}

/*	if(rgKeyData[DIK_COMMA] & 0x80) //for balpan
	{
		if(ArrowState2p[1]==PRESS)
		{
			PressedKey2p[1]=FALSE;
		}
		else
		{
			PressedKey2p[1]=TRUE;
			ArrowState2p[1]=PRESS;
		}

	}
	else
	{
		ArrowState2p[1]=NORMAL;
		PressedKey2p[1]=FALSE;
	}
	

	if(rgKeyData[DIK_SLASH] & 0x80)
	{
		if(ArrowState2p[3]==PRESS)
		{
			PressedKey2p[3]=FALSE;
		}
		else
		{
			PressedKey2p[3]=TRUE;
			ArrowState2p[3]=PRESS;
		}
	}
	else
	{
		ArrowState2p[3]=NORMAL;
		PressedKey2p[3]=FALSE;
	}


	if(rgKeyData[DIK_L] & 0x80)
	{
		if(ArrowState2p[5]==PRESS)
		{
			PressedKey2p[5]=FALSE;
		}
		else
		{
			PressedKey2p[5]=TRUE;
			ArrowState2p[5]=PRESS;
		}
	}
	else
	{
		ArrowState2p[5]=NORMAL;
		PressedKey2p[5]=FALSE;
	}

	if(rgKeyData[DIK_I] & 0x80)
	{
		if(ArrowState2p[7]==PRESS)
		{
			PressedKey2p[7]=FALSE;
		}
		else
		{
			PressedKey2p[7]=TRUE;
			ArrowState2p[7]=PRESS;
		}
	}
	else
	{
		ArrowState2p[7]=NORMAL;
		PressedKey2p[7]=FALSE;
	}

	if(rgKeyData[DIK_P] & 0x80)
	{
		if(ArrowState2p[9]==PRESS)
		{
			PressedKey2p[9]=FALSE;
		}
		else
		{
			PressedKey2p[9]=TRUE;
			ArrowState2p[9]=PRESS;
		}
	}
	else
	{
		ArrowState2p[9]=NORMAL;
		PressedKey2p[9]=FALSE;
	}*/
	// 1 player
	if(rgKeyData[DIK_Z] & 0x80)
	{
		if(ArrowState1p[1]==PRESS)
		{
			PressedKey1p[1]=FALSE;
		}
		else
		{
			PressedKey1p[1]=TRUE;
			ArrowState1p[1]=PRESS;
		}

	}
	else
	{
		ArrowState1p[1]=NORMAL;
		PressedKey1p[1]=FALSE;
	}
	

	if(rgKeyData[DIK_C] & 0x80)
	{
		if(ArrowState1p[3]==PRESS)
		{
			PressedKey1p[3]=FALSE;
		}
		else
		{
			PressedKey1p[3]=TRUE;
			ArrowState1p[3]=PRESS;
		}
	}
	else
	{
		ArrowState1p[3]=NORMAL;
		PressedKey1p[3]=FALSE;
	}


	if(rgKeyData[DIK_S] & 0x80)
	{
		if(ArrowState1p[5]==PRESS)
		{
			PressedKey1p[5]=FALSE;
		}
		else
		{
			PressedKey1p[5]=TRUE;
			ArrowState1p[5]=PRESS;
		}
	}
	else
	{
		ArrowState1p[5]=NORMAL;
		PressedKey1p[5]=FALSE;
	}

	if(rgKeyData[DIK_Q] & 0x80)
	{
		if(ArrowState1p[7]==PRESS)
		{
			PressedKey1p[7]=FALSE;
		}
		else
		{
			PressedKey1p[7]=TRUE;
			ArrowState1p[7]=PRESS;
		}
	}
	else
	{
		ArrowState1p[7]=NORMAL;
		PressedKey1p[7]=FALSE;
	}

	if(rgKeyData[DIK_E] & 0x80)
	{
		if(ArrowState1p[9]==PRESS)
		{
			PressedKey1p[9]=FALSE;
		}
		else
		{
			PressedKey1p[9]=TRUE;
			ArrowState1p[9]=PRESS;
		}
	}
	else
	{
		ArrowState1p[9]=NORMAL;
		PressedKey1p[9]=FALSE;
	}

	if(bJoy==TRUE)
	{
		ReadJoystickInput2();

		if(PressedKey1p[1] || PressedKey_Joy[1])PressedKey1p[1]=TRUE;
		if(PressedKey1p[3] || PressedKey_Joy[3])PressedKey1p[3]=TRUE;
		if(PressedKey1p[5] || PressedKey_Joy[5])PressedKey1p[5]=TRUE;
		if(PressedKey1p[7] || PressedKey_Joy[7])PressedKey1p[7]=TRUE;
		if(PressedKey1p[9] || PressedKey_Joy[9])PressedKey1p[9]=TRUE;
	}
}

/*--------------------------------------------------------------------------
| ReadFusionInput
|
| Requests keyboard data and performs any needed processing.
*-------------------------------------------------------------------------*/
void ReadFusionInput(void)
{
	char	rgKeyData[KEYBUFSIZE];
   DWORD                   dwEvents;
//   DWORD                   dw;
   static DWORD            dwKeyState = 0;
   HRESULT                 hRes;

   // get data from the keyboard
   dwEvents = KEYBUFSIZE;

   hRes=g_pdevCurrent->GetDeviceState(sizeof(rgKeyData),(LPVOID)&rgKeyData);

   if(hRes != DI_OK)
   {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if(hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return ;
   }

   // 2player
	if(rgKeyData[DIK_NUMPAD7] & 0x80)
	{
		if(ArrowState2p[1]==PRESS)
		{
			PressedKey2p[1]=FALSE;
		}
		else
		{
			PressedKey2p[1]=TRUE;
			ArrowState2p[1]=PRESS;
		}

	}
	else
	{
		ArrowState2p[1]=NORMAL;
		PressedKey2p[1]=FALSE;
	}
	

	if(rgKeyData[DIK_NUMPAD9] & 0x80)
	{
		if(ArrowState2p[3]==PRESS)
		{
			PressedKey2p[3]=FALSE;
		}
		else
		{
			PressedKey2p[3]=TRUE;
			ArrowState2p[3]=PRESS;
		}
	}
	else
	{
		ArrowState2p[3]=NORMAL;
		PressedKey2p[3]=FALSE;
	}


	if(rgKeyData[DIK_NUMPAD5] & 0x80)
	{
		if(ArrowState2p[5]==PRESS)
		{
			PressedKey2p[5]=FALSE;
		}
		else
		{
			PressedKey2p[5]=TRUE;
			ArrowState2p[5]=PRESS;
		}
	}
	else
	{
		ArrowState2p[5]=NORMAL;
		PressedKey2p[5]=FALSE;
	}

	if(rgKeyData[DIK_NUMPAD1] & 0x80)
	{
		if(ArrowState2p[7]==PRESS)
		{
			PressedKey2p[7]=FALSE;
		}
		else
		{
			PressedKey2p[7]=TRUE;
			ArrowState2p[7]=PRESS;
		}
	}
	else
	{
		ArrowState2p[7]=NORMAL;
		PressedKey2p[7]=FALSE;
	}

	if(rgKeyData[DIK_NUMPAD3] & 0x80)
	{
		if(ArrowState2p[9]==PRESS)
		{
			PressedKey2p[9]=FALSE;
		}
		else
		{
			PressedKey2p[9]=TRUE;
			ArrowState2p[9]=PRESS;
		}
	}
	else
	{
		ArrowState2p[9]=NORMAL;
		PressedKey2p[9]=FALSE;
	}

// 1 player
	if(rgKeyData[DIK_Q] & 0x80)
	{
		if(ArrowState1p[1]==PRESS)
		{
			PressedKey1p[1]=FALSE;
		}
		else
		{
			PressedKey1p[1]=TRUE;
			ArrowState1p[1]=PRESS;
		}

	}
	else
	{
		ArrowState1p[1]=NORMAL;
		PressedKey1p[1]=FALSE;
	}
	

	if(rgKeyData[DIK_E] & 0x80)
	{
		if(ArrowState1p[3]==PRESS)
		{
			PressedKey1p[3]=FALSE;
		}
		else
		{
			PressedKey1p[3]=TRUE;
			ArrowState1p[3]=PRESS;
		}
	}
	else
	{
		ArrowState1p[3]=NORMAL;
		PressedKey1p[3]=FALSE;
	}


	if(rgKeyData[DIK_S] & 0x80)
	{
		if(ArrowState1p[5]==PRESS)
		{
			PressedKey1p[5]=FALSE;
		}
		else
		{
			PressedKey1p[5]=TRUE;
			ArrowState1p[5]=PRESS;
		}
	}
	else
	{
		ArrowState1p[5]=NORMAL;
		PressedKey1p[5]=FALSE;
	}

	if(rgKeyData[DIK_Z] & 0x80)
	{
		if(ArrowState1p[7]==PRESS)
		{
			PressedKey1p[7]=FALSE;
		}
		else
		{
			PressedKey1p[7]=TRUE;
			ArrowState1p[7]=PRESS;
		}
	}
	else
	{
		ArrowState1p[7]=NORMAL;
		PressedKey1p[7]=FALSE;
	}

	if(rgKeyData[DIK_C] & 0x80)
	{
		if(ArrowState1p[9]==PRESS)
		{
			PressedKey1p[9]=FALSE;
		}
		else
		{
			PressedKey1p[9]=TRUE;
			ArrowState1p[9]=PRESS;
		}
	}
	else
	{
		ArrowState1p[9]=NORMAL;
		PressedKey1p[9]=FALSE;
	}

	if(bJoy==TRUE)
	{
		ReadJoystickInput2();

		if(PressedKey1p[1] || PressedKey_Joy[1])PressedKey1p[1]=TRUE;
	//	else if(PressedKey1p[1]==FALSE && PressedKey_Joy[1]==FALSE)PressedKey1p[1]=FALSE;
		if(PressedKey1p[3] || PressedKey_Joy[3])PressedKey1p[3]=TRUE;
	//	else if(PressedKey1p[3]==FALSE && PressedKey_Joy[3]==FALSE)PressedKey1p[3]=FALSE;
		if(PressedKey1p[5] || PressedKey_Joy[5])PressedKey1p[5]=TRUE;
	//	else if(PressedKey1p[5]==FALSE && PressedKey_Joy[5]==FALSE)PressedKey1p[5]=FALSE;
		if(PressedKey1p[7] || PressedKey_Joy[7])PressedKey1p[7]=TRUE;
	//	else if(PressedKey1p[7]==FALSE && PressedKey_Joy[7]==FALSE)PressedKey1p[7]=FALSE;
		if(PressedKey1p[9] || PressedKey_Joy[9])PressedKey1p[9]=TRUE;
	//	else if(PressedKey1p[9]==FALSE && PressedKey_Joy[9]==FALSE)PressedKey1p[9]=FALSE;
	}
}

/*--------------------------------------------------------------------------
| ReadJoystickInput
|
| Requests joystick data and performs any needed processing.
|
*-------------------------------------------------------------------------*/
void ReadJoystickInput(void)
{
   DWORD                   dwKeyState;
   HRESULT                 hRes;
   DIJOYSTATE              js;

   // poll the joystick to read the current state
   hRes = IDirectInputDevice2_Poll(g_pdevCurrent);

   // get data from the joystick
   hRes = IDirectInputDevice_GetDeviceState(g_pdevCurrent,
                                            sizeof(DIJOYSTATE), &js);

   if(hRes != DI_OK)
   {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if(hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return ;
   }

   //
   // Now study the position of the stick and the buttons.
   //

   dwKeyState = 0;

// 1 player
	if(js.rgbButtons[0] & 0x80)
	{
		if(ArrowState_Joy[1]==PRESS)
		{
			PressedKey_Joy[1]=FALSE;
		}
		else
		{
			PressedKey_Joy[1]=TRUE;
			ArrowState_Joy[1]=PRESS;
		}

	}
	else
	{
		ArrowState_Joy[1]=NORMAL;
		PressedKey_Joy[1]=FALSE;
	}
	

	if(js.rgbButtons[1] & 0x80)
	{
		if(ArrowState_Joy[3]==PRESS)
		{
			PressedKey_Joy[3]=FALSE;
//			return;
		}
		else
		{
			PressedKey_Joy[3]=TRUE;
			ArrowState_Joy[3]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[3]=NORMAL;
		PressedKey_Joy[3]=FALSE;
	}


/*	if(js.rgbButtons[14] & 0x80)
	{
		if(ArrowState_Joy[5]==PRESS)
		{
			PressedKey_Joy[5]=FALSE;
		}
		else
		{
			PressedKey_Joy[5]=TRUE;
			ArrowState_Joy[5]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[5]=NORMAL;
		PressedKey_Joy[5]=FALSE;
	}
*/
	if(js.rgbButtons[12] & 0x80)
	{
		if(ArrowState_Joy[5]==PRESS)
		{
			PressedKey_Joy[5]=FALSE;
		}
		else
		{
			PressedKey_Joy[5]=TRUE;
			ArrowState_Joy[5]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[5]=NORMAL;
		PressedKey_Joy[5]=FALSE;
	}

	if(js.rgbButtons[2] & 0x80)
	{
		if(ArrowState_Joy[7]==PRESS)
		{
			PressedKey_Joy[7]=FALSE;
		}
		else
		{
			PressedKey_Joy[7]=TRUE;
			ArrowState_Joy[7]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[7]=NORMAL;
		PressedKey_Joy[7]=FALSE;
	}

	if(js.rgbButtons[3] & 0x80)
	{
		if(ArrowState_Joy[9]==PRESS)
		{
			PressedKey_Joy[9]=FALSE;
		}
		else
		{
			PressedKey_Joy[9]=TRUE;
			ArrowState_Joy[9]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[9]=NORMAL;
		PressedKey_Joy[9]=FALSE;
	}
}

/*--------------------------------------------------------------------------
| PickInputDevice
|
| Make the n'th input device the one that we will use for game play.
|
*-------------------------------------------------------------------------*/

BOOL PickInputDevice(int n)
{
    if (n < g_cpdevFound) {

        /*
         *  Unacquire the old device.
         */
        if (g_pdevCurrent) {
            IDirectInputDevice_Unacquire(g_pdevCurrent);
        }

        /*
         *  Move to the new device.
         */
        g_pdevCurrent = g_rgpdevFound[n];

        /*
         *  Set ReadGameInput to the appropriate handler.
         */
        if (n == 0) {
            ReadGameInput = ReadKeyboardInput;
        } else {
            ReadGameInput = ReadJoystickInput;
        }

        CheckMenuRadioItem(GetSubMenu(GetMenu(hWnd), 0),
                           IDC_DEVICES, IDC_DEVICES + g_cpdevFound - 1,
                           IDC_DEVICES + n, MF_BYCOMMAND);

        ReacquireInput();

        return TRUE;
    } else {
        return FALSE;
    }
}
////////////////////////////////////////////
void ReadDDRInput(void)
{
   HRESULT                 hRes;
   DIJOYSTATE              js;

   // poll the joystick to read the current state
   hRes = IDirectInputDevice2_Poll(g_pdevJoystick);

   // get data from the joystick
   hRes = IDirectInputDevice_GetDeviceState(g_pdevJoystick,
                                            sizeof(DIJOYSTATE), &js);

   if(hRes != DI_OK)
   {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if(hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return ;
   }

   //
   // Now study the position of the stick and the buttons.
   //

	if(js.rgbButtons[0] & 0x80)
	{
		if(ArrowState_Joy[1]==PRESS)
		{
			PressedKey_Joy[1]=FALSE;
		}
		else
		{
			PressedKey_Joy[1]=TRUE;
			ArrowState_Joy[1]=PRESS;
		}

	}
	else
	{
		ArrowState_Joy[1]=NORMAL;
		PressedKey_Joy[1]=FALSE;
	}
	

	if(js.rgbButtons[1] & 0x80)
	{
		if(ArrowState_Joy[3]==PRESS)
		{
			PressedKey_Joy[3]=FALSE;
//			return;
		}
		else
		{
			PressedKey_Joy[3]=TRUE;
			ArrowState_Joy[3]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[3]=NORMAL;
		PressedKey_Joy[3]=FALSE;
	}


/*	if(js.rgbButtons[14] & 0x80)
	{
		if(ArrowState_Joy[5]==PRESS)
		{
			PressedKey_Joy[5]=FALSE;
		}
		else
		{
			PressedKey_Joy[5]=TRUE;
			ArrowState_Joy[5]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[5]=NORMAL;
		PressedKey_Joy[5]=FALSE;
	}
*/
	if(js.rgbButtons[12] & 0x80)
	{
		if(ArrowState_Joy[5]==PRESS)
		{
			PressedKey_Joy[5]=FALSE;
		}
		else
		{
			PressedKey_Joy[5]=TRUE;
			ArrowState_Joy[5]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[5]=NORMAL;
		PressedKey_Joy[5]=FALSE;
	}

	if(js.rgbButtons[2] & 0x80)
	{
		if(ArrowState_Joy[7]==PRESS)
		{
			PressedKey_Joy[7]=FALSE;
		}
		else
		{
			PressedKey_Joy[7]=TRUE;
			ArrowState_Joy[7]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[7]=NORMAL;
		PressedKey_Joy[7]=FALSE;
	}

	if(js.rgbButtons[3] & 0x80)
	{
		if(ArrowState_Joy[9]==PRESS)
		{
			PressedKey_Joy[9]=FALSE;
		}
		else
		{
			PressedKey_Joy[9]=TRUE;
			ArrowState_Joy[9]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[9]=NORMAL;
		PressedKey_Joy[9]=FALSE;
	}

}

void ReadKoinsInput(void)
{
   HRESULT                 hRes;
   DIJOYSTATE              js;

   static	int start;
   long x;

   // poll the joystick to read the current state
   hRes = IDirectInputDevice2_Poll(g_pdevJoystick);

   // get data from the joystick
   hRes = IDirectInputDevice_GetDeviceState(g_pdevJoystick,
                                            sizeof(DIJOYSTATE), &js);

   if(start==0)
   {
	   start++;
	   x=js.lX;
   }

   if(hRes != DI_OK)
   {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if(hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return ;
   }

	if (js.lX-x < 100 /*|| js.lX-x > 1000*/)
	{
		if(ArrowState_Joy[5]==PRESS)
		{
			PressedKey_Joy[5]=FALSE;
		}
		else
		{
			PressedKey_Joy[5]=TRUE;
			ArrowState_Joy[5]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[5]=NORMAL;
		PressedKey_Joy[5]=FALSE;
	}


   if (js.rgbButtons[0] & 0x80)
	{
		if(ArrowState_Joy[7]==PRESS)
		{
			PressedKey_Joy[7]=FALSE;
//			return;
		}
		else
		{
			PressedKey_Joy[7]=TRUE;
			ArrowState_Joy[7]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[7]=NORMAL;
		PressedKey_Joy[7]=FALSE;
	}

   if (js.rgbButtons[1] & 0x80) 
	{
		if(ArrowState_Joy[1]==PRESS)
		{
			PressedKey_Joy[1]=FALSE;
		}
		else
		{
			PressedKey_Joy[1]=TRUE;
			ArrowState_Joy[1]=PRESS;
		}

	}
	else
	{
		ArrowState_Joy[1]=NORMAL;
		PressedKey_Joy[1]=FALSE;
	}
   if (js.rgbButtons[2] & 0x80)
	{
		if(ArrowState_Joy[9]==PRESS)
		{
			PressedKey_Joy[9]=FALSE;
//			return;
		}
		else
		{
			PressedKey_Joy[9]=TRUE;
			ArrowState_Joy[9]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[9]=NORMAL;
		PressedKey_Joy[9]=FALSE;
	}

   if (js.rgbButtons[3] & 0x80)
	{
		if(ArrowState_Joy[3]==PRESS)
		{
			PressedKey_Joy[3]=FALSE;
//			return;
		}
		else
		{
			PressedKey_Joy[3]=TRUE;
			ArrowState_Joy[3]=PRESS;
		}
	}
	else
	{
		ArrowState_Joy[3]=NORMAL;
		PressedKey_Joy[3]=FALSE;
	}

}

BOOL ReacquireInput2(void)
{
    HRESULT hRes;

    // if we have a current device
    if(g_pdevJoystick)
    {
       // acquire the device
       hRes = IDirectInputDevice_Acquire(g_pdevJoystick);
       if(SUCCEEDED(hRes))
       {
          // acquisition successful
          return TRUE;
       }
       else
       {
          // acquisition failed
          return FALSE;
       }
    }
    else
    {
       // we don't have a current device
       return FALSE;
    }

}

BOOL PickInputDevice2(int n)
{
    if (n < g_cpdevFound) {

        /*
         *  Unacquire the old device.
         */
        if (g_pdevJoystick) {
            IDirectInputDevice_Unacquire(g_pdevJoystick);
        }

        /*
         *  Move to the new device.
         */
        g_pdevJoystick = g_rgpdevFound[n];

        /*
         *  Set ReadGameInput to the appropriate handler.
         */
/*        if (n == 0) {
            ReadGameInput = ReadKeyboardInput;
        } else {
            ReadGameInput = ReadJoystickInput;
        }*/

        ReacquireInput2();

        return TRUE;
    } else {
        return FALSE;
    }
}
