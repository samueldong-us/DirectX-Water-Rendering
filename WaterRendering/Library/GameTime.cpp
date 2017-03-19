#include "GameTime.h"

namespace Library
{
	GameTime::GameTime() : elapsedGameTime(), totalGameTime() {}

	double GameTime::ElapsedGameTime() const
	{
		return elapsedGameTime;
	}

	double GameTime::TotalGameTime() const
	{
		return totalGameTime;
	}

	void GameTime::SetElapsedGameTime(double time)
	{
		elapsedGameTime = time;
	}

	void GameTime::SetTotalGameTime(double time)
	{
		totalGameTime = time;
	}
}