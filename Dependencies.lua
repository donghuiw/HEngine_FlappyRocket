
-- HEngine Dependencies

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/HEngine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/HEngine/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/HEngine/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/HEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/HEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/HEngine/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/HEngine/vendor/ImGuizmo"
IncludeDir["filewatch"] = "%{wks.location}/HEngine/vendor/filewatch"
IncludeDir["glm"] = "%{wks.location}/HEngine/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/HEngine/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/HEngine/vendor/mono/include"
IncludeDir["shaderc"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Include/shaderc"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Include/spirv_cross"
IncludeDir["VulkanSDK"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Include"
IncludeDir["msdfgen"] = "%{wks.location}/HEngine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/HEngine/vendor/msdf-atlas-gen/msdf-atlas-gen"


LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Lib"
LibraryDir["mono"] = "%{wks.location}/HEngine/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/HEngine/vendor/VulkanSDK/Bin"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
