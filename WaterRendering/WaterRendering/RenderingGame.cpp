#include <sstream>

#include "GameException.h"
#include "Utility.h"

#include "RenderingGame.h"

using namespace std;

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };
	
	RenderingGame::RenderingGame(HINSTANCE instance, const wstring& windowClass, const wstring& windowTitle, int showCommand) :
		Game(instance, windowClass, windowTitle, showCommand)
	{
		depthStencilBufferEnabled = true;
	}

	RenderingGame::~RenderingGame()
	{
		for (GameComponent* component : components)
		{
			delete component;
		}
	}

	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&directInput, nullptr)))
		{
			throw GameException("DirectInput8Create() failed.");
		}
		keyboard = new KeyboardComponent(*this, directInput);
		components.push_back(keyboard);
		serviceContainer.AddService(keyboard);

		mouse = new MouseComponent(*this, directInput);
		components.push_back(mouse);
		serviceContainer.AddService(mouse);

		firstPersonCamera = new FirstPersonCamera(*this);
		components.push_back(firstPersonCamera);

		fpsComponent = new FpsComponent(*this);
		components.push_back(fpsComponent);

		triangleTest = new TriangleTest(*this, *firstPersonCamera);
		components.push_back(triangleTest);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		spriteBatch = new SpriteBatch(direct3DDeviceContext);
		spriteFont = new SpriteFont(direct3DDevice, L"Content\\Fonts\\SourceSansPro_14_Regular.spritefont");

		Game::Initialize();
	}

	void RenderingGame::Update(const GameTime& gameTime)
	{
		if (keyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}
		KeyboardComponent* test = serviceContainer.GetService<KeyboardComponent>();

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime& gameTime)
	{
		direct3DDeviceContext->ClearRenderTargetView(renderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
		direct3DDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		wostringstream mouseInfo;
		mouseInfo << "X: " << (mouse->IsButtonDown(MouseLeftButton) ? "Down" : "Up") << "    Y: " << firstPersonCamera->Direction().y << "    Z: " << firstPersonCamera->Direction().z;
		spriteBatch->Begin();
		spriteFont->DrawString(spriteBatch, mouseInfo.str().c_str(), XMFLOAT2(0.0f, 30.0f));
		spriteBatch->End();
		Game::Draw(gameTime);

		HRESULT result;
		if (FAILED(result = swapChain->Present(0, 0)))
		{
			throw GameException("IDXGISwapChain::Present() failed.", result);
		}
	}

	void RenderingGame::Shutdown()
	{
		ReleaseObject(directInput);
		DeleteObject(spriteBatch);
		DeleteObject(spriteFont);
		Game::Shutdown();
	}
}