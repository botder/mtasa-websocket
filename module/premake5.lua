
newoption {
    trigger = "vcpkg-dir",
    description = "Directory path to the vcpkg root directory"
}

local vcpkg = require "vcpkg"
vcpkg.triplets("x86", { "windows-static" })
vcpkg.triplets("x64", { "windows-static" })
vcpkg.set_debug "Debug"
vcpkg.set_release "Release"
vcpkg.links { "libeay32.lib", "ssleay32.lib" }
vcpkg.links_boost { "system", "regex", "random" }
vcpkg.links_debug { "zlibd.lib" }
vcpkg.links_release { "zlib.lib" }

if _OPTIONS["vcpkg-dir"] then
    vcpkg.set_root(_OPTIONS["vcpkg-dir"])
end

project "Module"
    language "C++"
    kind "SharedLib"
    targetname "websocket"
    targetprefix "ml_"

    vpaths {
        ["*"]   = { "premake5.lua", "vcpkg.lua" },
        ["PCH"] = { "stdafx.h", "stdafx.cpp" },
        ["MTA"] = { "src/ILuaModuleManager.h", "src/lua.hpp", "src/luaimports.cpp", "src/luaimports.h" },
        ["Code"] = { 
            "src/Module.cpp",           "src/Module.h",
            "src/Resource.cpp",         "src/Resource.h",
            "src/ResourceManager.cpp",  "src/ResourceManager.h",
            "src/WebsocketManager.cpp",  "src/WebsocketManager.h",
            "src/LuaWebsocket.cpp",     "src/LuaWebsocket.h",
            "src/Websocket.cpp",        "src/Websocket.h",
            "src/EndpointManager.cpp",  "src/EndpointManager.h",
            "src/ElementID.h",
        }
    }

    pchheader "stdafx.h"
    pchsource "stdafx.cpp"

    files {
        "vcpkg.lua",
        "premake5.lua",
        "stdafx.h",
        "stdafx.cpp",
        "src/**.cpp",
        "src/**.h",
        "src/**.hpp",
    }

    defines {
        "LUA_BUILD_AS_DLL",
        "_WEBSOCKETPP_CPP11_FUNCTIONAL_",
        "_WEBSOCKETPP_CPP11_RANDOM_DEVICE_",
        "_WEBSOCKETPP_CPP11_SYSTEM_ERROR_",
        "_WEBSOCKETPP_CPP11_MEMORY_",
        "_WEBSOCKETPP_CPP11_STL_",
    }

    includedirs {
        ".",
        "src", 
        "../vendor/lua/include",
    }

    vcpkg.apply()

    filter { "system:windows", "platforms:x86" }
        links { "../vendor/lua/lib/lua5.1.lib" }

    filter { "system:windows", "platforms:x64" }
        links { "../vendor/lua/lib/lua5.1_64.lib" }

    filter "system:not linux"
        excludes { "src/luaimports.cpp" }

    filter "platforms:x86"
        binpath "x32"

    filter "platforms:x64"
        binpath "x64"
