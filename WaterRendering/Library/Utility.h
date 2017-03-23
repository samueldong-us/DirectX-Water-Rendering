#pragma once

#include <algorithm>
#include <Shlwapi.h>

#include "Common.h"

namespace Library
{
	class Utility
	{
	public:
		static std::string CurrentDirectory()
		{
			WCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);
			std::wstring currentDirectory(buffer);
			return std::string(currentDirectory.begin(), currentDirectory.end());
		}

		static std::wstring ExecutableDirectory()
		{
			WCHAR buffer[MAX_PATH];
			GetModuleFileName(nullptr, buffer, MAX_PATH);
			PathRemoveFileSpec(buffer);
			return std::wstring(buffer);
		}

		static void GetFileName(const std::string& inputPath, std::string& filename)
		{
			std::string fullPath(inputPath);
			std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
			std::string::size_type indexOfLastSlash = fullPath.find_last_of('/');
			if (indexOfLastSlash == std::string::npos)
			{
				filename = fullPath;
			}
			else
			{
				filename = fullPath.substr(indexOfLastSlash + 1, fullPath.size() - indexOfLastSlash - 1);
			}
		}

		static void GetDirectory(const std::string& inputPath, std::string& directory)
		{
			std::string fullPath(inputPath);
			std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
			std::string::size_type indexOfLastSlash = fullPath.find_last_of('/');
			if (indexOfLastSlash == std::string::npos)
			{
				directory = "";
			}
			else
			{
				directory = fullPath.substr(0, indexOfLastSlash);
			}
		}

		static void GetFileNameAndDirectory(const std::string& inputPath, std::string& directory, std::string& filename)
		{
			GetDirectory(inputPath, directory);
			GetFileName(inputPath, filename);
		}
	};
}