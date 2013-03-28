/*
 * FmodSoundStore.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "FmodSoundStore.h"
#include "FmodSound.h"
#include <cassert>
#include <fmodex/fmod_errors.h>

FmodSoundStore::FmodSoundStore(void)	:	m_system( 0 ) {
}

FmodSoundStore::~FmodSoundStore(void)
{
}

/// @brief	fmod initialize.
bool	FmodSoundStore::Initialize()
{
	// Create a System object and initialize.
	if( _isErr( FMOD_System_Create(&m_system) ) )
		return false;

	unsigned int      version;
	if( _isErr( FMOD_System_GetVersion(m_system, &version) ) )
		return false;

	if (version < FMOD_VERSION)	{
		// ("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return false;
	}

	if( _isErr( FMOD_System_Init(m_system, 32, FMOD_INIT_NORMAL, 0) ) )
		return false;

	return true;
}

void	FmodSoundStore::Destroy()
{
	SoundStore::Destroy();

	if( m_system == 0 )
		return;

	if( _isErr(FMOD_System_Close(m_system)) )
		return;

	if( _isErr(FMOD_System_Release(m_system)) )
		return;

	m_system = 0;
}

bool	FmodSoundStore::Process( unsigned long )
{
	FMOD_System_Update(m_system);
	return true;
}

/// @brief	create fmod sound object
Sound *	FmodSoundStore::create()
{
	Sound * pSnd = new FmodSound( m_system );
	return pSnd;
}

/// @brief	fmod error check.
bool FmodSoundStore::_isErr(const FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		char	buff[1024];
		sprintf( buff, "%s", FMOD_ErrorString(result) );
		assert( 0 );
		return true;
	}
	return false;
}
