#ifndef _KICKITUP_TIMER_H_
#define _KICKITUP_TIMER_H_

#include <SDL/SDL.h>

class Timer
{
private:
	Uint32	mStartTicks;	// The clock time when the timer started
	Uint32	mPausedTicks;	// The ticks stored when the timer was paused
	bool	mPaused;
	bool	mStarted;

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

#endif // TIMER_H_
