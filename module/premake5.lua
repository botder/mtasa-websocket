project "Module"
    language "C++"
    kind "SharedLib"
    targetname "websocket"
    targetprefix "ml_"

    vpaths {
        ["Headers"] = "src/**.h",
        ["Sources"] = "src/**.cpp",
        ["Lua"] = { "lua/**.cpp", "lua/**.h" },
        ["*"] = "premake5.lua",
    }

    files {
        "premake5.lua",
        "lua/**.h",
        "lua/**c.pp",
        "src/**.h",
        "src/**.cpp",
    }

    includedirs {
        "lua", 
        "../vendor/websocketpp/include",
    }

    filter { "system:windows", "platforms:x86" }
        links { "lib/lua5.1.lib" }
        
    filter { "system:windows", "platforms:x64" }
        links { "lib/lua5.1_64.lib" }

    filter "system:not linux"
        excludes { "lua/luaimports.cpp" }

    filter "platforms:x32"
        binpath "x32"

    filter "platforms:x64"
        binpath "x64"
