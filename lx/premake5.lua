project "lxxml"
   kind "StaticLib"
   language "C"
   cdialect "c99"
   targetdir "../bin"
   includedirs { "lxxml/include" }
   files { "lxxml/src/**.c", "lxxml/include/**.h" }
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
