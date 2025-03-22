#pragma once

#include "HEngine/Core/Base.h"
#include "HEngine/Core/Log.h"

#include <filesystem>

#ifdef HE_ENABLE_ASSERTS

//Assertions implement macros that handle types, check conditions, and messages
#define HE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { \
    if (!(check)) { \
        HE##type##ERROR(msg, __VA_ARGS__); \
        HE_DEBUGBREAK(); \
    } \
}
//Asserts an implementation with a message
#define HE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
// Asserts no message implementation
#define HE_INTERNAL_ASSERT_NO_MSG(type, check) HE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
// Get the correct macro, selected based on the number of parameters passed in
#define HE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define HE_INTERNAL_ASSERT_GET_MACRO(...) HE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HE_INTERNAL_ASSERT_WITH_MSG, HE_INTERNAL_ASSERT_NO_MSG)
// The final assertion macro
#define HE_ASSERT(...) HE_EXPAND_MACRO(HE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define HE_CORE_ASSERT(...) HE_EXPAND_MACRO(HE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else
#define HE_ASSERT(...)
#define HE_CORE_ASSERT(...)
#endif