#include "hepch.h"
#include "FileSystem.h"

namespace HEngine
{
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);		//�Զ�����ģʽ���ļ�,�����ļ����������ļ�ָ���Ƶ��ļ�ĩβ

		if (!stream)
		{
			//Failed to open the file
			return {};
		}

		std::streampos end = stream.tellg();	//���ص�ǰ��λ��ָ���ļ�ĩβ
		stream.seekg(0, std::ios::beg);		//���ļ�ָ���ƶ����ļ���ͷ
		uint64_t size = end - stream.tellg();	// ������ļ��Ĵ�С

		if (size == 0)
		{
			//File is empty
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);			//���ļ����ж�ȡ size �ֽڵ����ݵ� buffer ��
		stream.close();
		return buffer;
	}
}