#pragma once

#include "Common.h"
#include "Game.h"
#include "GameException.h"

namespace Library
{
	class Game;

	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI)

	public:
		GameComponent();
		GameComponent(Game& game);
		virtual ~GameComponent();

		Game* GetGame();
		void SetGame(Game& game);
		bool Enabled() const;
		void SetEnabled(bool enabled);

		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);

	protected:
		Game* game;
		bool enabled;
	};
}