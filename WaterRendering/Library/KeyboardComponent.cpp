#include "KeyboardComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(KeyboardComponent)

	KeyboardComponent::KeyboardComponent(Game& game, LPDIRECTINPUT8 directInput) : GameComponent(game), directInput(directInput), device(nullptr)
	{
		assert(directInput != nullptr);
		ZeroMemory(currentState, sizeof(currentState));
		ZeroMemory(lastState, sizeof(lastState));
	}

	KeyboardComponent::~KeyboardComponent()
	{
		if (device != nullptr)
		{
			device->Unacquire();
			device->Release();
			device = nullptr;
		}
	}

	const byte* const KeyboardComponent::CurrentState() const
	{
		return currentState;
	}

	const byte* const KeyboardComponent::LastState() const
	{
		return lastState;
	}

	void KeyboardComponent::Initialize()
	{
		if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &device, nullptr)))
		{
			throw GameException("IDIRECTINPUT8::CreateDevice() failed.");
		}
		if (FAILED(device->SetDataFormat(&c_dfDIKeyboard)))
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

	void KeyboardComponent::Update(const GameTime& gameTime)
	{
		if (device != nullptr)
		{
			memcpy(lastState, currentState, sizeof(currentState));
			if (FAILED(device->GetDeviceState(sizeof(currentState), (LPVOID)currentState)))
			{
				if (SUCCEEDED(device->Acquire()))
				{
					device->GetDeviceState(sizeof(currentState), (LPVOID)currentState);
				}
			}
		}
	}

	bool KeyboardComponent::IsKeyUp(byte key) const
	{
		return (currentState[key] & 0x80) == 0;
	}

	bool KeyboardComponent::IsKeyDown(byte key) const
	{
		return (currentState[key] & 0x80) != 0;
	}

	bool KeyboardComponent::WasKeyUp(byte key) const
	{
		return (lastState[key] & 0x80) == 0;
	}

	bool KeyboardComponent::WasKeyDown(byte key) const
	{
		return (lastState[key] & 0x80) != 0;
	}

	bool KeyboardComponent::WasKeyPressedThisFrame(byte key) const
	{
		return IsKeyDown(key) && WasKeyUp(key);
	}

	bool KeyboardComponent::WasKeyReleasedThisFrame(byte key) const
	{
		return IsKeyUp(key) && WasKeyDown(key);
	}

	bool KeyboardComponent::IsKeyHeldDown(byte key) const
	{
		return IsKeyDown(key) && WasKeyDown(key);
	}
}