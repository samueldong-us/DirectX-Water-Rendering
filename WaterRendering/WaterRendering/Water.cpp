#include "Utility.h"
#include "Water.h"

using namespace std;

namespace Rendering
{
	RTTI_DEFINITIONS(Water)

	const int Water::Levels = 10;
	const int Water::Resolution = 32;
	const float Water::MaximumDistance = 1024.0f;

	Water::Water(Game& game, Camera& camera) : DrawableGameComponent(game, camera) {}

	Water::~Water()
	{
		delete effect;
		ReleaseObject(inputLayout);
		ReleaseObject(vertexBuffer);
	}

	void Water::Initialize()
	{
		nodes.emplace_back(XMFLOAT2(0.0f, 0.0f), 512.0f);
		effect = new Effect(*game);
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		effect->LoadCompiledEffect(L"Content\\Effects\\Water.cso");

		Technique* technique = effect->TechniquesByName().find("main11")->second;
		Pass* pass = technique->PassesByName().find("WireFrame")->second;
		D3D11_INPUT_ELEMENT_DESC inputElementDescription[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		pass->CreateInputLayout(inputElementDescription, ARRAYSIZE(inputElementDescription), &inputLayout);

		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(vertexBufferDescription));
		vertexBufferDescription.ByteWidth = sizeof(WaterVertex) * nodes.size() * powf(4.0f, Levels);
		vertexBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (FAILED(game->Direct3DDevice()->CreateBuffer(&vertexBufferDescription, nullptr, &vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	void Water::Update(const GameTime& gameTime)
	{
		vector<WaterVertex> output;
		for (Node node : nodes)
		{
			SelectNodes(node, MaximumDistance, Levels, output);
		}
		outputSize = output.size();

		D3D11_MAPPED_SUBRESOURCE mappedVertex;
		ZeroMemory(&mappedVertex, sizeof(mappedVertex));
		ID3D11DeviceContext* context = game->Direct3DDeviceContext();
		context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertex);
		memcpy(mappedVertex.pData, &output[0], sizeof(WaterVertex) * outputSize);
		context->Unmap(vertexBuffer, 0);
	}

	void Water::Draw(const GameTime& gameTime)
	{
		
		ID3D11DeviceContext* context = game->Direct3DDeviceContext();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		context->IASetInputLayout(inputLayout);
		UINT stride = sizeof(WaterVertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		Technique* technique = effect->TechniquesByName().find("main11")->second;
		Pass* pass = technique->PassesByName().find("WireFrame")->second;
		Variable* worldViewProjection = effect->VariablesByName().find("WorldViewProjection")->second;
		*worldViewProjection << camera->ViewProjectionMatrix();
		Variable* resolution = effect->VariablesByName().find("Resolution")->second;
		*resolution << Resolution;
		Variable* cameraPosition = effect->VariablesByName().find("CameraPosition")->second;
		*cameraPosition << camera->PositionVector();
		pass->Apply(0, context);
		context->Draw(outputSize, 0);
	}

	bool Water::SelectNodes(Node node, float range, int level, vector<WaterVertex>& output)
	{
		if (!InRange(node, range))
			return false;
		if (level == 0)
		{
			AddNode(node, 1.0f, range, output);
			return true;
		}
		else
		{
			if (!InRange(node, range / 2.0f))
			{
				AddNode(node, 1.0f, range, output);
			}
			else
			{
				Node children[4];
				Children(node, children);
				for (int i = 0; i < 4; i++)
				{
					if (!SelectNodes(children[i], range / 2.0f, level - 1, output))
					{
						AddNode(children[i], 2.0f, range, output);
					}
				}
			}
			return true;
		}
	}

	void Water::AddNode(Node node, float factor, float range, vector<WaterVertex>& output)
	{
		XMFLOAT2 vertices[4];
		Vertices(node, vertices);
		for (int i = 0; i < 4; i++)
		{
			output.push_back(WaterVertex(XMFLOAT4(vertices[i].x, vertices[i].y, 0.0f, 1.0f), XMFLOAT2(factor, range)));
		}
	}

	bool Water::InRange(Node node, float range)
	{
		XMFLOAT2 vertices[4];
		Vertices(node, vertices);
		XMVECTOR camera = this->camera->PositionVector();
		bool insidePlane = true;
		for (int i = 0; i < 4; i++)
		{
			int j = (i + 1) % 4;
			XMVECTOR point1 = XMLoadFloat3(&XMFLOAT3(vertices[i].x, vertices[i].y, 0.0f));
			XMVECTOR point2 = XMLoadFloat3(&XMFLOAT3(vertices[j].x, vertices[j].y, 0.0f));
			float projection;
			XMStoreFloat(&projection, XMVector3Dot(point2 - point1, camera - point1) / XMVector3LengthSq(point2 - point1));
			float distance;
			if (projection < 0.0f || projection > 1.0f)
			{
				float distance1, distance2;
				XMStoreFloat(&distance1, XMVector3Length(camera - point1));
				XMStoreFloat(&distance2, XMVector3Length(camera - point2));
				distance = fminf(distance1, distance2);
				insidePlane = false;
			}
			else
			{
				XMStoreFloat(&distance, XMVector3LinePointDistance(point1, point2, camera));
			}
			if (distance < range)
				return true;
		}
		if (insidePlane)
		{
			XMVECTOR point1 = XMLoadFloat3(&XMFLOAT3(vertices[0].x, vertices[0].y, 0.0f));
			XMVECTOR point2 = XMLoadFloat3(&XMFLOAT3(vertices[1].x, vertices[1].y, 0.0f));
			XMVECTOR point3 = XMLoadFloat3(&XMFLOAT3(vertices[2].x, vertices[2].y, 0.0f));
			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(point3 - point2, point2 - point1));
			float distance;
			XMStoreFloat(&distance, XMVector3Dot(camera - point1, normal));
			if (fabsf(distance) < range)
				return true;
		}
		return false;
	}

	void Water::Children(Node node, Node* children)
	{
		static float dx[] = { -0.25f, 0.25f, 0.25f, -0.25f };
		static float dy[] = { -0.25f, -0.25f, 0.25f, 0.25f };
		for (int i = 0; i < 4; i++)
		{
			children[i] = Node(XMFLOAT2(node.Position.x + node.Size * dx[i], node.Position.y + node.Size * dy[i]), node.Size / 2.0f);
		}
	}

	void Water::Vertices(Node node, XMFLOAT2* vertices)
	{
		static float dx[] = { -0.5f, 0.5f, 0.5f, -0.5f };
		static float dy[] = { -0.5f, -0.5f, 0.5f, 0.5f };
		for (int i = 0; i < 4; i++)
		{
			vertices[i] = XMFLOAT2(node.Position.x + node.Size * dx[i], node.Position.y + node.Size * dy[i]);
		}
	}
}