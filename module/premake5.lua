project "Module"
    language "C++"
    kind "SharedLib"
    targetname "websocket"
    targetprefix "ml_"

    vpaths {
        ["Headers"] = "src/**.h",
        ["Sources"] = "src/**.cpp",
        ["Lua"] = { "lua/**.cpp", "lua/**.h" },
    }

    files {
        "**.h",
        "**.cpp",
    }

    includedirs {
        "lua", 
        "../vendor/websocketpp/include",
    }

    filter "system:windows"
        excludes { "lua/luaimports.cpp", "lua/luaimports.h" }
        defines { "LUA_BUILD_AS_DLL" }

    -- Link to platform and configuration dependent lua5.1 library 
    filter { "system:windows", "platforms:x32", "configurations:Release" }
        links { "libs/x32/lua5.1" }

    filter { "system:windows", "platforms:x32", "configurations:Debug" }
        links { "libs/x32/lua5.1_d" }

    filter { "system:windows", "platforms:x64", "configurations:Release" }
        links { "libs/x64/lua5.1" }

    filter { "system:windows", "platforms:x64", "configurations:Debug" }
        links { "libs/x64/lua5.1_d" }

    -- Set platform dependent target directory
    filter "platforms:x32"
        binpath "x32"

    filter "platforms:x64"
        binpath "x64"
