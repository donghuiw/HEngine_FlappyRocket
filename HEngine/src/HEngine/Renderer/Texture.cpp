#include "hepch.h"
#include "HEngine/Renderer/Texture.h"

#include "HEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace HEngine 
{
	Ref<HEngine::Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(specification);
		}
		HE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<HEngine::Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
		}
		HE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}