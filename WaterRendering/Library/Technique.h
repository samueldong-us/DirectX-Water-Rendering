#pragma once

#include "Common.h"
#include "Game.h"
#include "Effect.h"
#include "Pass.h"

namespace Library
{
	class Effect;
	class Pass;

	class Technique
	{
	public:
		Technique(Game& game, Effect& effect, ID3DX11EffectTechnique* technique);
		~Technique();

		Effect& GetEffect();
		ID3DX11EffectTechnique* GetTechnique() const;
		const D3DX11_TECHNIQUE_DESC& TechniqueDescription() const;
		const std::string& Name() const;
		const std::vector<Pass*>& Passes() const;
		const std::map<std::string, Pass*>& PassesByName() const;

	private:
		Effect& effect;
		ID3DX11EffectTechnique* technique;
		D3DX11_TECHNIQUE_DESC techniqueDescription;
		std::string name;
		std::vector<Pass*> passes;
		std::map<std::string, Pass*> passesByName;
	};
}