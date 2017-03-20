#include "GameException.h"

using namespace std;

namespace Library
{
	GameException::GameException(const char* const& message, HRESULT hResult) : exception(message), hResult(hResult) {}

	HRESULT GameException::HResult() const
	{
		return hResult;
	}

	wstring GameException::WhatW() const
	{
		string whatString(what());
		wstring whatW;
		whatW.assign(whatString.begin(), whatString.end());
		return whatW;
	}
}