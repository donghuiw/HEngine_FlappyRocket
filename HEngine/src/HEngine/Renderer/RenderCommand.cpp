#include "hepch.h"
#include "HEngine/Renderer/RenderCommand.h"


namespace HEngine
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}