-- Create helper function for output
cwd = os.getcwd()
binpath = function (path) targetdir(os.realpath(("%s/bin/%s"):format(cwd, path))) end

solution "Websocket"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }
    location "build"

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
    }

    cppdialect "C++14"
    characterset "MBCS"
    pic "On"
    symbols "On"

    filter "platforms:x86"
        architecture "x86"

    filter "platforms:x64"
        architecture "x86_64"
    
    filter "system:windows"
        defines { "WIN32", "_WIN32" }
        flags { "StaticRuntime" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        targetsuffix "_d"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    include "module"
