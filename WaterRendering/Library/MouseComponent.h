#pragma once

#include <dinput.h>

#include "GameComponent.h"

namespace Library
{
	enum MouseButton
	{
		MouseLeftButton = 0,
		MouseRightButton = 1,
		MouseMiddleButton = 2,
		MouseX1Button = 3
	};

	class MouseComponent : public GameComponent
	{
		RTTI_DECLARATIONS(MouseComponent, GameComponent)

	public:
		MouseComponent(Game& game, LPDIRECTINPUT8 directInput);
		~MouseComponent();

		LPDIMOUSESTATE CurrentState();
		LPDIMOUSESTATE LastState();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		long X() const;
		long Y() const;
		long Wheel() const;

		bool IsButtonUp(MouseButton button) const;
		bool IsButtonDown(MouseButton button) const;
		bool WasButtonUp(MouseButton button) const;
		bool WasButtonDown(MouseButton button) const;
		bool WasButtonPressedThisFrame(MouseButton button) const;
		bool WasButtonReleasedThisFrame(MouseButton button) const;
		bool IsButtonHeldDown(MouseButton button) const;

	private:
		LPDIRECTINPUT8 directInput;
		LPDIRECTINPUTDEVICE8 device;
		DIMOUSESTATE currentState;
		DIMOUSESTATE lastState;

		long x;
		long y;
		long wheel;
	};
}