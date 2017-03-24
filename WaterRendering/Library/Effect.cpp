#include "Utility.h"
#include "Effect.h"

using namespace std;

namespace Library
{
	void Effect::CompileEffectFromFile(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const wstring& filename)
	{
		UINT shaderFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessage = nullptr;
		HRESULT result = D3DCompileFromFile(filename.c_str(), nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessage);
		if (errorMessage != nullptr)
		{
			GameException exception((char*)errorMessage->GetBufferPointer(), result);
			ReleaseObject(errorMessage);
			throw exception;
		}

		if (FAILED(result))
		{
			throw GameException("D3DX11CompileFromFile() failed.", result);
		}

		result = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, direct3DDevice, effect);
		if (FAILED(result))
		{
			throw GameException("D3DX11CreateEffectFromMemory() failed.", result);
		}
		ReleaseObject(compiledShader);
	}

	void Effect::LoadCompiledEffect(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const wstring& filename)
	{
		std::vector<char> compiledShader;
		Utility::LoadBinaryFile(filename, compiledShader);

		HRESULT result = D3DX11CreateEffectFromMemory(&compiledShader.front(), compiledShader.size(), NULL, direct3DDevice, effect);
		if (FAILED(result))
		{
			throw GameException("D3DX11CreateEffectFromMemory() failed.", result);
		}
	}

	Effect::Effect(Game& game) : game(game), effect(nullptr), effectDescription() {}

	Effect::~Effect()
	{
		for (Technique* technique : techniques)
		{
			delete technique;
		}
		for (Variable* variable : variables)
		{
			delete variable;
		}
		ReleaseObject(effect);
	}

	Game& Effect::GetGame()
	{
		return game;
	}

	ID3DX11Effect* Effect::GetEffect() const
	{
		return effect;
	}

	const D3DX11_EFFECT_DESC& Effect::EffectDescription() const
	{
		return effectDescription;
	}

	const vector<Technique*>& Effect::Techniques() const
	{
		return techniques;
	}

	const map<string, Technique*>& Effect::TechniquesByName() const
	{
		return techniquesByName;
	}

	const vector<Variable*>& Effect::Variables() const
	{
		return variables;
	}

	const map<string, Variable*>& Effect::VariablesByName() const
	{
		return variablesByName;
	}

	void Effect::CompileFromFile(const wstring & filename)
	{
		CompileEffectFromFile(game.Direct3DDevice(), &effect, filename);
		Initialize();
	}

	void Effect::LoadCompiledEffect(const wstring & filename)
	{
		LoadCompiledEffect(game.Direct3DDevice(), &effect, filename);
		Initialize();
	}

	void Effect::Initialize()
	{
		HRESULT result = effect->GetDesc(&effectDescription);
		if (FAILED(result))
		{
			throw GameException("ID3DX11Effect::GetDesc() failed.", result);
		}

		for (UINT i = 0; i < effectDescription.Techniques; i++)
		{
			Technique* technique = new Technique(game, *this, effect->GetTechniqueByIndex(i));
			techniques.push_back(technique);
			techniquesByName[technique->Name()] = technique;
		}

		for (UINT i = 0; i < effectDescription.GlobalVariables; i++)
		{
			Variable* variable = new Variable(*this, effect->GetVariableByIndex(i));
			variables.push_back(variable);
			variablesByName[variable->Name()] = variable;
		}
	}
}