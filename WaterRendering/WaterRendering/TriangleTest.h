#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"
#include "Effect.h"

using namespace Library;

namespace Rendering
{
	class TriangleTest : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TriangleTest, DrawableGameComponent)

	public:
		TriangleTest(Game& game, Camera& camera);
		~TriangleTest();

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct BasicEffectVertex
		{
			BasicEffectVertex() {}
			BasicEffectVertex(XMFLOAT4 position, XMFLOAT4 color) : Position(position), Color(color) {}
			XMFLOAT4 Position;
			XMFLOAT4 Color;
		};

		Effect* basicEffect;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
	};
}