/*
 * KeyboardDevice.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "iInputDevice.h"

#ifndef KEYBOARDDEVICE_H_
#define KEYBOARDDEVICE_H_

class KeyboardDevice :	public iInputDevice {
public:
	virtual ~KeyboardDevice(void)	{}

	virtual void	Reset()	{}
	virtual void	Update( unsigned long delta )	{}
	virtual int		GetState( const eKind kind, const uKey key );
};

#endif /* KEYBOARDDEVICE_H_ */
