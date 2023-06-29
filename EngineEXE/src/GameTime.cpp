#include "pch.h"

#include "GameTime.h"
#include <windows.h>
#include <random>

GameTimer::GameTimer()
	:m_SecondsPerCount(0.0), m_DeltaTime(-1.0), m_BaseTime(0), 
	m_PausedTime(0), m_PrevTime(0), m_CurrTime(0), m_startTime(0), m_Paused(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

float GameTimer::GetGameTime() const
{
	// m_BaseTime includes all time including paused time which we dont want. Thus we need to 
	// subtract total time paused from the time that we paused the application and then subtract
	// the total time which will leave us with the total time application has been running.
	if (m_Paused)
	{
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	// if the game is not paused we can subtract paused time from the current time and then subtract
	// base time to be left with the total time application has been running.
	else
	{
		return (float)(((m_CurrTime - m_startTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	} 
}

float GameTimer::GetDeltaTime() const
{
	return float(m_DeltaTime);
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Paused = false;
}

void GameTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_PrevTime);
	srand(m_startTime);

	// Check if we are starting from a paused state
	if (m_Paused)
	{
		// Calcualte paused time by subtracting the time stopped from the time the application started
		m_PausedTime += (m_startTime - m_StopTime);

		// Since we are restaring the timer reset the prev time to current times
		m_PrevTime = m_startTime;

		// Unpause the game
		m_StopTime = 0;
		m_Paused = false;
		ShowCursor(false);
	}
}

void GameTimer::Pause()
{
	// If we are alreadyt paused, then dont do anything
	if (!m_Paused)
	{
		// Get the current time
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		// Pause the timer and count the time paused
		m_StopTime = currTime;
		m_Paused = true;
		ShowCursor(true);
	}
}

void GameTimer::Tick()
{
	// if the application is paused delta time will be 0 as no frames are processed
	if(m_Paused)
	{
		m_DeltaTime = 0.0;
		return;
	}

	// Get the time this frame.
	__int64 currTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;

	// Time difference between this frame and the previous.
	m_DeltaTime = (m_CurrTime - m_PrevTime)*m_SecondsPerCount;

	// Prepare for next frame.
	m_PrevTime = m_CurrTime;

	// Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if(m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}
	if (m_DeltaTime > 0.1)
	{
		m_DeltaTime = 0.0f;
	}
}
