#pragma once

#include "HEngine/Core/PlatformDetection.h"

#include <memory>

#ifdef HE_DEBUG
#if defined(HE_PLATFORM_WINDOWS)
#define HE_DEBUGBREAK() __debugbreak()
#elif defined(HE_PLATFORM_LINUX)
#include <signal.h>
#define HE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define HE_ENABLE_ASSERTS
#else
#define HE_DEBUGBREAK()
#endif

#define HE_EXPAND_MACRO(x) x
#define HE_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define HE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace HEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
#include "HEngine/Core/Log.h"
#include "HEngine/Core/Assert.h"
