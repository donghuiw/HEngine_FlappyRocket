local HEngineRootDir = '../../../..'
include (HEngineRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "FlappyRocket"
	architecture "x86_64"
	startproject "FlappyRocket"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FlappyRocket"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	links
	{
		"HEngine-ScriptCore"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "HEngine"
	include (HEngineRootDir .. "/HEngine-ScriptCore")
group ""
