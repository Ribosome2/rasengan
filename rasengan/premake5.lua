-- premake5.lua
workspace "Rasengan"
   configurations { "Debug", "Release" }

project "Rasengan"
   architecture ("x64")
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
	"src/**.h", 
	"src/**.cpp", 
	"src/**.hpp" ,
	"src/IMGUI/**.hpp" ,
	"src/IMGUI/**.h" ,
	"src/IMGUI/**.cpp" ,
   "ThirdParty/imgui/*.h",
   "ThirdParty/imgui/*.cpp",
   "ThirdParty/glfw/*.h"
	}


   includedirs{
      "ThirdParty/glfw/include",
      "ThirdParty/include",
      "src",
      "src/IMGUI",
      "ThirdParty/imgui",
      os.getenv("VK_SDK_PATH").."/include",--get environment variable in LUA
   }
   libdirs{
      "ThirdParty/glfw/lib-vc2019",
	  os.getenv("VK_SDK_PATH").."/Lib",--get environment variable in LUA
   }

   links{
      "glfw3","vulkan-1.lib"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"