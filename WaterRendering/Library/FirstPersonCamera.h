#pragma once

#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "Camera.h"

namespace Library
{
	class FirstPersonCamera : public Camera
	{
		RTTI_DECLARATIONS(FirstPersonCamera, Camera)

	public:
		FirstPersonCamera(Game& game);
		FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		virtual ~FirstPersonCamera();

		float& MouseSensitivity();
		float& RotationRate();
		float& MovementRate();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;
		static const float DefaultRotationRate;
		static const float DefaultMovementRate;

	protected:
		float mouseSensitivity;
		float rotationRate;
		float movementRate;

		KeyboardComponent* keyboard;
		MouseComponent* mouse;
	};
}