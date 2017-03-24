#pragma once

#include "Common.h"
#include "Game.h"
#include "Technique.h"

namespace Library
{
	class Technique;

	class Pass
	{
	public:
		Pass(Game& game, Technique& technique, ID3DX11EffectPass* pass);

		Technique& GetTechnique();
		ID3DX11EffectPass* GetPass() const;
		const D3DX11_PASS_DESC& PassDescription() const;
		const std::string& Name() const;

		void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDescription, UINT numberOfElements, ID3D11InputLayout** inputLayout);
		void Apply(UINT flags, ID3D11DeviceContext* context);

	private:
		Game& game;
		Technique& technique;
		ID3DX11EffectPass* pass;
		D3DX11_PASS_DESC passDescription;
		std::string name;
	};
}