#include "Pass.h"

using namespace std;

namespace Library
{
	Pass::Pass(Game& game, Technique& technique, ID3DX11EffectPass* pass) : game(game), technique(technique), pass(pass)
	{
		pass->GetDesc(&passDescription);
		name = passDescription.Name;
	}

	Technique& Pass::GetTechnique()
	{
		return technique;
	}

	ID3DX11EffectPass* Pass::GetPass() const
	{
		return pass;
	}

	const D3DX11_PASS_DESC& Pass::PassDescription() const
	{
		return passDescription;
	}

	const string& Pass::Name() const
	{
		return name;
	}

	void Pass::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDescription, UINT numberOfElements, ID3D11InputLayout** inputLayout)
	{
		HRESULT result = game.Direct3DDevice()->CreateInputLayout(inputElementDescription, numberOfElements, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, inputLayout);
		if (FAILED(result))
		{
			throw GameException("ID3D11Device::CreateInputLayout() failed.", result);
		}
	}

	void Pass::Apply(UINT flags, ID3D11DeviceContext* context)
	{
		pass->Apply(flags, context);
	}
}