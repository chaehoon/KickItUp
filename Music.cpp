#include "Music.h"

extern FMOD_SYSTEM  *	gpSystem;

Music::Music()  : m_pSystem( 0 ),
	m_pSound( 0 ),
	m_pChannel( 0 )
{
}

Music::~Music(void)
{
	Free();
}

bool    Music::Load( const std::string & musicFileName )
{
	m_pSystem = gpSystem;
	if( IsLoaded() )
		Free();

	FMOD_RESULT ret = FMOD_System_CreateStream(m_pSystem,
			musicFileName.c_str(),
			FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL,
			0, &m_pSound);

	/*
	FMOD_RESULT ret = m_pSystem->createStream(
			musicFileName.c_str(),
			FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL,
			0, &m_pSound);
	*/

	return ret == FMOD_OK;
}

void    Music::Free()
{
	if( IsLoaded() ) {
		FMOD_Sound_Release(m_pSound);
		// m_pSound->release();
		m_pSound = 0;
		m_pChannel = 0;
	}
}

bool    Music::Play( const bool loop /* = false */ )
{
	if( IsLoaded() ) {
		FMOD_Sound_SetMode(m_pSound, loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
		FMOD_RESULT ret = FMOD_System_PlaySound(m_pSystem,
				FMOD_CHANNEL_FREE,
				m_pSound,
				0, &m_pChannel);

		/*
		m_pSound->setMode( loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF );
		FMOD_RESULT ret = m_pSystem->playSound(
				FMOD_CHANNEL_FREE,
				m_pSound,
				false, &m_pChannel);
		*/
		return ret == FMOD_OK;
	}
	return false;	
}

void    Music::Resume()    {
	if( IsPaused() ) {
		FMOD_Channel_SetPaused(m_pChannel, 0);
		// m_pChannel->setPaused( false );
	}
}

void    Music::Pause()     {
	FMOD_Channel_SetPaused(m_pChannel, 1);
	// m_pChannel->setPaused( true );
}

void Music::Halt()
{
	if( m_pChannel )
		FMOD_Channel_Stop(m_pChannel);
		// m_pChannel->stop();
}

unsigned int  Music::GetCurrentPosition()
{
	unsigned int ms;
	FMOD_RESULT	ret = FMOD_Channel_GetPosition(m_pChannel, &ms, FMOD_TIMEUNIT_MS);
	/*
	FMOD_RESULT	ret;
	ret = m_pChannel->getPosition( &ms, FMOD_TIMEUNIT_MS );
	*/
	if( ret != FMOD_OK )
		return -1;

	return ms;
}
