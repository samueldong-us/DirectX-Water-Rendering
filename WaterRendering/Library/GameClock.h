#pragma once

#include <Windows.h>
#include <exception>

#include "GameTime.h"

namespace Library
{
	class GameClock
	{
	public:
		GameClock();

		const LARGE_INTEGER& StartTime() const;
		const LARGE_INTEGER& CurrentTime() const;
		const LARGE_INTEGER& LastTime() const;

		void Reset();
		double GetFrequency() const;
		void GetTime(LARGE_INTEGER& time) const;
		void UpdateGameTime(GameTime& gameTime);

	private:
		LARGE_INTEGER startTime;
		LARGE_INTEGER currentTime;
		LARGE_INTEGER lastTime;
		double frequency;
	};
}