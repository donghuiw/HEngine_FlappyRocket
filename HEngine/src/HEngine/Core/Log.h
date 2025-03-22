#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "HEngine/Core/Base.h"
//This ignores call warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


#include "HEngine/Events/Event.h"
template <> 
struct fmt::formatter<HEngine::Event> : ostream_formatter {};

// 为 glm::vec3 定义格式化器
template <>
struct fmt::formatter<glm::vec3> {
	// 格式化解析（一般不需要特殊处理）
	constexpr auto parse(fmt::format_parse_context& ctx) {
		return ctx.begin();  // 默认不处理其他格式化选项
	}

	// 格式化 glm::vec3 对象为字符串
	template <typename FormatContext>
	auto format(const glm::vec3& vec, FormatContext& ctx) const {
		// 格式化为 (x, y, z)，保留3位小数
		return fmt::format_to(ctx.out(), "({:.3f}, {:.3f}, {:.3f})", vec.x, vec.y, vec.z);
	}
};


namespace HEngine
{
	class  Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

//core log macros
#define HE_CORE_TRACE(...)		 ::HEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HE_CORE_INFO(...)			 ::HEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HE_CORE_WARN(...)		 ::HEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HE_CORE_ERROR(...)		 ::HEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HE_CORE_CRITICAL(...)		 ::HEngine::Log::GetCoreLogger()->critical (__VA_ARGS__)

//client log macros
#define HE_TRACE(...)		 ::HEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HE_INFO(...)		 ::HEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define HE_WARN(...)		 ::HEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HE_ERROR(...)		 ::HEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define HE_CRITICAL(...)		 ::HEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

