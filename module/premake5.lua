
local vcpkg_dir = false, false

if os.istarget("windows") then
    newoption {
        trigger = "vcpkg-dir",
        description = "Directory path to the vcpkg root directory"
    }

    vcpkg_dir = _OPTIONS["vcpkg-dir"]
end

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

    defines { "LUA_BUILD_AS_DLL" }

    includedirs {
        "lua", 
        "../vendor/websocketpp/include",
        "../vendor/sol2/include",
    }

    if os.istarget("windows") and vcpkg_dir and os.isdir(vcpkg_dir) then
        local installed_dir = path.join(vcpkg_dir, "installed")

        local function links_boost(names)
            for key, name in pairs(names) do
                filter { "platforms:x86", "configurations:Debug" }
                    libdirs(path.join(installed_dir, "x86-windows-static", "debug", "lib"))
                    links { "boost_".. name .."-vc140-mt-gd.lib" }

                filter { "platforms:x64", "configurations:Debug" }
                    libdirs(path.join(installed_dir, "x64-windows-static", "debug", "lib"))
                    links { "boost_".. name .."-vc140-mt-gd.lib" }

                filter { "platforms:x86", "configurations:Release" }
                    libdirs(path.join(installed_dir, "x86-windows-static", "lib"))
                    links { "boost_".. name .."-vc140-mt.lib" }

                filter { "platforms:x64", "configurations:Release" }
                    libdirs(path.join(installed_dir, "x64-windows-static", "lib"))
                    links { "boost_".. name .."-vc140-mt.lib" }
            end
        end

        filter { "platforms:x86" }
            includedirs(path.join(installed_dir, "x86-windows-static", "include"))

        filter { "platforms:x64" }
            includedirs(path.join(installed_dir, "x64-windows-static", "include"))

        links_boost { "system", "regex" }
    end

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
