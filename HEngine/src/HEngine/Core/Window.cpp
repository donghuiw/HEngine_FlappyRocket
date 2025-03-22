#include "hepch.h"
#include "HEngine/Core/Window.h"

#ifdef HE_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace HEngine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef HE_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		HE_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}