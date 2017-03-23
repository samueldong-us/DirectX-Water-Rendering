#pragma once

#include "GameComponent.h"

namespace Library
{
	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent)

	public:
		DrawableGameComponent();
		DrawableGameComponent(Game& game);
		virtual ~DrawableGameComponent();

		bool Visible() const;
		void SetVisible(bool visible);

		virtual void Draw(const GameTime& gameTime);

	protected:
		bool visible;
	};
}