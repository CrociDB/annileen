local BUILD_DIR = path.join("build", _ACTION)
if _OPTIONS["cc"] ~= nil then
	BUILD_DIR = BUILD_DIR .. "_" .. _OPTIONS["cc"]
end
local ANNILEEN_DIR = "annileen"
local BGFX_DIR = "bgfx"
local BIMG_DIR = "bimg"
local BX_DIR = "bx"
local GLFW_DIR = "glfw"
local GLM_DIR = "glm"
local PERLINNOISE_DIR = "PerlinNoise"
local TOML11_DIR = "toml11"
local FMT_DIR = "fmt"
local ASSIMP_DIR = "assimp"

newoption {
	trigger     = "python",
	value       = "python3",
	description = "Choose python version",
	allowed = {
	   { "python3",    "Python3 - whatever one installed in the system" },
	   { "python3.6",  "force Python3.6" },
	   { "python3.7",  "force Python3.7" },
	   { "python3.8",  "force Python3.8" },
	   { "python3.9",  "force Python3.9" },
	   { "python3.10",  "force Python3.10" },
	}
 }

solution "annileen-engine"
	location(BUILD_DIR)
	startproject "example-worldbuilding"
	configurations { "Release", "ReleaseEditor", "Debug" }
	if os.is64bit() and not os.istarget("windows") then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end
	filter "system:windows"
		defines "BGFX_CONFIG_RENDERER_DIRECT3D11"
		defines "BGFX_CONFIG_RENDERER_DIRECT3D12"
		defines "BGFX_CONFIG_RENDERER_VULKAN"
		defines "BGFX_CONFIG_RENDERER_METAL"
		buildoptions { "/Zc:__cplusplus" }
	filter "configurations:ReleaseEditor"
		defines "NDEBUG"
		defines "BX_CONFIG_DEBUG=1"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Full"
	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"
		defines "NDEBUG"
		optimize "Full"
	filter "configurations:Debug*"
		defines "_DEBUG"
		defines "BX_CONFIG_DEBUG=1"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Debug"
		symbols "On"
	filter "platforms:x86"
		architecture "x86"
	filter "platforms:x86_64"
		architecture "x86_64"
	filter "system:macosx"
		xcodebuildsettings {
			["MACOSX_DEPLOYMENT_TARGET"] = "10.9",
			["ALWAYS_SEARCH_USER_PATHS"] = "YES", -- This is the minimum version of macos we'll be able to run on
		};

function setBxCompat()
	filter "action:vs*"
		includedirs { path.join(BX_DIR, "include/compat/msvc") }
	filter { "system:windows", "action:gmake" }
		includedirs { path.join(BX_DIR, "include/compat/mingw") }
	filter { "system:macosx" }
		includedirs { path.join(BX_DIR, "include/compat/osx") }
		buildoptions { "-x objective-c++" }
end
		
project "annileen-editor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "On"
	rtti "On"
	files
	{	
		path.join(ANNILEEN_DIR, "editor/*"),
		path.join(ANNILEEN_DIR, "imgui-utils/*"),
	}
	includedirs
	{
		ANNILEEN_DIR,
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(GLFW_DIR, "include"),	
		path.join(GLM_DIR, "glm"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(ANNILEEN_DIR, "engine"),
		path.join(ANNILEEN_DIR, "resources/imgui"),	
		path.join(FMT_DIR, "include"),
		path.join(ASSIMP_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty/dear-imgui/widgets"),
		TOML11_DIR,
		PERLINNOISE_DIR
	}	
	links { "annileen" }
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()
	
project "annileen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "On"
	rtti "On"
	files
	{
		path.join(ANNILEEN_DIR, "engine/*"),
		path.join(ANNILEEN_DIR, "engine/core/*"),
		path.join(ANNILEEN_DIR, "engine/text/*"),	
		path.join(ANNILEEN_DIR, "engine/scene/*"),	
		path.join(ANNILEEN_DIR, "imgui-utils/*"),
		path.join(BIMG_DIR, "src/image_decode.cpp"),
		path.join(BIMG_DIR, "3rdparty/tinyexr/deps/miniz/miniz.c"),

	}
	includedirs
	{
		ANNILEEN_DIR,
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		path.join(GLM_DIR, "glm"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BIMG_DIR, "3rdparty"),
		path.join(BIMG_DIR, "3rdparty/tinyexr/deps/miniz"),
		path.join(ANNILEEN_DIR, "resources/imgui"),
		path.join(FMT_DIR, "include"),
		path.join(ASSIMP_DIR, "include"),
		TOML11_DIR,
		PERLINNOISE_DIR
	}
	filter "system:windows"
		prebuildcommands { "%{os.getcwd()}/tools/win-tools/run_asset_tools.bat" }
	filter "system:not windows"
		if _OPTIONS['python'] then
			prebuildcommands { "%{_OPTIONS[\"python\"]} %{os.getcwd()}/tools/asset_tools.py" }
		end
	setBxCompat()

project "example-worldbuilding"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "On"
	rtti "On"
	files
	{
		path.join(ANNILEEN_DIR, "*"),
		path.join(ANNILEEN_DIR, "examples/worldbuilding/*"),
	}
	includedirs
	{
		ANNILEEN_DIR,
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		path.join(GLM_DIR, "glm"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(ANNILEEN_DIR, "engine"),
		path.join(ANNILEEN_DIR, "resources/imgui"),
		path.join(FMT_DIR, "include"),
		path.join(ASSIMP_DIR, "include"),
		TOML11_DIR,
		PERLINNOISE_DIR
	}
	debugdir "."
	links { "bgfx", "bimg", "bx", "annileen", "glfw", "assimp", "imgui" }
	filter "configurations:ReleaseEditor"
		links {"annileen-editor"}
		defines "NDEBUG"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Full"
	filter "configurations:Release"
		defines "NDEBUG"
		optimize "Full"
	filter "configurations:Debug*"
		links {"annileen-editor"}
		defines "_DEBUG"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Debug"
		symbols "On"
	filter "system:windows"
		links { "gdi32", "kernel32", "psapi" }
	filter "system:linux"
		links { "dl", "GL", "pthread", "X11" }
	filter "system:macosx"
		links { "QuartzCore.framework", "Metal.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
	setBxCompat()

project "example-cube"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "On"
	rtti "On"
	files
	{
		path.join(ANNILEEN_DIR, "*"),
		path.join(ANNILEEN_DIR, "examples/cube/*"),
	}
	includedirs
	{
		ANNILEEN_DIR,
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		path.join(GLM_DIR, "glm"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(ANNILEEN_DIR, "engine"),
		path.join(ANNILEEN_DIR, "resources/imgui"),
		path.join(FMT_DIR, "include"),
		path.join(ASSIMP_DIR, "include"),
		TOML11_DIR,
		PERLINNOISE_DIR
	}
	debugdir "."
	links { "annileen", "bgfx", "bimg", "bx", "glfw", "assimp", "imgui" }
	filter "configurations:ReleaseEditor"
		links {"annileen-editor"}
		defines "NDEBUG"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Full"
	filter "configurations:Release"
		defines "NDEBUG"
		optimize "Full"
	filter "configurations:Debug*"
		links {"annileen-editor"}
		defines "_DEBUG"
		defines "_ANNILEEN_COMPILER_EDITOR"
		optimize "Debug"
		symbols "On"
	filter "system:windows"
		links { "gdi32", "kernel32", "psapi" }
	filter "system:linux"
		links { "dl", "GL", "pthread", "X11" }
	filter "system:macosx"
		links { "QuartzCore.framework", "Metal.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
	setBxCompat()


project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "Off"
	rtti "Off"
	defines {"__STDC_FORMAT_MACROS", "BGFX_CONFIG_RENDERER_OPENGL=32"}
	files
	{
		path.join(BGFX_DIR, "include/bgfx/**.h"),
		path.join(BGFX_DIR, "src/*.cpp"),
		path.join(BGFX_DIR, "src/*.h"),
	}
	excludes
	{
		path.join(BGFX_DIR, "src/amalgamated.cpp"),
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BGFX_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BGFX_DIR, "3rdparty/dxsdk/include"),
		path.join(BGFX_DIR, "3rdparty/khronos")
	}
	filter "configurations:Debug"
		defines "BGFX_CONFIG_DEBUG=1"
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		excludes
		{
			path.join(BGFX_DIR, "src/glcontext_glx.cpp"),
			path.join(BGFX_DIR, "src/glcontext_egl.cpp")
		}
	filter "system:macosx"
		files
		{
			path.join(BGFX_DIR, "src/*.mm"),
		}
	setBxCompat()

project "bimg"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "Off"
	rtti "Off"
	files
	{
		path.join(BIMG_DIR, "include/bimg/*.h"),
		path.join(BIMG_DIR, "src/image.cpp"),
		path.join(BIMG_DIR, "src/image_gnf.cpp"),
		path.join(BIMG_DIR, "src/image_decode.cpp"),
		path.join(BIMG_DIR, "src/*.cpp"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/src/decoder/*.cc"),
		path.join(BIMG_DIR, "3rdparty/tinyexr/deps/miniz/miniz.c"),
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BIMG_DIR, "3rdparty/astc-codec"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/include"),
		path.join(BIMG_DIR, "3rdparty/iqa/include"),
		path.join(BIMG_DIR, "3rdparty/"),
		path.join(BIMG_DIR, "3rdparty/tinyexr/deps/miniz/"),
	}
	setBxCompat()

project "bx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	files
	{
		path.join(BX_DIR, "include/bx/*.h"),
		path.join(BX_DIR, "include/bx/inline/*.inl"),
		path.join(BX_DIR, "src/*.cpp")
	}
	excludes
	{
		path.join(BX_DIR, "src/amalgamated.cpp"),
		path.join(BX_DIR, "src/crtnone.cpp")
	}
	includedirs
	{
		path.join(BX_DIR, "3rdparty"),
		path.join(BX_DIR, "include")
	}
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()
		
project "glfw"
	kind "StaticLib"
	language "C"
	files
	{
		path.join(GLFW_DIR, "include/GLFW/*.h"),
    	path.join(GLFW_DIR, "src/internal.h"),
    	path.join(GLFW_DIR, "src/platform.h"),
    	path.join(GLFW_DIR, "src/mappings.h"),
    	path.join(GLFW_DIR, "src/context.c"),
    	path.join(GLFW_DIR, "src/init.c"),
    	path.join(GLFW_DIR, "src/input.c"),
    	path.join(GLFW_DIR, "src/monitor.c"),
    	path.join(GLFW_DIR, "src/platform.c"),
    	path.join(GLFW_DIR, "src/vulkan.c"),
    	path.join(GLFW_DIR, "src/window.c"),
    	path.join(GLFW_DIR, "src/egl_context.c"),
    	path.join(GLFW_DIR, "src/osmesa_context.c"),
    	path.join(GLFW_DIR, "src/null_platform.h"),
    	path.join(GLFW_DIR, "src/null_joystick.h"),
    	path.join(GLFW_DIR, "src/null_init.c"),
    	path.join(GLFW_DIR, "src/null_monitor.c"),
    	path.join(GLFW_DIR, "src/null_window.c"),
    	path.join(GLFW_DIR, "src/null_joystick.c"),
	}
	includedirs { path.join(GLFW_DIR, "include") }
	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			path.join(GLFW_DIR, "src/win32_*.*"),
			path.join(GLFW_DIR, "src/wgl_context.*")
		}
	filter "system:linux"
		defines "_GLFW_X11"
		files
		{
			path.join(GLFW_DIR, "src/glx_context.*"),
			path.join(GLFW_DIR, "src/linux*.*"),
			path.join(GLFW_DIR, "src/posix*.*"),
			path.join(GLFW_DIR, "src/x11*.*"),
			path.join(GLFW_DIR, "src/xkb*.*")
		}
	filter "system:macosx"
		defines "_GLFW_COCOA"
		files
		{
			path.join(GLFW_DIR, "src/cocoa_*.*"),
			path.join(GLFW_DIR, "src/posix_thread.h"),
			path.join(GLFW_DIR, "src/nsgl_context.h"),
			path.join(GLFW_DIR, "src/egl_context.h"),
			path.join(GLFW_DIR, "src/osmesa_context.h"),

			path.join(GLFW_DIR, "src/posix_thread.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/egl_context.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/osmesa_context.c"),                       
		}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"

project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	defines "__STDC_FORMAT_MACROS"
	files
	{
		path.join(BGFX_DIR, "3rdparty/dear-imgui/*"),
		path.join(BGFX_DIR, "3rdparty/dear-imgui/widgets/*"),		
	}
	includedirs
	{
		path.join(BGFX_DIR, "3rdparty/"),	
		path.join(BX_DIR, "3rdparty"),
		path.join(BX_DIR, "include")
	}
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()

project "PerlinNoise"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	defines "__STDC_FORMAT_MACROS"
	files
	{
		path.join(PERLINNOISE_DIR, "*"),
	}
	includedirs
	{
		path.join(PERLINNOISE_DIR, "*"),
	}
	setBxCompat()

project "assimp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
  
	defines {
		-- "SWIG",
		"ASSIMP_BUILD_NO_OWN_ZLIB",
  
		"ASSIMP_BUILD_NO_X_IMPORTER",
		"ASSIMP_BUILD_NO_3DS_IMPORTER",
		"ASSIMP_BUILD_NO_MD3_IMPORTER",
		"ASSIMP_BUILD_NO_MDL_IMPORTER",
		"ASSIMP_BUILD_NO_MD2_IMPORTER",
		-- "ASSIMP_BUILD_NO_PLY_IMPORTER",
		"ASSIMP_BUILD_NO_ASE_IMPORTER",
		-- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
		"ASSIMP_BUILD_NO_AMF_IMPORTER",
		"ASSIMP_BUILD_NO_HMP_IMPORTER",
		"ASSIMP_BUILD_NO_SMD_IMPORTER",
		"ASSIMP_BUILD_NO_MDC_IMPORTER",
		"ASSIMP_BUILD_NO_MD5_IMPORTER",
		"ASSIMP_BUILD_NO_STL_IMPORTER",
		"ASSIMP_BUILD_NO_LWO_IMPORTER",
		"ASSIMP_BUILD_NO_DXF_IMPORTER",
		"ASSIMP_BUILD_NO_NFF_IMPORTER",
		"ASSIMP_BUILD_NO_RAW_IMPORTER",
		"ASSIMP_BUILD_NO_OFF_IMPORTER",
		"ASSIMP_BUILD_NO_AC_IMPORTER",
		"ASSIMP_BUILD_NO_BVH_IMPORTER",
		"ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
		"ASSIMP_BUILD_NO_IRR_IMPORTER",
		"ASSIMP_BUILD_NO_Q3D_IMPORTER",
		"ASSIMP_BUILD_NO_B3D_IMPORTER",
		"ASSIMP_BUILD_NO_COLLADA_IMPORTER",
		"ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
		"ASSIMP_BUILD_NO_CSM_IMPORTER",
		"ASSIMP_BUILD_NO_3D_IMPORTER",
		"ASSIMP_BUILD_NO_LWS_IMPORTER",
		"ASSIMP_BUILD_NO_OGRE_IMPORTER",
		"ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
		"ASSIMP_BUILD_NO_MS3D_IMPORTER",
		"ASSIMP_BUILD_NO_COB_IMPORTER",
		"ASSIMP_BUILD_NO_BLEND_IMPORTER",
		"ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
		"ASSIMP_BUILD_NO_NDO_IMPORTER",
		"ASSIMP_BUILD_NO_IFC_IMPORTER",
		"ASSIMP_BUILD_NO_XGL_IMPORTER",
		"ASSIMP_BUILD_NO_FBX_IMPORTER",
		"ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
		-- "ASSIMP_BUILD_NO_GLTF_IMPORTER",
		"ASSIMP_BUILD_NO_C4D_IMPORTER",
		"ASSIMP_BUILD_NO_3MF_IMPORTER",
		"ASSIMP_BUILD_NO_X3D_IMPORTER",
		"ASSIMP_BUILD_NO_MMD_IMPORTER",
		
		"ASSIMP_BUILD_NO_STEP_EXPORTER",
		"ASSIMP_BUILD_NO_SIB_IMPORTER",
  
		-- "ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS",
		-- "ASSIMP_BUILD_NO_FLIPUVS_PROCESS",
		-- "ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS",
		-- "ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS",
		"ASSIMP_BUILD_NO_JOINVERTICES_PROCESS",
		-- "ASSIMP_BUILD_NO_TRIANGULATE_PROCESS",
		"ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS",
		-- "ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS",
		"ASSIMP_BUILD_NO_REMOVEVC_PROCESS",
		"ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS",
		"ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS",
		"ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS",
		-- "ASSIMP_BUILD_NO_VALIDATEDS_PROCESS",
		"ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS",
		"ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS",
		"ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS",
		"ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS",
		"ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS",
		"ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS",
		"ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS",
		"ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS",
		"ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS",
		"ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS",
		"ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS",
		"ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS",
		"ASSIMP_BUILD_NO_DEBONE_PROCESS",
		"ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS",
		"ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS",
	}
  
	files {
		path.join(ASSIMP_DIR, "include/**"),
		path.join(ASSIMP_DIR, "code/Assimp.cpp"),
		path.join(ASSIMP_DIR, "code/BaseImporter.cpp"),
		path.join(ASSIMP_DIR, "code/ColladaLoader.cpp"),
		path.join(ASSIMP_DIR, "code/ColladaParser.cpp"),
		path.join(ASSIMP_DIR, "code/CreateAnimMesh.cpp"),
		path.join(ASSIMP_DIR, "code/PlyParser.cpp"),
		path.join(ASSIMP_DIR, "code/PlyLoader.cpp"),
		path.join(ASSIMP_DIR, "code/BaseProcess.cpp"),
		path.join(ASSIMP_DIR, "code/EmbedTexturesProcess.cpp"),
		path.join(ASSIMP_DIR, "code/ConvertToLHProcess.cpp"),
		path.join(ASSIMP_DIR, "code/DefaultIOStream.cpp"),
		path.join(ASSIMP_DIR, "code/DefaultIOSystem.cpp"),
		path.join(ASSIMP_DIR, "code/DefaultLogger.cpp"),
		path.join(ASSIMP_DIR, "code/GenVertexNormalsProcess.cpp"),
		path.join(ASSIMP_DIR, "code/Importer.cpp"),
		path.join(ASSIMP_DIR, "code/ImporterRegistry.cpp"),
		path.join(ASSIMP_DIR, "code/MaterialSystem.cpp"),
		path.join(ASSIMP_DIR, "code/PostStepRegistry.cpp"),
		path.join(ASSIMP_DIR, "code/ProcessHelper.cpp"),
		path.join(ASSIMP_DIR, "code/scene.cpp"),
		path.join(ASSIMP_DIR, "code/ScenePreprocessor.cpp"),
		path.join(ASSIMP_DIR, "code/ScaleProcess.cpp"),
		path.join(ASSIMP_DIR, "code/SGSpatialSort.cpp"),
		path.join(ASSIMP_DIR, "code/SkeletonMeshBuilder.cpp"),
		path.join(ASSIMP_DIR, "code/SpatialSort.cpp"),
		path.join(ASSIMP_DIR, "code/TriangulateProcess.cpp"),
		path.join(ASSIMP_DIR, "code/ValidateDataStructure.cpp"),
		path.join(ASSIMP_DIR, "code/Version.cpp"),
		path.join(ASSIMP_DIR, "code/VertexTriangleAdjacency.cpp"),
		path.join(ASSIMP_DIR, "code/ObjFileImporter.cpp"),
		path.join(ASSIMP_DIR, "code/ObjFileMtlImporter.cpp"),
		path.join(ASSIMP_DIR, "code/ObjFileParser.cpp"),
		path.join(ASSIMP_DIR, "code/glTFImporter.cpp"),
		path.join(ASSIMP_DIR, "code/glTF2Importer.cpp"),
		path.join(ASSIMP_DIR, "code/MakeVerboseFormat.cpp"),
		path.join(ASSIMP_DIR, "code/CalcTangentsProcess.cpp"),
		path.join(ASSIMP_DIR, "code/ScaleProcess.cpp"),
		path.join(ASSIMP_DIR, "code/EmbedTexturesProcess.cpp"),
		path.join(ASSIMP_DIR, "contrib/irrXML/*"),
	}
  
	includedirs {
		path.join(ASSIMP_DIR, "include"),
		path.join(ASSIMP_DIR, "contrib/irrXML"),
		path.join(ASSIMP_DIR, "contrib/zlib"),
		path.join(ASSIMP_DIR, "contrib/rapidjson/include"),
	}
  
	 filter "system:windows"
		systemversion "latest"
  
	 filter  "configurations:Debug"
		 runtime "Debug"
		 symbols "on"
  
	 filter  "configurations:Release"
		 runtime "Release"
		 optimize "on"
  