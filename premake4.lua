solution "TestCPP"

	configurations { "Release", "Debug" }
	
	language "C++"
	
	includedirs "include"
	flags { "ExtraWarnings", "FatalWarnings" }
	flags { "EnableSSE", "EnableSSE2" }
	
	project "app"
		kind "ConsoleApp"
		targetdir "bin"
		files { "src/app.cpp" }
		links { "logging" }

	project "threading"
		kind "SharedLib"
		targetdir "lib"
		files { "src/thread.cpp", "src/mutex.cpp" }
	
	project "logging"
		kind "SharedLib"
		targetdir "lib"
		files { "src/logger.cpp" }
		links { "threading" }

	configuration "Release"
		flags { "Optimize" }

	configuration "Debug"
		flags { "Symbols" }
