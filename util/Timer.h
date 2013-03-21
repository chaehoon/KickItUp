#ifndef _KICKITUP_TIMER_H
#define _KICKITUP_TIMER_H

#include "../common.h"
#include <SDL/SDL.h>

class Timer
{
private:
	Uint32	m_StartTicks;	// The clock time when the timer started
	Uint32	m_PausedTicks;	// The ticks stored when the timer was paused
	bool	m_Paused;
	bool	m_Started;

public:
	Timer(void);    //Initializes variables
	~Timer(void);

	//The various clock actions
	void Start();
	void Stop();
	void Pause();
	void Unpause();

	//Gets the timer's time
	Uint32	GetTicks();

	//Checks the status of the timer
	bool IsStarted();
	bool IsPaused();
};

#endif // _KICKITUP_TIMER_H
