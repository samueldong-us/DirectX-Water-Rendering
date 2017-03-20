#include <Windows.h>
#include <memory>

#include "RenderingGame.h"
#include "GameException.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;
using namespace Library;
using namespace Rendering;

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR commandLine, int showCommand)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	unique_ptr<RenderingGame> game(new RenderingGame(instance, L"RenderingClass", L"Water Rendering", showCommand));

	try
	{
		game->Run();
	}
	catch (GameException e)
	{
		MessageBox(game->WindowHandle(), e.WhatW().c_str(), game->WindowTitle().c_str(), MB_ABORTRETRYIGNORE);
	}

	return 0;
}