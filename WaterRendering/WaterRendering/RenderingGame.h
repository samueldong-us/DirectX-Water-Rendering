#pragma once

#include <dinput.h>

#include "Game.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "FirstPersonCamera.h"
#include "FpsComponent.h"
#include "TriangleTest.h"
#include "Water.h"

using namespace Library;

namespace Rendering
{
	class RenderingGame : public Game
	{
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
		~RenderingGame();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		static const XMVECTORF32 BackgroundColor;

		LPDIRECTINPUT8 directInput;
		KeyboardComponent* keyboard;
		MouseComponent* mouse;
		FirstPersonCamera* firstPersonCamera;

		FpsComponent* fpsComponent;
		Water* water;

		SpriteBatch* spriteBatch;
		SpriteFont* spriteFont;

		virtual void Shutdown() override;
	};
}