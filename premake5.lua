-- Create helper function for output
cwd = os.getcwd()
binpath = function (path) targetdir(os.realpath(("%s/bin/%s"):format(cwd, path))) end

solution "Websocket"
    configurations { "Debug", "Release" }
    platforms { "x32", "x64" }
    location "build"

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
    }

    filter "platforms:x32"
        architecture "x32"

    filter "platforms:x64"
        architecture "x64"

    flags { "C++14" }
    pic "On"
    symbols "On"
    
    filter "system:windows"
        defines { "WIN32", "_WIN32" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "C++14" }
        targetsuffix "_d"

    filter "configurations:Release"
        defines { "NDEBUG" }
        flags { "C++14", "Optimize" }
        optimize "On"

    include "module"
