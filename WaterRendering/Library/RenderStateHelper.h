#pragma once

#include "Common.h"
#include "Game.h"

namespace Library
{
	class Game;

	class RenderStateHelper
	{
	public:
		RenderStateHelper(Game& game);
		~RenderStateHelper();

		static void ResetAll(ID3D11DeviceContext* deviceContext);

		ID3D11RasterizerState* RasterizerState();
		ID3D11BlendState* BlendState();
		ID3D11DepthStencilState* DepthStencilState();

		void SaveRasterizerState();
		void RestoreRasterizerState() const;
		void SaveBlendState();
		void RestoreBlendState() const;
		void SaveDepthStencilState();
		void RestoreDepthStencilState() const;
		void SaveAll();
		void RestoreAll() const;
		void ResetShaders() const;

	private:
		Game& game;
		ID3D11RasterizerState* rasterizerState;
		ID3D11BlendState* blendState;
		FLOAT* blendFactor;
		UINT sampleMask;
		ID3D11DepthStencilState* depthStencilState;
		UINT stencilRef;
	};
}