#include "Variable.h"

using namespace std;

namespace Library
{
	Variable::Variable(Effect & effect, ID3DX11EffectVariable * variable) : effect(effect), variable(variable)
	{
		variable->GetDesc(&variableDescription);
		name = variableDescription.Name;
		type = variable->GetType();
		type->GetDesc(&typeDescription);
	}

	Effect& Variable::GetEffect()
	{
		return effect;
	}

	ID3DX11EffectVariable* Variable::GetVariable() const
	{
		return variable;
	}

	const D3DX11_EFFECT_VARIABLE_DESC& Variable::VariableDescription() const
	{
		return variableDescription;
	}

	ID3DX11EffectType * Variable::Type() const
	{
		return type;
	}

	const D3DX11_EFFECT_TYPE_DESC& Variable::TypeDescription() const
	{
		return typeDescription;
	}

	const string& Variable::Name() const
	{
		return name;
	}

	Variable& Variable::operator<<(CXMMATRIX value)
	{
		ID3DX11EffectMatrixVariable* variable = this->variable->AsMatrix();
		if (!variable->IsValid())
		{
			throw GameException("Invalid effect variable cast.");
		}
		variable->SetMatrix(reinterpret_cast<const float*>(&value));
		return *this;
	}

	Variable& Variable::operator<<(ID3D11ShaderResourceView* value)
	{
		ID3DX11EffectShaderResourceVariable* variable = this->variable->AsShaderResource();
		if (!variable->IsValid())
		{
			throw GameException("Invalid effect variable cast.");
		}
		variable->SetResource(value);
		return *this;
	}

	Variable& Variable::operator<<(FXMVECTOR value)
	{
		ID3DX11EffectVectorVariable* variable = this->variable->AsVector();
		if (!variable->IsValid())
		{
			throw GameException("Invalid effect variable cast.");
		}
		variable->SetFloatVector(reinterpret_cast<const float*>(&value));
		return *this;
	}

	Variable& Variable::operator<<(float value)
	{
		ID3DX11EffectScalarVariable* variable = this->variable->AsScalar();
		if (!variable->IsValid())
		{
			throw GameException("Invalid effect variable cast.");
		}
		variable->SetFloat(value);
		return *this;
	}
}