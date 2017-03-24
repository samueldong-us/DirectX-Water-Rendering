#include "RenderStateHelper.h"

namespace Library
{
	RenderStateHelper::RenderStateHelper(Game& game) :
		game(game), rasterizerState(nullptr), blendState(nullptr), blendFactor(new FLOAT[4]),
		sampleMask(UINT_MAX), depthStencilState(nullptr), stencilRef(UINT_MAX) {}

	RenderStateHelper::~RenderStateHelper()
	{
		ReleaseObject(rasterizerState);
		ReleaseObject(blendState);
		ReleaseObject(depthStencilState);
		DeleteObjects(blendFactor);
	}

	ID3D11RasterizerState* RenderStateHelper::RasterizerState()
	{
		return rasterizerState;
	}

	ID3D11BlendState* RenderStateHelper::BlendState()
	{
		return blendState;
	}

	ID3D11DepthStencilState* RenderStateHelper::DepthStencilState()
	{
		return depthStencilState;
	}

	void RenderStateHelper::ResetAll(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->RSSetState(nullptr);
		deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
		deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
	}

	void RenderStateHelper::SaveRasterizerState()
	{
		ReleaseObject(rasterizerState);
		game.Direct3DDeviceContext()->RSGetState(&rasterizerState);
	}

	void RenderStateHelper::RestoreRasterizerState() const
	{
		game.Direct3DDeviceContext()->RSSetState(rasterizerState);
	}

	void RenderStateHelper::SaveBlendState()
	{
		ReleaseObject(blendState);
		game.Direct3DDeviceContext()->OMGetBlendState(&blendState, blendFactor, &sampleMask);
	}

	void RenderStateHelper::RestoreBlendState() const
	{
		game.Direct3DDeviceContext()->OMSetBlendState(blendState, blendFactor, sampleMask);
	}

	void RenderStateHelper::SaveDepthStencilState()
	{
		ReleaseObject(depthStencilState);
		game.Direct3DDeviceContext()->OMGetDepthStencilState(&depthStencilState, &stencilRef);
	}

	void RenderStateHelper::RestoreDepthStencilState() const
	{
		game.Direct3DDeviceContext()->OMSetDepthStencilState(depthStencilState, stencilRef);
	}

	void RenderStateHelper::SaveAll()
	{
		SaveRasterizerState();
		SaveBlendState();
		SaveDepthStencilState();
	}

	void RenderStateHelper::RestoreAll() const
	{
		RestoreRasterizerState();
		RestoreBlendState();
		RestoreDepthStencilState();
	}

	void RenderStateHelper::ResetShaders() const
	{
		ID3D11DeviceContext* context = game.Direct3DDeviceContext();
		context->VSSetShader(NULL, NULL, 0);
		context->HSSetShader(NULL, NULL, 0);
		context->DSSetShader(NULL, NULL, 0);
		context->GSSetShader(NULL, NULL, 0);
		context->PSSetShader(NULL, NULL, 0);
	}
}