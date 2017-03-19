#include "Game.h"

using namespace std;

namespace Library
{
	const UINT Game::DefaultScreenWidth = 1024;
	const UINT Game::DefaultScreenHeight = 768;

	Game::Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand) :
		instance(instance), windowClass(windowClass), windowTitle(windowTitle), showCommand(showCommand), 
		windowHandle(), window(), screenWidth(DefaultScreenWidth), screenHeight(DefaultScreenHeight), gameClock(), gameTime() {}

	Game::~Game() {}

	HINSTANCE Game::Instance() const
	{
		return instance;
	}

	HWND Game::WindowHandle() const
	{
		return windowHandle;
	}

	const WNDCLASSEX& Game::Window() const
	{
		return window;
	}

	const wstring& Game::WindowClass() const
	{
		return windowClass;
	}

	const wstring & Game::WindowTitle() const
	{
		return windowTitle;
	}

	int Game::ScreenWidth() const
	{
		return screenWidth;
	}

	int Game::ScreenHeight() const
	{
		return screenHeight;
	}

	void Game::Run()
	{
		InitializeWindow();
		Initialize();

		MSG message;
		ZeroMemory(&message, sizeof(message));

		gameClock.Reset();

		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				gameClock.UpdateGameTime(gameTime);
				Update(gameTime);
				Draw(gameTime);
			}
		}

		Shutdown();
	}

	void Game::Exit()
	{
		PostQuitMessage(0);
	}

	void Game::Initialize() {}

	void Game::Update(const GameTime & gameTime) {}

	void Game::Draw(const GameTime & gameTime) {}

	void Game::InitializeWindow()
	{
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;
		window.lpfnWndProc = WndProc;
		window.hInstance = instance;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = windowClass.c_str();

		RECT windowSize = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&window);
		POINT center = CenterWindow(screenWidth, screenHeight);
		windowHandle = CreateWindow(windowClass.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, nullptr, nullptr, instance, nullptr);

		ShowWindow(windowHandle, showCommand);
		UpdateWindow(windowHandle);
	}

	void Game::Shutdown()
	{
		UnregisterClass(windowClass.c_str(), window.hInstance);
	}

	POINT Game::CenterWindow(int windowWidth, int windowHeight)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		POINT center;
		center.x = (screenWidth - windowWidth) / 2;
		center.y = (screenHeight - windowHeight) / 2;

		return center;
	}

	LRESULT Game::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}