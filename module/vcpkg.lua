
local module = {}
local vcpkg = {}

vcpkg.links_debug = {}
vcpkg.links_release = {}
vcpkg.platforms = {}

vcpkg.system_map =
{
    ["linux"]          = "linux",
    ["osx"]            = "macosx",
    ["windows"]        = "windows",
    ["windows-static"] = "windows",
}

vcpkg.architecture_map =
{
    ["x86"]   = "x86",
    ["x64"]   = "x86_64",
}

function module.use_default_triplets()
    module.triplets("arm",   { "uwp", "windows" })
    module.triplets("arm64", { "uwp", "windows" })
    module.triplets("x86",   { "linux", "osx", "uwp", "windows", "windows-static" })
    module.triplets("x64",   { "linux", "osx", "uwp", "windows", "windows-static" })
end

function module.set_root(root_path)
    vcpkg.root_dir = root_path
    vcpkg.installed_dir = path.join(root_path, "installed")
end

function module.triplets(platform, systems)
    assert(type(platform) == "string", "expected string at argument 1, got ".. type(platform))
    assert(type(systems) == "table", "expected table at argument 2, got ".. type(systems))

    if not vcpkg.platforms[platform] then
        vcpkg.platforms[platform] = {}
    end

    platform = vcpkg.platforms[platform]

    for index, system in pairs(systems) do
        if type(system) == "string" and system ~= "" then
            platform[system] = true
        end
    end
end

function module.set_debug(name)
    assert(type(name) == "string", "expected string at argument 1, got ".. type(name))
    vcpkg.debug_name = name
end

function module.set_release(name)
    assert(type(name) == "string", "expected string at argument 1, got ".. type(name))
    vcpkg.release_name = name
end

function module.links(names)
    assert(type(names) == "table", "expected table at argument 1, got ".. type(names))
    module.links_debug(names)
    module.links_release(names)
end

function module.links_debug(names)
    assert(type(names) == "table", "expected table at argument 1, got ".. type(names))

    for index, name in pairs(names) do
        if type(name) == "string" and name ~= "" then
            vcpkg.links_debug[name] = true
        end
    end
end

function module.links_release(names)
    assert(type(names) == "table", "expected table at argument 1, got ".. type(names))

    for index, name in pairs(names) do
        if type(name) == "string" and name ~= "" then
            vcpkg.links_release[name] = true
        end
    end
end

function module.links_boost(names)
    assert(type(names) == "table", "expected table at argument 1, got ".. type(names))

    local debug_names = {}
    local release_names = {}

    for index, name in pairs(names) do
        if type(name) == "string" and name ~= "" then
            debug_names[#debug_names + 1] = ("boost_%s-vc140-mt-gd.lib"):format(name)
            release_names[#release_names + 1] = ("boost_%s-vc140-mt.lib"):format(name)
        end
    end

    module.links_debug(debug_names)
    module.links_release(release_names)
end

function module.apply()
    for platform in pairs(vcpkg.platforms) do
        local architecture = "architecture:".. (vcpkg.architecture_map[platform] or platform)

        for system in pairs(vcpkg.platforms[platform]) do
            local triplet = ("%s-%s"):format(platform, system)
            system = vcpkg.system_map[system] or system

            filter({ architecture, "system:".. system })
            includedirs(path.join(vcpkg.installed_dir, triplet, "include"))
        end
    end

    for platform in pairs(vcpkg.platforms) do
        local architecture = "architecture:".. (vcpkg.architecture_map[platform] or platform)
        local debug_configuration = "configurations:".. vcpkg.debug_name
        local release_configuration = "configurations:".. vcpkg.release_name

        for system in pairs(vcpkg.platforms[platform]) do
            local triplet = ("%s-%s"):format(platform, system)
            system = vcpkg.system_map[system] or system

            filter({ architecture, debug_configuration, "system:".. system })
            libdirs(path.join(vcpkg.installed_dir, triplet, "debug", "lib"))

            for name in pairs(vcpkg.links_debug) do
                links(name)
            end

            filter({ architecture, release_configuration, "system:".. system })
            libdirs(path.join(vcpkg.installed_dir, triplet, "lib"))

            for name in pairs(vcpkg.links_release) do
                links(name)
            end
        end
    end
end

return module
