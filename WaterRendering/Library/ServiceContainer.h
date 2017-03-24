#pragma once

#include "Common.h"

namespace Library
{
	class ServiceContainer
	{
	public:
		template<typename T> void AddService(T* service)
		{
			services[T::TypeIdClass()] = service;
		}

		template<typename T> void RemoveService()
		{
			services.erase(T::TypeIdClass());
		}

		template<typename T> T* GetService() const
		{
			auto iterator = services.find(T::TypeIdClass());
			return iterator == services.end() ? nullptr : (T*)iterator->second;
		}

	private:
		std::map<UINT, void*> services;
	};
}