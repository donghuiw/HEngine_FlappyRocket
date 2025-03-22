#include "hepch.h"
#include "FileSystem.h"

namespace HEngine
{
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);		//以二进制模式打开文件,并打开文件后立即将文件指针移到文件末尾

		if (!stream)
		{
			//Failed to open the file
			return {};
		}

		std::streampos end = stream.tellg();	//返回当前流位置指针文件末尾
		stream.seekg(0, std::ios::beg);		//将文件指针移动到文件开头
		uint64_t size = end - stream.tellg();	// 计算出文件的大小

		if (size == 0)
		{
			//File is empty
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);			//从文件流中读取 size 字节的数据到 buffer 中
		stream.close();
		return buffer;
	}
}