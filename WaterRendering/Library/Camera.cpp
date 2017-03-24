#include "Camera.h"

namespace Library
{
	RTTI_DEFINITIONS(Camera)

	const float Camera::DefaultFieldOfView = XM_PIDIV4;
	const float Camera::DefaultNearPlane = 0.01f;
	const float Camera::DefaultFarPlane = 1000.0f;

	Camera::Camera(Game& game) : GameComponent(game), fieldOfView(DefaultFieldOfView), aspectRatio(game.AspectRatio()),
		nearPlane(DefaultNearPlane), farPlane(DefaultFarPlane), position(), direction(), up(), right(), viewMatrix(), projectionMatrix() {}

	Camera::Camera(Game & game, float fieldOfView, float aspectRatio, float nearPlane, float farPlane) :
		GameComponent(game), fieldOfView(fieldOfView), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), 
		position(), direction(), up(), right(), viewMatrix(), projectionMatrix() {}

	Camera::~Camera() {}

	const XMFLOAT3& Camera::Position() const
	{
		return position;
	}

	const XMFLOAT3& Camera::Direction() const
	{
		return direction;
	}

	const XMFLOAT3& Camera::Up() const
	{
		return up;
	}

	const XMFLOAT3& Camera::Right() const
	{
		return right;
	}

	XMVECTOR Camera::PositionVector() const
	{
		return XMLoadFloat3(&position);
	}

	XMVECTOR Camera::DirectionVector() const
	{
		return XMLoadFloat3(&direction);
	}

	XMVECTOR Camera::UpVector() const
	{
		return XMLoadFloat3(&up);
	}

	XMVECTOR Camera::RightVector() const
	{
		return XMLoadFloat3(&right);
	}

	float Camera::FieldOfView() const
	{
		return fieldOfView;
	}

	float Camera::AspectRatio() const
	{
		return aspectRatio;
	}

	float Camera::NearPlane() const
	{
		return nearPlane;
	}

	float Camera::FarPlane() const
	{
		return farPlane;
	}
	
	XMMATRIX Camera::ViewMatrix() const
	{
		return XMLoadFloat4x4(&viewMatrix);
	}

	XMMATRIX Camera::ProjectionMatrix() const
	{
		return XMLoadFloat4x4(&projectionMatrix);
	}

	XMMATRIX Camera::ViewProjectionMatrix() const
	{
		return XMMatrixMultiply(ViewMatrix(), ProjectionMatrix());
	}

	void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		SetPosition(XMVectorSet(x, y, z, 1.0f));
	}

	void Camera::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&this->position, position);
	}

	void Camera::SetPosition(const XMFLOAT3& position)
	{
		this->position = position;
	}

	void Camera::Reset()
	{
		position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		direction = XMFLOAT3(0.0f, 1.0f, 0.0f);
		up = XMFLOAT3(0.0f, 0.0f, 1.0f);
		right = XMFLOAT3(1.0f, 0.0f, 0.0f);

		UpdateViewMatrix();
	}

	void Camera::Initialize()
	{
		UpdateProjectionMatrx();
		Reset();
	}

	void Camera::Update(const GameTime& gameTime)
	{
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		XMVECTOR position = XMLoadFloat3(&this->position);
		XMVECTOR direction = XMLoadFloat3(&this->direction);
		XMVECTOR up = XMLoadFloat3(&this->up);

		XMMATRIX viewMatrix = XMMatrixLookToRH(position, direction, up);
		XMStoreFloat4x4(&this->viewMatrix, viewMatrix);
	}

	void Camera::UpdateProjectionMatrx()
	{
		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(fieldOfView, aspectRatio, nearPlane, farPlane);
		XMStoreFloat4x4(&this->projectionMatrix, projectionMatrix);
	}

	void Camera::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&this->direction);
		XMVECTOR up = XMLoadFloat3(&this->up);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&this->direction, direction);
		XMStoreFloat3(&this->up, up);
		XMStoreFloat3(&this->right, right);
	}

	void Camera::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transform);
	}
}