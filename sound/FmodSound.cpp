/*
 * FmodSound.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "FmodSound.h"
#include "../Util/File.h"
#include <cassert>

FmodSound::FmodSound( FMOD_SYSTEM * pSystem)  : m_pSystem( pSystem ),
	m_pSound( 0 ),
	m_pChannel( 0 )
{
}

FmodSound::~FmodSound(void)
{
	Free();
}

bool    FmodSound::Load( const std::string & musicFileName )
{
	if( IsLoaded() )
		Free();

	FMOD_RESULT ret;
	ret = FMOD_System_CreateStream(m_pSystem, musicFileName.c_str(), FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_pSound);

	assert( ret == FMOD_OK );

	return ret == FMOD_OK;
}

void    FmodSound::Free()
{
	if( IsLoaded() ) {
		FMOD_Sound_Release(m_pSound);
		m_pSound = 0;
		m_pChannel = 0;
	}
}

bool    FmodSound::Play( const bool loop /* = false */ )
{
	if( IsLoaded() ) {
		if(FMOD_Sound_SetMode(m_pSound, loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF ) == FMOD_OK) {
			if(FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, m_pSound, false, &m_pChannel) == FMOD_OK) {
				return true;
			}
		}
	}
	return false;
}

void    FmodSound::Resume()    {
	if( IsPaused() ) {
		FMOD_Channel_SetPaused( m_pChannel, false);
	}
}

void    FmodSound::Pause()     {
	FMOD_Channel_SetPaused(m_pChannel, true);
}

void FmodSound::Stop()
{
	if( m_pChannel )
		FMOD_Channel_Stop(m_pChannel);
}

unsigned int  FmodSound::GetPosition()
{
	unsigned int ms;
	FMOD_RESULT	ret;
	ret = FMOD_Channel_GetPosition(m_pChannel, &ms, FMOD_TIMEUNIT_MS );
	if( ret != FMOD_OK )
		return -1;

	return ms;
}
