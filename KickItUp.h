#ifndef _KICKITUP_KICKITUP_H
#define _KICKITUP_KICKITUP_H

#include "common.h"

class SoundStore;
class SurfaceStore;
class Context;
class Surface;

/// @file	KickItUp.h
/// @brief	KickItUp class.
class KickItUp {
private:
	Context *		m_pContext;
	bool			m_bQuit;

public:
	KickItUp(void);
	~KickItUp(void);

	bool Initialize();		///< Initialize KickItUp.
	bool Run();				///< KickItUp main routine.
	void Destroy();			///< Destroy KickItUp.

	bool Process( unsigned long delta );

	void SetQuit( const bool bQuit = true )	{	m_bQuit = bQuit;	}

private:
    bool _loadImage();
    bool _loadSound();
};

#endif // _KICKITUP_KICKITUP_H
