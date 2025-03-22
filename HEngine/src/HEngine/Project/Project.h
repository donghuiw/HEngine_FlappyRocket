#pragma once

#include <string>
#include <filesystem>

#include "HEngine/Core/Base.h"

namespace HEngine
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;					//存储起始场景的路径

		std::filesystem::path AssetDirectory;			//存储资产文件夹的路径
		std::filesystem::path ScriptModulePath;		//存储脚本某块的路径
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()		//返回当前活动项目的目录路径
		{
			HE_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()	//返回资源文件夹的完整路径
		{
			HE_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		//TODO(Yan): move to asset manager when we have one
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)		//返回一个完整的文件系统路径
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
		std::filesystem::path m_ProjectDirectory;		//根目录路径

		inline static Ref<Project> s_ActiveProject;		//活动的项目
	};
}