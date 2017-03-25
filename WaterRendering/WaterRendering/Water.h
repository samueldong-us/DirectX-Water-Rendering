#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"
#include "Effect.h"
#include "KeyboardComponent.h"

using namespace Library;

namespace Rendering
{
	class Water : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Water, DrawableGameComponent)
			
	public:
		Water(Game& game, Camera& camera);
		~Water();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		static const int Levels;
		static const int Resolution;
		static const float MaximumDistance;

		static const int WaveLevels;
		static const int NumberOfWavesPerLevel;
		static const float MaximumWaveLength;
		static const XMFLOAT2 WindDirection;
	
	private:
		struct Node
		{
			XMFLOAT2 Position;
			FLOAT Size;

			Node() {}
			Node(XMFLOAT2 position, FLOAT size) : Position(position), Size(size) {}
		};

		struct WaterVertex
		{
			XMFLOAT4 Position;
			XMFLOAT2 FactorRange;

			WaterVertex() {}
			WaterVertex(XMFLOAT4 position, XMFLOAT2 factorRange) : Position(position), FactorRange(factorRange) {}
		};

		struct GerstnerWave
		{
			XMFLOAT2 Direction;
			FLOAT Frequency;
			FLOAT Amplitude;
			FLOAT PhaseSpeed;
			XMFLOAT3 _Padding;
			
			GerstnerWave() {}
			GerstnerWave(XMFLOAT2 direction, FLOAT frequency, FLOAT amplitude, FLOAT phaseSpeed) :
				Direction(direction), Frequency(frequency), Amplitude(amplitude), PhaseSpeed(phaseSpeed) {}
		};

		std::vector<Node> nodes;
		std::vector<GerstnerWave> waves;
		float roughness;
		bool wireframe;
		int outputSize;
		Effect* effect;
		ID3D11ShaderResourceView* environmentMap;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* vertexBuffer;

		KeyboardComponent* keyboard;

		void GenerateWaves();

		bool SelectNodes(Node node, float range, int level, std::vector<WaterVertex>& output);
		void AddNode(Node node, float factor, float range, std::vector<WaterVertex>& output);
		bool InRange(Node node, float range);
		void Children(Node node, Node* children);
		void Vertices(Node node, XMFLOAT2* vertices);
	};
}