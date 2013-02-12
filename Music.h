#ifndef _KICKITUP_MUSIC_H
#define _KICKITUP_MUSIC_H

#include "Sound.h"
#include <fmodex/fmod.hpp>

class Music :   public Sound
{
private:
	FMOD::System *	m_pSystem;
	FMOD::Sound *	m_pSound;	
	FMOD::Channel *	m_pChannel;

public:
	Music();
	virtual ~Music(void);

	virtual bool    Load( const std::string & musicFileName );
	virtual void    Free();

	virtual bool    IsLoaded() const    {   return m_pSound != 0;   }
	virtual bool    IsPlaying() {
		bool	playing;
		m_pChannel->isPlaying( &playing );
		return playing;
	}

	virtual bool    IsPaused()  {
		bool paused;
		m_pChannel->getPaused(&paused);
		return paused;
	}

	virtual bool    Play( const bool loop = false );
	virtual void    Resume();
	virtual void    Pause();
	virtual void    Halt();

	virtual unsigned int  GetCurrentPosition();
};

#endif // _MUSIC_H
