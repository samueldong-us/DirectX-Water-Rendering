#pragma once

#include <exception>
#include <Windows.h>
#include <string>

namespace Library
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* const& message, HRESULT hResult = S_OK);

		HRESULT HResult() const;
		std::wstring WhatW() const;

	private:
		HRESULT hResult;
	};
}