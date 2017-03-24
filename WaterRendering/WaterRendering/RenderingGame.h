#pragma once

#include <dinput.h>

#include "Game.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "FpsComponent.h"
#include "ServiceContainer.h"

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

		const ServiceContainer& Services() const;

	private:
		static const XMVECTORF32 BackgroundColor;

		ServiceContainer serviceContainer;

		LPDIRECTINPUT8 directInput;
		KeyboardComponent* keyboard;
		MouseComponent* mouse;

		FpsComponent* fpsComponent;

		virtual void Shutdown() override;
	};
}