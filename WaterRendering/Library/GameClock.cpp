#include "GameClock.h"

using namespace std;

namespace Library
{
	GameClock::GameClock() : startTime(), currentTime(), lastTime(), frequency()
	{
		frequency = GetFrequency();
		Reset();
	}

	const LARGE_INTEGER& GameClock::StartTime() const
	{
		return startTime;
	}

	const LARGE_INTEGER& GameClock::CurrentTime() const
	{
		return currentTime;
	}

	const LARGE_INTEGER& GameClock::LastTime() const
	{
		return lastTime;
	}

	void GameClock::Reset()
	{
		GetTime(startTime);
		currentTime = startTime;
		lastTime = currentTime;
	}

	double GameClock::GetFrequency() const
	{
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
		{
			throw exception("QueryPerformanceFrequency failed.");
		}
		return (double)frequency.QuadPart;
	}

	void GameClock::GetTime(LARGE_INTEGER& time) const
	{
		QueryPerformanceCounter(&time);
	}

	void GameClock::UpdateGameTime(GameTime& gameTime)
	{
		GetTime(currentTime);
		gameTime.SetElapsedGameTime((currentTime.QuadPart - lastTime.QuadPart) / frequency);
		gameTime.SetTotalGameTime((currentTime.QuadPart - startTime.QuadPart) / frequency);
		lastTime = currentTime;
	}
}