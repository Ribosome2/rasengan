-- premake5.lua
workspace "Rasengan"
   configurations { "Debug", "Release" }

project "Rasengan"
   architecture ("x64")
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
	"src/**.h", 
	"src/**.cpp", 
	"src/**.hpp" ,
   "ThirdParty/glfw/*.h"
	}


   includedirs{
      "ThirdParty/glfw/include",
      "ThirdParty/include",
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