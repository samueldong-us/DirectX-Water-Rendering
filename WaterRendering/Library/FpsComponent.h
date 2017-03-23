#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "DrawableGameComponent.h"

namespace Library
{
	class FpsComponent : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(FpsComponent, DrawableGameComponent)

	public:
		FpsComponent(Game& game);
		~FpsComponent();

		XMFLOAT2& TextPosition();
		int FrameRate() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		SpriteBatch* spriteBatch;
		SpriteFont* spriteFont;
		XMFLOAT2 textPosition;

		int frameCount;
		int frameRate;
		double lastTotalElapsedTime;
	};
}