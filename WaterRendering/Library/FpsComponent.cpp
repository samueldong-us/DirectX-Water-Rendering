#include <sstream>
#include <iomanip>

#include "Utility.h"
#include "FpsComponent.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(FpsComponent)

	FpsComponent::FpsComponent(Game& game) :
		DrawableGameComponent(game), spriteBatch(nullptr), spriteFont(nullptr), textPosition(0.0f, 60.0f),
		frameCount(), frameRate(), lastTotalElapsedTime() {}

	FpsComponent::~FpsComponent()
	{
		DeleteObject(spriteFont);
		DeleteObject(spriteBatch);
	}

	XMFLOAT2& FpsComponent::TextPosition()
	{
		return textPosition;
	}

	int FpsComponent::FrameRate() const
	{
		return frameRate;
	}

	void FpsComponent::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		spriteBatch = new SpriteBatch(game->Direct3DDeviceContext());
		spriteFont = new SpriteFont(game->Direct3DDevice(), L"Content\\Fonts\\SourceSansPro_14_Regular.spritefont");
	}

	void FpsComponent::Update(const GameTime& gameTime)
	{
		if (gameTime.TotalGameTime() - lastTotalElapsedTime >= 1.0)
		{
			lastTotalElapsedTime = gameTime.TotalGameTime();
			frameRate = frameCount;
			frameCount = 0;
		}
		frameCount++;
	}

	void FpsComponent::Draw(const GameTime& gameTime)
	{
		spriteBatch->Begin();
		wostringstream fpsText;
		fpsText << setprecision(4) << L"Frame Rate: " << frameRate << "    Total Elapsed Time: " << gameTime.TotalGameTime();
		spriteFont->DrawString(spriteBatch, fpsText.str().c_str(), textPosition);
		spriteBatch->End();
	}
}