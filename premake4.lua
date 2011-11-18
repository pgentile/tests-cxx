

function sources(sources)
	local sourceDir = "src"
	local source
	for _, source in pairs(sources) do
		files(path.join(sourceDir, source))
	end
end

solution "TestCPP"

	configurations { "Release", "Debug" }
	
	language "C++"
	
	includedirs "include"
	flags { "ExtraWarnings", "FatalWarnings" }
	flags { "EnableSSE", "EnableSSE2" }
	buildoptions { "-pthread", "-ansi", "-felide-constructors" }
	linkoptions { "-pthread" }
	
	configuration "Release"
		flags { "Optimize" }

	configuration "Debug"
		flags { "Symbols" }
		targetsuffix "-debug"
	
	project "common"
		kind "SharedLib"
		sources { 
			"threading/Thread.cpp",
			"threading/Mutex.cpp",
			"threading/ReadWriteLock.cpp", 
			"patterns/Singleton.cpp"
		}

	project "logger"
		kind "SharedLib"
		links { "common" }
		sources {
			"logger/Logger.cpp"
		}

	project "app"
		kind "ConsoleApp"
		links { "common", "logger" }
		sources { "app.cpp" }
