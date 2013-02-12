#ifndef _KICKITUP_CHUNK_H
#define _KICKITUP_CHUNK_H

#include "Sound.h"
#include <fmodex/fmod.hpp>

class Chunk : public Sound
{
private:
	FMOD::System *	m_pSystem;
	FMOD::Sound *	m_pSound;	
	FMOD::Channel *	m_pChannel;

public:
	Chunk();
	virtual ~Chunk(void);

	virtual bool    Load( const std::string & musicFileName );
	virtual void    Free();

	virtual bool    IsLoaded() const    {   return m_pSound != 0;   }

	virtual bool    Play( const bool loop = false );
	virtual void    Halt();

};

#endif // _CHUNK_H
