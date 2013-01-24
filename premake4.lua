-- Generation des Makefiles du projet

BOOST_INCLUDE_DIR = "/Users/pgentile/Programmation/OpenSourceProjects/boost"
BOOST_LIB_DIR = "/Users/pgentile/Programmation/OpenSourceProjects/boost/stage/lib"

POCO_FOUNDATION_INCLUDE_DIR = '/Users/pgentile/Programmation/OpenSourceProjects/poco/Foundation/include'
POCO_UTIL_INCLUDE_DIR = '/Users/pgentile/Programmation/OpenSourceProjects/poco/Util/include'
POCO_NET_INCLUDE_DIR = '/Users/pgentile/Programmation/OpenSourceProjects/poco/Net/include'
POCO_LIB_DIR = '/Users/pgentile/Programmation/OpenSourceProjects/poco/lib/Darwin/x86_64'

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
	
	includedirs {
		"src",
		"include",
		path.join(BOOST_INCLUDE_DIR, "boost/tr1/tr1"),
		BOOST_INCLUDE_DIR,
		POCO_FOUNDATION_INCLUDE_DIR,
		POCO_UTIL_INCLUDE_DIR,
		POCO_NET_INCLUDE_DIR,
		"/opt/local/include"
	}
	
	flags { "ExtraWarnings", "FatalWarnings", "Symbols", "EnableSSE", "EnableSSE2", "Optimize" }
	buildoptions { "-ansi" }
	linkoptions { "-rdynamic" }
	
	libdirs {
		BOOST_LIB_DIR,
		POCO_LIB_DIR,
		"/opt/local/lib"
	}
	
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
	
	project "zmqdealer"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common", "zmq" }
		sources { "zmq/dealer.cpp" }

	project "zmqhandler"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common", "zmq" }
		sources { "zmq/handler.cpp" }

	project "poco"
		kind "ConsoleApp"
		targetdir "bin"
		links { "common", "zmq", "PocoUtil", "PocoFoundation", "PocoNet" }
		sources { "poco.cpp" }

	project "app-strtol"
		kind "ConsoleApp"
		targetdir "bin"
		sources { "app-strtol.cpp" }
