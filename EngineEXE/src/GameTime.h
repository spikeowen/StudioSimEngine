#pragma once
class GameTimer
{
public:
	GameTimer();

	// Get the current GameTime in seconds
	float GetGameTime()const;
	//Get the DeltaTime in seconds
	float GetDeltaTime()const;

	void Reset();
	void Start();
	void Pause();
	void Tick();

	inline bool GetPauseState() { return m_Paused; }
private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;
	__int64 m_startTime;

	bool m_Paused;
};

