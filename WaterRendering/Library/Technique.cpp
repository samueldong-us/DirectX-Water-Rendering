#include "Technique.h"

using namespace std;

namespace Library
{
	Technique::Technique(Game& game, Effect& effect, ID3DX11EffectTechnique* technique) : effect(effect), technique(technique)
	{
		technique->GetDesc(&techniqueDescription);
		name = techniqueDescription.Name;
		for (UINT i = 0; i < techniqueDescription.Passes; i++)
		{
			Pass* pass = new Pass(game, *this, technique->GetPassByIndex(i));
			passes.push_back(pass);
			passesByName[pass->Name()] = pass;
		}
	}

	Technique::~Technique()
	{
		for (Pass* pass : passes)
		{
			delete pass;
		}
	}

	Effect& Technique::GetEffect()
	{
		return effect;
	}

	ID3DX11EffectTechnique* Technique::GetTechnique() const
	{
		return technique;
	}

	const D3DX11_TECHNIQUE_DESC& Technique::TechniqueDescription() const
	{
		return techniqueDescription;
	}

	const string& Technique::Name() const
	{
		return name;
	}

	const vector<Pass*>& Technique::Passes() const
	{
		return passes;
	}

	const map<string, Pass*>& Technique::PassesByName() const
	{
		return passesByName;
	}
}