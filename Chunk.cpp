#include "Chunk.h"

extern FMOD::System *	gpSystem;

Chunk::Chunk()  : m_pSystem( 0 ),
	m_pSound( 0 ),
	m_pChannel( 0 )
{
}

Chunk::~Chunk(void)
{
	Free();
}

bool    Chunk::Load( const std::string & musicFileName )
{
	m_pSystem = gpSystem;
	if( IsLoaded() )
		Free();

	FMOD_RESULT ret = m_pSystem->createStream(
			musicFileName.c_str(),
			FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL,
			0,
			&m_pSound);

	return ret == FMOD_OK;
}

void    Chunk::Free()
{
	if( IsLoaded() ) {
		m_pSound->release();
		m_pSound = 0;
		m_pChannel = 0;
	}
}

bool    Chunk::Play( const bool loop /* = false */ )
{
	if( IsLoaded() ) {
		m_pSound->setMode( loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF );
		FMOD_RESULT ret = m_pSystem->playSound(
				FMOD_CHANNEL_FREE,
				m_pSound,
				false,
				&m_pChannel);
		return ret == FMOD_OK;
	}
	return false;	
}

void Chunk::Halt()
{
	if( m_pChannel )
		m_pChannel->stop();
}

