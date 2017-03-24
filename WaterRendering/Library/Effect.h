#pragma once

#include "Common.h"
#include "Game.h"
#include "Technique.h"
#include "Variable.h"

namespace Library
{
	class Technique;
	class Variable;

	class Effect
	{
	public:
		Effect(Game& game);
		virtual ~Effect();

		static void CompileEffectFromFile(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const std::wstring& filename);
		static void LoadCompiledEffect(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const std::wstring& filename);

		Game& GetGame();
		ID3DX11Effect* GetEffect() const;
		const D3DX11_EFFECT_DESC& EffectDescription() const;
		const std::vector<Technique*>& Techniques() const;
		const std::map<std::string, Technique*>& TechniquesByName() const;
		const std::vector<Variable*>& Variables() const;
		const std::map<std::string, Variable*>& VariablesByName() const;
		void CompileFromFile(const std::wstring& filename);
		void LoadCompiledEffect(const std::wstring& filename);

	private:
		void Initialize();

		Game& game;
		ID3DX11Effect* effect;
		D3DX11_EFFECT_DESC effectDescription;
		std::vector<Technique*> techniques;
		std::map<std::string, Technique*> techniquesByName;
		std::vector<Variable*> variables;
		std::map<std::string, Variable*> variablesByName;
	};
}