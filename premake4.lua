

function sources(sources)
	local sourceDir = "src"
	local source
	for _, source in pairs(sources) do
		files(path.join(sourceDir, source))
	end
end

solution "TestCPP"

	configurations { "Release" }
	
	language "C++"
	
	includedirs "include"
	flags { "ExtraWarnings", "FatalWarnings", "Symbols", "EnableSSE", "EnableSSE2" }
	buildoptions { "-pthread", "-ansi" }
	linkoptions { "-pthread", "-rdynamic" }
	
	configuration "Release"

	project "common"
		kind "SharedLib"
		targetdir "lib"
		sources { 
			"threading/Thread.cpp",
			"threading/Mutex.cpp",
			"threading/ReadWriteLock.cpp", 
			"util/UndefinedError.cpp",
			"patterns/Singleton.cpp",
			"core/Backtrace.cpp",
			"core/Exception.cpp",
			"core/CLibException.cpp",
			"core/Reflection.cpp",
			"core/StackElement.cpp",
			"logger/Logger.cpp"
		}

	project "app"
		kind "ConsoleApp"
		links { "common" }
		targetdir "bin"
		sources { "app.cpp" }
