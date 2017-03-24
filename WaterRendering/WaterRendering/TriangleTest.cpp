#include "Utility.h"
#include "TriangleTest.h"

namespace Rendering
{
	RTTI_DEFINITIONS(TriangleTest)

	TriangleTest::TriangleTest(Game& game, Camera& camera) : DrawableGameComponent(game, camera) {}
	
	TriangleTest::~TriangleTest()
	{
		delete basicEffect;
		ReleaseObject(inputLayout);
		ReleaseObject(vertexBuffer);
		ReleaseObject(indexBuffer);
	}

	void TriangleTest::Initialize()
	{
		basicEffect = new Effect(*game);
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		basicEffect->LoadCompiledEffect(L"Content\\Effects\\BasicEffect.cso");

		Technique* technique = basicEffect->TechniquesByName().find("main11")->second;
		Pass* pass = technique->PassesByName().find("p0")->second;
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		pass->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &inputLayout);

		BasicEffectVertex vertices[] =
		{
			BasicEffectVertex(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
			BasicEffectVertex(XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)),
			BasicEffectVertex(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
			BasicEffectVertex(XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
		};
		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(vertexBufferDescription));
		vertexBufferDescription.ByteWidth = sizeof(vertices);
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubresourceData;
		ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));
		vertexSubresourceData.pSysMem = vertices;
		if (FAILED(game->Direct3DDevice()->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}

		UINT indices[] = { 0, 1, 2, 2, 3, 0 };
		D3D11_BUFFER_DESC indexBufferDescription;
		ZeroMemory(&indexBufferDescription, sizeof(indexBufferDescription));
		indexBufferDescription.ByteWidth = sizeof(indices);
		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubresourceData;
		ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));
		indexSubresourceData.pSysMem = indices;
		if (FAILED(game->Direct3DDevice()->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	void TriangleTest::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* context = game->Direct3DDeviceContext();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(inputLayout);
		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Technique* technique = basicEffect->TechniquesByName().find("main11")->second;
		Pass* pass = technique->PassesByName().find("p0")->second;
		Variable* worldViewProjection = basicEffect->VariablesByName().find("WorldViewProjection")->second;
		*worldViewProjection << camera->ViewProjectionMatrix();
		pass->Apply(0, context);
		context->DrawIndexed(6, 0, 0);
	}
}