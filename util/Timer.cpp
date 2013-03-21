#include "Timer.h"

Timer::Timer() :	m_StartTicks( 0 ),
	m_PausedTicks( 0 ),
	m_Paused( false ),
	m_Started( false )
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
	//Start the timer
	m_Started = true;

	//Unpause the timer
	m_Paused = false;

	//Get the current clock time
	m_StartTicks = SDL_GetTicks();    
}

void Timer::Stop()
{
	//Stop the timer
	m_Started = false;

	//Unpause the timer
	m_Paused = false;    
}

void Timer::Pause()
{
	//If the timer is running and isn't already m_Paused
	if( ( m_Started == true ) && ( m_Paused == false ) )
	{
		//Pause the timer
		m_Paused = true;

		//Calculate the m_Paused ticks
		m_PausedTicks = SDL_GetTicks() - m_StartTicks;
	}
}

void Timer::Unpause()
{
	//If the timer is m_Paused
	if( m_Paused == true )
	{
		//Unpause the timer
		m_Paused = false;

		//Reset the starting ticks
		m_StartTicks = SDL_GetTicks() - m_PausedTicks;

		//Reset the m_Paused ticks
		m_PausedTicks = 0;
	}
}

Uint32 Timer::GetTicks()
{
	//If the timer is running
	if( m_Started == true )
	{
		//If the timer is m_Paused
		if( m_Paused == true )
		{
			//Return the number of ticks when the timer was m_Paused
			return m_PausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			return SDL_GetTicks() - m_StartTicks;
		}    
	}

	//If the timer isn't running
	return 0;    
}

bool Timer::IsStarted()
{
	return m_Started;    
}

bool Timer::IsPaused()
{
	return m_Paused;    
}
