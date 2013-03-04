/*
 * KeyboardDevice.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "KeyboardDevice.h"
#include <cassert>
#include <SDL/SDL.h>

int		KeyboardDevice::GetState( const eKind kind, const uKey key )
{
	int	ret = 0;

	switch( kind ) {
		case KeyState:{
			Uint8 * keystates = SDL_GetKeyState( NULL );
			if( keystates )
				ret = static_cast<int>( keystates[key.key] );
			break;
		}
		default:
			assert( 0 );
			break;
	}
	return ret;
}
