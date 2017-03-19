#pragma once

#include <Windows.h>
#include <string>

#include "GameClock.h"
#include "GameTime.h"

namespace Library
{
	class Game
	{
	public:
		Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
		virtual ~Game();

		HINSTANCE Instance() const;
		HWND WindowHandle() const;
		const WNDCLASSEX& Window() const;
		const std::wstring& WindowClass() const;
		const std::wstring& WindowTitle() const;
		int ScreenWidth() const;
		int ScreenHeight() const;

		virtual void Run();
		virtual void Exit();
		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);
		
	protected:
		virtual void InitializeWindow();
		virtual void Shutdown();

		static const UINT DefaultScreenWidth;
		static const UINT DefaultScreenHeight;

		HINSTANCE instance;
		std::wstring windowClass;
		std::wstring windowTitle;
		int showCommand;

		HWND windowHandle;
		WNDCLASSEX window;

		UINT screenWidth;
		UINT screenHeight;

		GameClock gameClock;
		GameTime gameTime;

	private:
		POINT CenterWindow(int windowWidth, int windowHeight);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	};
}