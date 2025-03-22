#pragma once

#include "HEngine/Core/Buffer.h"
#include <filesystem>

namespace HEngine
{
	class FileSystem
	{
	public:
		//TODO: move to FileSystem class
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}