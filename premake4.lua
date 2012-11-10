-- Generation des Makefiles du projet

BOOST_INCLUDE_DIR = "/Users/pgentile/Programmation/OpenSourceProjects/boost"

function sources(sources)
	local sourceDir = "src"
	local source
	for _, source in pairs(sources) do
		files(path.join(sourceDir, source))
	end
end

function threaded()
	buildoptions { "-pthread" }
	linkoptions { "-pthread" }
end

-- Description de la solution

solution "TestCPP"

	configurations { "Release" }
	
	language "C++"
	
	includedirs { "src", "include", BOOST_INCLUDE_DIR }
	flags { "ExtraWarnings", "FatalWarnings", "Symbols", "EnableSSE", "EnableSSE2", "Optimize" }
	buildoptions { "-ansi" }
	linkoptions { "-rdynamic" }
	
	includedirs { "/opt/local/include" }
	libdirs { "/opt/local/lib" }
	
	threaded()
	
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
			"logger/Logger.cpp",
			"ndbm/RawStoreFile.cpp"
		}

	project "app"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common" }
		sources { "app.cpp" }
	
	project "zmqclient"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common", "zmq" }
		sources { "zmqclient.cpp" }
	
	project "zmqserver"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common", "zmq" }
		sources { "zmqserver.cpp" }

