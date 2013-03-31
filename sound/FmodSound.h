/*
 * FmodSound.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef FMODSOUND_H_
#define FMODSOUND_H_

#include <string>
#include <fmodex/fmod.h>
#include "Sound.h"

class FmodSound :	public	Sound {
private:
	FMOD_SYSTEM  *	m_pSystem;
	FMOD_SOUND  *	m_pSound;
	FMOD_CHANNEL  *	m_pChannel;

public:
	FmodSound( FMOD_SYSTEM * pSystem);
	virtual ~FmodSound(void);

	virtual bool    Load( const std::string & musicFileName );
	virtual void    Free();

	virtual bool    IsLoaded() const    {   return m_pSound != 0;   }
	virtual bool    IsPlaying() {
		// TODO: bugfix
		int	playing;
		FMOD_Channel_IsPlaying(m_pChannel, &playing);
		return playing != 0;
	}

	virtual bool    IsPaused()  {
		int paused;
		FMOD_Channel_GetPaused(m_pChannel, &paused);
		return paused != 0;
	}

	virtual bool    Play( const bool loop = false );
	virtual void    Resume();
	virtual void    Pause();
	virtual void    Stop();

	virtual unsigned int  GetPosition();
};

#endif /* FMODSOUND_H_ */
