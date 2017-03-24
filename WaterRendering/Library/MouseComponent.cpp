#include "MouseComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(MouseComponent)

	MouseComponent::MouseComponent(Game& game, LPDIRECTINPUT8 directInput) :
		GameComponent(game), directInput(directInput), device(nullptr), x(), y(), wheel()
	{
		assert(directInput != nullptr);
		ZeroMemory(&currentState, sizeof(currentState));
		ZeroMemory(&lastState, sizeof(lastState));
	}

	MouseComponent::~MouseComponent()
	{
		if (device != nullptr)
		{
			device->Unacquire();
			device->Release();
			device = nullptr;
		}
	}

	LPDIMOUSESTATE MouseComponent::CurrentState()
	{
		return &currentState;
	}

	LPDIMOUSESTATE MouseComponent::LastState()
	{
		return &lastState;
	}

	long MouseComponent::X() const
	{
		return x;
	}

	long MouseComponent::Y() const
	{
		return y;
	}

	long MouseComponent::Wheel() const
	{
		return wheel;
	}

	void MouseComponent::Initialize()
	{
		if (FAILED(directInput->CreateDevice(GUID_SysMouse, &device, nullptr)))
		{
			throw GameException("IDIRECTINPUT8::CreateDevice() failed.");
		}
		if (FAILED(device->SetDataFormat(&c_dfDIMouse)))
		{
			throw GameException("IDIRECTINPUTDEVICE8::SetDataFormat() failed.");
		}
		if (FAILED(device->SetCooperativeLevel(game->WindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			throw GameException("IDIRECTINPUTDEVICE8::SetCooperativeLevel() failed.");
		}
		if (FAILED(device->Acquire()))
		{
			throw GameException("IDIRECTINPUTDEVICE8::Acquire() failed.");
		}
	}

	void MouseComponent::Update(const GameTime& gameTime)
	{
		if (device != nullptr)
		{
			memcpy(&lastState, &currentState, sizeof(currentState));

			if (FAILED(device->GetDeviceState(sizeof(currentState), &currentState)))
			{
				if (SUCCEEDED(device->Acquire()))
				{
					if (FAILED(device->GetDeviceState(sizeof(currentState), &currentState)))
					{
						return;
					}
				}
			}

			x += currentState.lX;
			y += currentState.lY;
			wheel += currentState.lZ;
		}
	}

	bool MouseComponent::IsButtonUp(MouseButton button) const
	{
		return (currentState.rgbButtons[button] & 0x80) == 0;
	}

	bool MouseComponent::IsButtonDown(MouseButton button) const
	{
		return (currentState.rgbButtons[button] & 0x80) != 0;
	}

	bool MouseComponent::WasButtonUp(MouseButton button) const
	{
		return (lastState.rgbButtons[button] & 0x80) == 0;
	}

	bool MouseComponent::WasButtonDown(MouseButton button) const
	{
		return (lastState.rgbButtons[button] & 0x80) != 0;
	}

	bool MouseComponent::WasButtonPressedThisFrame(MouseButton button) const
	{
		return IsButtonDown(button) && WasButtonUp(button);
	}

	bool MouseComponent::WasButtonReleasedThisFrame(MouseButton button) const
	{
		return IsButtonUp(button) && WasButtonDown(button);
	}

	bool MouseComponent::IsButtonHeldDown(MouseButton button) const
	{
		return IsButtonDown(button) && WasButtonDown(button);
	}
}