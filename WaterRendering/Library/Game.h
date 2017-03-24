#pragma once

#include "Common.h"
#include "GameClock.h"
#include "GameTime.h"
#include "GameComponent.h"
#include "ServiceContainer.h"
#include "RenderStateHelper.h"

namespace Library
{
	class GameComponent;
	class RenderStateHelper;

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
		float AspectRatio() const;
		int ScreenWidth() const;
		int ScreenHeight() const;

		ID3D11Device1* Direct3DDevice() const;
		ID3D11DeviceContext1* Direct3DDeviceContext() const;
		bool DepthBufferEnabled() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDescription() const;
		const D3D11_VIEWPORT& Viewport() const;

		const std::vector<GameComponent*>& Components() const;
		const ServiceContainer& Services() const;

		virtual void Run();
		virtual void Exit();
		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);
		
	protected:
		virtual void InitializeWindow();
		virtual void InitializeDirectX();
		virtual void Shutdown();

		static const UINT DefaultScreenWidth;
		static const UINT DefaultScreenHeight;
		static const UINT DefaultFrameRate;

		HINSTANCE instance;
		std::wstring windowClass;
		std::wstring windowTitle;
		int showCommand;

		HWND windowHandle;
		WNDCLASSEX window;

		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Device1* direct3DDevice;
		ID3D11DeviceContext1* direct3DDeviceContext;
		IDXGISwapChain1* swapChain;
		
		UINT frameRate;
		bool isFullScreen;
		bool depthStencilBufferEnabled;
		
		ID3D11Texture2D* depthStencilBuffer;
		D3D11_TEXTURE2D_DESC backBufferDescription;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11DepthStencilView* depthStencilView;
		D3D11_VIEWPORT viewport;

		UINT screenWidth;
		UINT screenHeight;

		GameClock gameClock;
		GameTime gameTime;
		std::vector<GameComponent*> components;

		ServiceContainer serviceContainer;
		RenderStateHelper* stateHelper;

	private:
		POINT CenterWindow(int windowWidth, int windowHeight);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	};
}