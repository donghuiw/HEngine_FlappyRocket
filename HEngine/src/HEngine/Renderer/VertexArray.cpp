#include "hepch.h"
#include "HEngine/Renderer/VertexArray.h"

#include "HEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace HEngine
{
	Ref<VertexArray> VertexArray::Create()
	{	
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:  HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}
		HE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}