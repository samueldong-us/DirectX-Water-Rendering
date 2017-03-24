#pragma once

#include <dinput.h>

#include "GameComponent.h"

namespace Library
{
	class KeyboardComponent : public GameComponent
	{
		RTTI_DECLARATIONS(KeyboardComponent, GameComponent)

	public:
		KeyboardComponent(Game& game, LPDIRECTINPUT8 directInput);
		~KeyboardComponent();

		const byte* const CurrentState() const;
		const byte* const LastState() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		bool IsKeyUp(byte key) const;
		bool IsKeyDown(byte key) const;
		bool WasKeyUp(byte key) const;
		bool WasKeyDown(byte key) const;
		bool WasKeyPressedThisFrame(byte key) const;
		bool WasKeyReleasedThisFrame(byte key) const;
		bool IsKeyHeldDown(byte key) const;

	private:
		static const int KeyCount = 256;

		LPDIRECTINPUT8 directInput;
		LPDIRECTINPUTDEVICE8 device;
		byte currentState[KeyCount];
		byte lastState[KeyCount];
	};
}