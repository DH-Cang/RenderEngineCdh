add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"}) -- update compile commands for intellisence

add_rules("mode.debug", "mode.release")

add_defines("_WINDOWS")
add_defines("UNICODE")
add_defines("_UNICODE")
add_defines("NOMINMAX")
add_defines("_CRT_SECURE_NO_WARNINGS")
if (is_mode("release")) then
    set_runtimes("MD")
else
    set_runtimes("MDd")
end

target("RenderEngineCdh")
    set_kind("binary")
    add_files("./*.cpp")
    add_files("./Common/*.cpp")
    add_headerfiles("./*.h")
    add_headerfiles("./Common/*.h")

    -- add_files("./Engine/*.cpp")
    -- add_headerfiles("./Engine/*.h")

    add_files("./Utility/*.cpp")
    add_headerfiles("./Utility/*.h")

    add_files("D3DRHI/*.cpp")
    add_headerfiles("D3DRHI/*.h")

    -- search include file in current dir
    add_includedirs(".")

    add_files("./Texture/*.cpp")
    add_headerfiles("./Texture/*.h")

    after_build(function (target)
        os.cp("./Shaders/*.hlsl", "$(buildir)/Shaders/") -- temp copy to specific dir
        os.cp("./Resources/Textures/*.dds", "$(buildir)/Textures/") -- temp copy to specific dir
    end)
    

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

