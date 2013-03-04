#ifndef _KICKITUP_MUSIC_H
#define _KICKITUP_MUSIC_H

#include "Sound.h"
#include <fmodex/fmod.h>

class Music :   public Sound
{
private:
	FMOD_SYSTEM  *	m_pSystem;
	FMOD_SOUND  *	m_pSound;
	FMOD_CHANNEL  *	m_pChannel;

public:
	Music();
	virtual ~Music(void);

	virtual bool    Load( const std::string & musicFileName );
	virtual void    Free();

	virtual bool    IsLoaded() const    {   return m_pSound != 0;   }
	virtual bool    IsPlaying() {
		FMOD_BOOL isplaying;
		FMOD_Channel_IsPlaying(m_pChannel, &isplaying);
		return isplaying != 0;

		/*
		bool	playing;
		m_pChannel->isPlaying( &playing );
		return playing;
		*/
	}

	virtual bool    IsPaused()  {
		FMOD_BOOL paused;
		FMOD_Channel_GetPaused(m_pChannel, &paused);
		return paused != 0;

		/*
		bool paused;
		m_pChannel->getPaused(&paused);
		return paused;
		*/
	}

	virtual bool    Play( const bool loop = false );
	virtual void    Resume();
	virtual void    Pause();
	virtual void    Halt();

	virtual unsigned int  GetCurrentPosition();
};

#endif // _MUSIC_H
