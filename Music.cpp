#include "Music.h"

extern FMOD::System *	gpSystem;

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

	FMOD_RESULT ret = m_pSystem->createStream(
			musicFileName.c_str(),
			FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL,
			0, &m_pSound);

	return ret == FMOD_OK;
}

void    Music::Free()
{
	if( IsLoaded() ) {
		m_pSound->release();
		m_pSound = 0;
		m_pChannel = 0;
	}
}

bool    Music::Play( const bool loop /* = false */ )
{
	if( IsLoaded() ) {
		m_pSound->setMode( loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF );
		FMOD_RESULT ret = m_pSystem->playSound(
				FMOD_CHANNEL_FREE,
				m_pSound,
				false, &m_pChannel);
		return ret == FMOD_OK;
	}
	return false;	
}

void    Music::Resume()    {
	if( IsPaused() ) {
		m_pChannel->setPaused( false );
	}
}

void    Music::Pause()     {
	m_pChannel->setPaused( true );
}

void Music::Halt()
{
	if( m_pChannel )
		m_pChannel->stop();
}

unsigned int  Music::GetCurrentPosition()
{
	unsigned int ms;
	FMOD_RESULT	ret;
	ret = m_pChannel->getPosition( &ms, FMOD_TIMEUNIT_MS );
	if( ret != FMOD_OK )
		return -1;

	return ms;
}
