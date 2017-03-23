#include "GameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(GameComponent)

	GameComponent::GameComponent() : game(nullptr), enabled(true) {}
	
	GameComponent::GameComponent(Game& game) : game(&game), enabled(true) {}

	GameComponent::~GameComponent() {}

	Game* GameComponent::GetGame()
	{
		return game;
	}

	void GameComponent::SetGame(Game& game)
	{
		this->game = &game;
	}

	bool GameComponent::Enabled() const
	{
		return enabled;
	}

	void GameComponent::SetEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

	void GameComponent::Initialize() {}

	void GameComponent::Update(const GameTime& gameTime) {}
}