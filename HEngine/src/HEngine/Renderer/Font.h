#pragma once

#include <filesystem>

#include "HEngine/Core/Base.h"
#include "HEngine/Renderer/Texture.h"

namespace HEngine
{
	struct MSDFData;
	
	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}