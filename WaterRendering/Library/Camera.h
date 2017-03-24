#pragma once

#include "GameComponent.h"

namespace Library
{
	class Camera : public GameComponent
	{
		RTTI_DECLARATIONS(Camera, GameComponent)

	public:
		Camera(Game& game);
		Camera(Game& game, float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

		virtual ~Camera();

		const XMFLOAT3& Position() const;
		const XMFLOAT3& Direction() const;
		const XMFLOAT3& Up() const;
		const XMFLOAT3& Right() const;

		XMVECTOR PositionVector() const;
		XMVECTOR DirectionVector() const;
		XMVECTOR UpVector() const;
		XMVECTOR RightVector() const;

		float FieldOfView() const;
		float AspectRatio() const;
		float NearPlane() const;
		float FarPlane() const;

		XMMATRIX ViewMatrix() const;
		XMMATRIX ProjectionMatrix() const;
		XMMATRIX ViewProjectionMatrix() const;

		virtual void SetPosition(FLOAT x, FLOAT y, FLOAT z);
		virtual void SetPosition(FXMVECTOR position);
		virtual void SetPosition(const XMFLOAT3& position);

		virtual void Reset();
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrx();
		void ApplyRotation(CXMMATRIX transform);
		void ApplyRotation(const XMFLOAT4X4& transform);

		static const float DefaultFieldOfView;
		static const float DefaultAspectRatio;
		static const float DefaultNearPlane;
		static const float DefaultFarPlane;

	protected:
		float fieldOfView;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		XMFLOAT3 position;
		XMFLOAT3 direction;
		XMFLOAT3 up;
		XMFLOAT3 right;

		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
	};
}