#ifndef _KICKITUP_SOUND_H
#define _KICKITUP_SOUND_H

#include <string>

class Sound
{
public:
	Sound(void) {}
	virtual ~Sound(void)    {}

	virtual bool    Load( const std::string & musicFileName ) = 0;
	virtual void    Free() = 0;

	virtual bool    IsLoaded() const = 0;
	virtual bool    Play( const bool loop = false ) = 0;
	virtual void    Stop()	= 0;

	virtual bool    IsPlaying() {   return false;   }
	virtual bool    IsPaused()  {   return false;   }
	virtual void    Resume()    {}
	virtual void    Pause()     {}

	virtual unsigned int  GetPosition()	{	return 0;	}
};

#endif // _SOUND_H
