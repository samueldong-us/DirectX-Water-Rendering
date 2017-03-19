#pragma once

namespace Library
{
	class GameTime
	{
	public:
		GameTime();

		double ElapsedGameTime() const;
		double TotalGameTime() const;
		void SetElapsedGameTime(double time);
		void SetTotalGameTime(double time);

	private:
		double elapsedGameTime;
		double totalGameTime;
	};
}