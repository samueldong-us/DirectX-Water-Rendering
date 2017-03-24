#include "Game.h"
#include "GameException.h"
#include "DrawableGameComponent.h"

using namespace std;

namespace Library
{
	const UINT Game::DefaultScreenWidth = 1024;
	const UINT Game::DefaultScreenHeight = 768;
	const UINT Game::DefaultFrameRate = 60;

	Game::Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand) :
		instance(instance), windowClass(windowClass), windowTitle(windowTitle), showCommand(showCommand), 
		windowHandle(), window(), screenWidth(DefaultScreenWidth), screenHeight(DefaultScreenHeight), gameClock(), gameTime(),
		featureLevel(D3D_FEATURE_LEVEL_10_0), direct3DDevice(nullptr), direct3DDeviceContext(nullptr), swapChain(nullptr),
		frameRate(DefaultFrameRate), isFullScreen(false), depthStencilBufferEnabled(false), depthStencilBuffer(nullptr),
		renderTargetView(nullptr), depthStencilView(nullptr), viewport() {}

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

	float Game::AspectRatio() const
	{
		return (float)screenWidth / (float)screenHeight;
	}

	int Game::ScreenWidth() const
	{
		return screenWidth;
	}

	int Game::ScreenHeight() const
	{
		return screenHeight;
	}

	ID3D11Device1 * Game::Direct3DDevice() const
	{
		return direct3DDevice;
	}

	ID3D11DeviceContext1 * Game::Direct3DDeviceContext() const
	{
		return direct3DDeviceContext;
	}

	bool Game::DepthBufferEnabled() const
	{
		return depthStencilBufferEnabled;
	}

	bool Game::IsFullScreen() const
	{
		return isFullScreen;
	}

	const D3D11_TEXTURE2D_DESC& Game::BackBufferDescription() const
	{
		return backBufferDescription;
	}

	const D3D11_VIEWPORT& Game::Viewport() const
	{
		return viewport;
	}

	const std::vector<GameComponent*>& Game::Components() const
	{
		return components;
	}

	const ServiceContainer & Game::Services() const
	{
		return serviceContainer;
	}

	void Game::Run()
	{
		InitializeWindow();
		InitializeDirectX();
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

	void Game::Initialize()
	{
		for (GameComponent* component : components)
		{
			component->Initialize();
		}
	}

	void Game::Update(const GameTime& gameTime)
	{
		for (GameComponent* component : components)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}
	}

	void Game::Draw(const GameTime& gameTime) 
	{
		for (GameComponent* component : components)
		{
			DrawableGameComponent* drawableComponent = component->As<DrawableGameComponent>();
			if (drawableComponent != nullptr && drawableComponent->Visible())
			{
				drawableComponent->Draw(gameTime);
			}
		}
	}

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

	void Game::InitializeDirectX()
	{
		HRESULT result;
		UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		ID3D11Device* tempDirect3DDevice = nullptr;
		ID3D11DeviceContext* tempDirect3DDeviceContext = nullptr;
		if (FAILED(result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &tempDirect3DDevice, &featureLevel, &tempDirect3DDeviceContext)))
		{
			throw GameException("D3D11CreateDevice() failed.", result);
		}
		if (FAILED(result = tempDirect3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&direct3DDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed.", result);
		}
		if (FAILED(result = tempDirect3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&direct3DDeviceContext))))
		{
			throw GameException("ID3D11DeviceContext::QueryInterface() failed.", result);
		}
		ReleaseObject(tempDirect3DDevice);
		ReleaseObject(tempDirect3DDeviceContext);

		DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));
		swapChainDescription.Width = screenWidth;
		swapChainDescription.Height = screenHeight;
		swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		IDXGIDevice* dxgiDevice = nullptr;
		if (FAILED(result = direct3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", result);
		}
		IDXGIAdapter* dxgiAdapter = nullptr;
		if (FAILED(result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
		{
			ReleaseObject(dxgiDevice);
			throw GameException("IDXGIDevice::GetParent() failed retrieving adapter.", result);
		}
		IDXGIFactory2* dxgiFactory = nullptr;
		if (FAILED(result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			throw GameException("IDXGIAdapter::GetParent() failed retrieving factory.", result);
		}
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDescription;
		ZeroMemory(&fullScreenDescription, sizeof(fullScreenDescription));
		fullScreenDescription.RefreshRate.Numerator = frameRate;
		fullScreenDescription.RefreshRate.Denominator = 1;
		fullScreenDescription.Windowed = !isFullScreen;
		if (FAILED(result = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, windowHandle, &swapChainDescription, &fullScreenDescription, nullptr, &swapChain)))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			ReleaseObject(dxgiFactory);
			throw GameException("IDXGIDevice::CreateSwapChainForHwnd() failed.", result);
		}
		ReleaseObject(dxgiDevice);
		ReleaseObject(dxgiAdapter);
		ReleaseObject(dxgiFactory);

		ID3D11Texture2D* backBuffer;
		if (FAILED(result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
		{
			throw GameException("IDXGISwapChain::GetBuffer() failed.", result);
		}
		backBuffer->GetDesc(&backBufferDescription);
		if (FAILED(result = direct3DDevice->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView)))
		{
			ReleaseObject(backBuffer);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", result);
		}
		ReleaseObject(backBuffer);

		if (depthStencilBufferEnabled)
		{
			D3D11_TEXTURE2D_DESC depthStencilDescription;
			ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
			depthStencilDescription.Width = screenWidth;
			depthStencilDescription.Height = screenHeight;
			depthStencilDescription.MipLevels = 1;
			depthStencilDescription.ArraySize = 1;
			depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDescription.SampleDesc.Count = 1;
			depthStencilDescription.SampleDesc.Quality = 0;

			if (FAILED(result = direct3DDevice->CreateTexture2D(&depthStencilDescription, nullptr, &depthStencilBuffer)))
			{
				throw GameException("IDXGIDevice:CreateTexture2D() failed.", result);
			}
			if (FAILED(result = direct3DDevice->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView)))
			{
				throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", result);
			}
		}

		direct3DDeviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = screenWidth;
		viewport.Height = screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		direct3DDeviceContext->RSSetViewports(1, &viewport);
	}

	void Game::Shutdown()
	{
		ReleaseObject(renderTargetView);
		ReleaseObject(depthStencilView);
		ReleaseObject(swapChain);
		ReleaseObject(depthStencilBuffer);

		if (direct3DDeviceContext != nullptr)
		{
			direct3DDeviceContext->ClearState();
		}

		ReleaseObject(direct3DDeviceContext);
		ReleaseObject(direct3DDevice);

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