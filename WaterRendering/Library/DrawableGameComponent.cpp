#include "DrawableGameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent() : GameComponent(), visible(true) {}

	DrawableGameComponent::DrawableGameComponent(Game& game) : GameComponent(game), visible(true) {}

	DrawableGameComponent::DrawableGameComponent(Game& game, Camera& camera) : GameComponent(game), visible(true), camera(&camera) {}

	DrawableGameComponent::~DrawableGameComponent() {}

	bool DrawableGameComponent::Visible() const
	{
		return visible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		this->visible = visible;
	}

	Camera* DrawableGameComponent::GetCamera()
	{
		return camera;
	}

	void DrawableGameComponent::SetCamera(Camera* camera)
	{
		this->camera = camera;
	}

	void DrawableGameComponent::Draw(const GameTime& gameTime) {}
}