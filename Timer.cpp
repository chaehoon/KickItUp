#include "Timer.h"

Timer::Timer() :	mStartTicks( 0 ), mPausedTicks( 0 ), mPaused( false ), mStarted( false )
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();    
}

void Timer::Stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;    
}

void Timer::Pause()
{
	//If the timer is running and isn't already mPaused
	if( ( mStarted == true ) && ( mPaused == false ) ) {
		//Pause the timer
		mPaused = true;

		//Calculate the mPaused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
	}
}

void Timer::Unpause()
{
	//If the timer is mPaused
	if( mPaused == true ) {
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the mPaused ticks
		mPausedTicks = 0;
	}
}

Uint32 Timer::GetTicks()
{
	//If the timer is running
	if( mStarted == true ) {
		//If the timer is mPaused
		if( mPaused == true ) {
			//Return the number of ticks when the timer was mPaused
			return mPausedTicks;
		} else {
			//Return the current time minus the start time
			return SDL_GetTicks() - mStartTicks;
		}    
	}

	//If the timer isn't running
	return 0;    
}

bool Timer::IsStarted()
{
	return mStarted;    
}

bool Timer::IsPaused()
{
	return mPaused;    
}
