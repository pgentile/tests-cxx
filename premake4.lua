-- Generation des Makefiles du projet

POCO_ROOT = "/Users/pgentile/Programmation/OpenSourceProjects/poco"
POCO_FOUNDATION_INCLUDE_DIR = POCO_ROOT .. '/Foundation/include'
POCO_UTIL_INCLUDE_DIR = POCO_ROOT .. '/Util/include'
POCO_NET_INCLUDE_DIR = POCO_ROOT .. '/Net/include'
POCO_LIB_DIR = POCO_ROOT .. '/lib/Darwin/x86_64'

function sources(sources)
	local sourceDir = "src"
	local source
	for _, source in pairs(sources) do
		files(path.join(sourceDir, source))
	end
end

function use_threads()
	buildoptions { "-pthread" }
	linkoptions { "-pthread" }
end

function use_poco()
	includedirs {
		POCO_FOUNDATION_INCLUDE_DIR,
		POCO_UTIL_INCLUDE_DIR,
		POCO_NET_INCLUDE_DIR
	}
	libdirs {
  		POCO_LIB_DIR
	}
	links { "PocoUtil", "PocoFoundation", "PocoNet" }
end

function use_zmq()
  includedirs "vendors/cppzmq/include"
	links "zmq"
end

function extra_warnings()
  buildoptions { "-Wextra", "-Wsign-conversion", "-Wconversion" }
end

-- Description de la solution

solution "TestCPP"

	configurations { "Debug", "Release" }
	
	language "C++"
	
	includedirs {
		"src",
		"include",
		"/opt/local/include"
	}
	
	flags { "ExtraWarnings", "FatalWarnings", "Symbols", "EnableSSE", "EnableSSE2" }
	buildoptions { "-std=c++11", "-fPIC" }
	
	libdirs {
		"/opt/local/lib"
	}
	
	use_threads()
	
	configuration "Debug"
	
	configuration "Release"
	  defines { "NDEBUG" }
	  flags { "OptimizeSpeed" }

	project "common"
		kind "SharedLib"
		targetdir "lib"
		extra_warnings()
		sources { 
			"threading/ReadWriteLock.cpp", 
			"util/Singleton.cpp",
			"util/Backtrace.cpp",
			"util/Reflection.cpp",
			"logger/Logger.cpp",
			"ndbm/RawStoreFile.cpp",
			"date/LocalDateTime.cpp",
			"date/LocalDateTimeRange.cpp"
		}
	
	project "model"
	    kind "SharedLib"
	    targetdir "lib"
  		extra_warnings()
		links { "common" }
	    sources {
	        "model/data/PfiData.cpp",
            "model/data/ComptePayeurData.cpp",
            "model/cliche/Pfi.cpp",
            "model/cliche/ComptePayeur.cpp"
	    }

	project "app"
		kind "ConsoleApp"
		targetdir "bin"
		extra_warnings()
		links { "common" }
		sources { "app.cpp" }
	
	project "zmqclient"
		kind "ConsoleApp"
		targetdir "bin"
		extra_warnings()
		use_zmq()
		links { "common" }
		sources { "zmqclient.cpp" }
	
	project "zmqserver"
		kind "ConsoleApp"
		targetdir "bin"
		extra_warnings()
		use_zmq()
		links { "common" }
		sources { "zmqserver.cpp" }
	
	project "zmqdealer"
		kind "ConsoleApp"
		targetdir "bin"
		extra_warnings()
		use_zmq()
		links { "common" }
		sources { "zmq/dealer.cpp" }

	project "zmqhandler"
		kind "ConsoleApp"
		targetdir "bin"
		extra_warnings()
		use_zmq()
		links { "common" }
		sources { "zmq/handler.cpp" }

	project "poco"
		kind "ConsoleApp"
		targetdir "bin"
		use_poco()
		links { "common" }
		sources { "poco.cpp" }

	project "app-strtol"
		kind "ConsoleApp"
		extra_warnings()
		targetdir "bin"
		sources { "app-strtol.cpp" }

	project "app-exceptions"
		kind "ConsoleApp"
		extra_warnings()
		targetdir "bin"
		links { "common" }
		sources { "app-exceptions.cpp" }
	
	project "app-mmap"
	    kind "ConsoleApp"
  		extra_warnings()
	    targetdir "bin"
  		links { "common" }
	    sources { "app-mmap.cpp" }

	project "app-logger"
	    kind "ConsoleApp"
  		extra_warnings()
	    targetdir "bin"
		links { "common" }
	    sources { "app-logger.cpp" }
	
	project "app-cpp11"
        kind "ConsoleApp"
    		extra_warnings()
        targetdir "bin"
  		links { "common" }
        sources { "app-cpp11.cpp" }

  	project "detach-thread"
  	    kind "ConsoleApp"
    		extra_warnings()
  	    targetdir "bin"
  		links { "common" }
  	    sources { "detach-thread.cpp" }
  	  
  	  project "streams"
  	    kind "ConsoleApp"
    		extra_warnings()
  	    targetdir "bin"
    		links { "common" }
  	    sources { "streams.cpp" }
