#include "DrawableGameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent() : GameComponent(), visible(true) {}

	DrawableGameComponent::DrawableGameComponent(Game& game) : GameComponent(game), visible(true) {}

	DrawableGameComponent::~DrawableGameComponent() {}

	bool DrawableGameComponent::Visible() const
	{
		return visible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		this->visible = visible;
	}

	void DrawableGameComponent::Draw(const GameTime& gameTime) {}
}