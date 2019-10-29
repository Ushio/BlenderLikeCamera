workspace "BlenderLikeCamera"
    location "build"
    configurations { "Debug", "Release" }

architecture "x86_64"

project "BlenderLikeCamera"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/"
    systemversion "latest"
    flags { "MultiProcessorCompile", "NoPCH" }

    -- Src
    files { "src/**.h", "src/**.hpp", "src/**.cpp" }

    -- raylib
    RAY_ROOT = "libs/raylib-2.5.0-Win64-msvc15";
    RAY_DLL = path.join(RAY_ROOT, "bin/raylib.dll");
    RAY_DLL_FULLPATH = path.join(os.getcwd(), RAY_DLL);

    includedirs { "%{RAY_ROOT}/include" }
    libdirs { "%{RAY_ROOT}/lib" }
    links { "raylib" }

    postbuildcommands {
        "{COPY} " .. RAY_DLL_FULLPATH .. " %{cfg.targetdir}"
    }

    -- glm
    includedirs { "libs/glm/" }
    defines {"GLM_FORCE_CTOR_INIT"}
    defines {"GLM_ENABLE_EXPERIMENTAL"}

    symbols "On"

    filter {"Debug"}
        targetname ("BlenderLikeCamera_Debug")
        optimize "Off"
    filter {"Release"}
        targetname ("BlenderLikeCamera_Release")
        optimize "Full"
    filter{}
