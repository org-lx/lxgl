workspace "lxgl"
   architecture "x86_64"
   configurations { "Debug", "Release"}

include "lx"

project "lxgl"
   kind "ConsoleApp"
   language "C"
   cdialect "c99"
   targetdir "bin"
   includedirs { "include", "lx/lxxml/include" }
   files { "src/**.c", "include/**.h" }
   
   links { "lxxml", "curl" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
