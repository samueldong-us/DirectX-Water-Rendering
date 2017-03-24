#pragma once

#include "Common.h"
#include "Effect.h"

namespace Library
{
	class Effect;
	class Variable
	{
	public:
		Variable(Effect& effect, ID3DX11EffectVariable* variable);

		Effect& GetEffect();
		ID3DX11EffectVariable* GetVariable() const;
		const D3DX11_EFFECT_VARIABLE_DESC& VariableDescription() const;
		ID3DX11EffectType* Type() const;
		const D3DX11_EFFECT_TYPE_DESC& TypeDescription() const;
		const std::string& Name() const;

		Variable& operator<<(CXMMATRIX value);
		Variable& operator<<(ID3D11ShaderResourceView* value);
		Variable& operator<<(FXMVECTOR value);
		Variable& operator<<(float value);

	private:
		Effect& effect;
		ID3DX11EffectVariable* variable;
		D3DX11_EFFECT_VARIABLE_DESC variableDescription;
		ID3DX11EffectType* type;
		D3DX11_EFFECT_TYPE_DESC typeDescription;
		std::string name;
	};
}