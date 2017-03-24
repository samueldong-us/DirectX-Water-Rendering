#include "FirstPersonCamera.h"

namespace Library
{
	RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float FirstPersonCamera::DefaultMovementRate = 10.0f;
	const float FirstPersonCamera::DefaultMouseSensitivity = 100.0f;

	FirstPersonCamera::FirstPersonCamera(Game& game) :
		Camera(game), mouseSensitivity(DefaultMouseSensitivity), rotationRate(DefaultRotationRate), movementRate(DefaultMovementRate),
		keyboard(nullptr), mouse(nullptr) {}

	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlane, float farPlane) :
		Camera(game, fieldOfView, aspectRatio, nearPlane, farPlane), 
		mouseSensitivity(DefaultMouseSensitivity), rotationRate(DefaultRotationRate), movementRate(DefaultMovementRate),
		keyboard(nullptr), mouse(nullptr) {}

	FirstPersonCamera::~FirstPersonCamera() {}

	float& FirstPersonCamera::MouseSensitivity()
	{
		return mouseSensitivity;
	}

	float& FirstPersonCamera::RotationRate()
	{
		return rotationRate;
	}

	float& FirstPersonCamera::MovementRate()
	{
		return movementRate;
	}

	void FirstPersonCamera::Initialize()
	{
		keyboard = game->Services().GetService<KeyboardComponent>();
		mouse = game->Services().GetService<MouseComponent>();
		Camera::Initialize();
	}

	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		XMFLOAT2 movementAmount = XMFLOAT2(0.0f, 0.0f);
		if (keyboard != nullptr)
		{
			if (keyboard->IsKeyDown(DIK_W))
				movementAmount.y += 1.0f;
			if (keyboard->IsKeyDown(DIK_S))
				movementAmount.y -= 1.0f;
			if (keyboard->IsKeyDown(DIK_D))
				movementAmount.x += 1.0f;
			if (keyboard->IsKeyDown(DIK_A))
				movementAmount.x -= 1.0f;
		}

		XMFLOAT2 rotationAmount = XMFLOAT2(0.0f, 0.0f);
		if (mouse != nullptr && mouse->IsButtonHeldDown(MouseLeftButton))
		{
			LPDIMOUSESTATE mouseState = mouse->CurrentState();
			rotationAmount.x = -mouseState->lX * mouseSensitivity;
			rotationAmount.y = -mouseState->lY * mouseSensitivity;
		}

		float deltaTime = (float)gameTime.ElapsedGameTime();
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * rotationRate * deltaTime;
		XMVECTOR right = XMLoadFloat3(&this->right);
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));
		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));

		XMVECTOR position = XMLoadFloat3(&this->position);
		XMVECTOR movement = XMLoadFloat2(&movementAmount) * movementRate * deltaTime;
		XMVECTOR strafe = right * XMVectorGetX(movement);
		position += strafe;
		XMVECTOR forward = XMLoadFloat3(&direction) * XMVectorGetY(movement);
		position += forward;
		SetPosition(position);

		Camera::Update(gameTime);
	}
}