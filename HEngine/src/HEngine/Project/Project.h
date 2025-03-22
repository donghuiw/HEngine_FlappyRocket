#pragma once

#include <string>
#include <filesystem>

#include "HEngine/Core/Base.h"

namespace HEngine
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;					//�洢��ʼ������·��

		std::filesystem::path AssetDirectory;			//�洢�ʲ��ļ��е�·��
		std::filesystem::path ScriptModulePath;		//�洢�ű�ĳ���·��
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()		//���ص�ǰ���Ŀ��Ŀ¼·��
		{
			HE_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()	//������Դ�ļ��е�����·��
		{
			HE_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		//TODO(Yan): move to asset manager when we have one
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)		//����һ���������ļ�ϵͳ·��
		{
			HE_CORE_ASSERT(s_ActiveProject);
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;		//��Ŀ¼·��

		inline static Ref<Project> s_ActiveProject;		//�����Ŀ
	};
}