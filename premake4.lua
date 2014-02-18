-- Generation des Makefiles du projet

POCO_ROOT = "/Users/pgentile/Programmation/OpenSourceProjects/poco"
POCO_FOUNDATION_INCLUDE_DIR = POCO_ROOT .. '/Foundation/include'
POCO_UTIL_INCLUDE_DIR = POCO_ROOT .. '/Util/include'
POCO_NET_INCLUDE_DIR = POCO_ROOT .. '/Net/include'
POCO_LIB_DIR = POCO_ROOT .. '/lib/Darwin/x86_64'

SOURCE_DIR = 'src'


function sources(sources)
    local _, source
    for _, source in pairs(sources) do
        files(path.join(SOURCE_DIR, source))
    end
end


function find_sources(patterns)
    local _, pattern
    for _, pattern in pairs(patterns) do
        files(os.matchfiles(path.join(SOURCE_DIR, pattern)))
    end
end


function use_poco()
    includedirs {
        POCO_FOUNDATION_INCLUDE_DIR,
        POCO_UTIL_INCLUDE_DIR,
        POCO_NET_INCLUDE_DIR
    }
    libdirs { POCO_LIB_DIR }
    links {
        "PocoUtil",
        "PocoFoundation",
        "PocoNet"
    }
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
        SOURCE_DIR,
        "include",
        "/opt/local/include"
    }
    
    flags { "ExtraWarnings", "FatalWarnings", "Symbols", "EnableSSE", "EnableSSE2" }
    buildoptions { "-std=c++11", "-fPIC", "-pthread" }
    linkoptions { "-pthread" }
    
    libdirs {
        "/opt/local/lib"
    }
    
    configuration "Debug"
    
    configuration "Release"
        defines { "NDEBUG" }
        flags { "OptimizeSpeed" }

    project "common"
        kind "SharedLib"
        targetdir "lib"
        extra_warnings()
        find_sources {
            "date/**.cpp",
            "threading/**.cpp",
            "util/**.cpp",
            "logger/**.cpp",
            "ndbm/**.cpp",
            "date/**.cpp",
            "io/**.cpp"
        }

    project "model"
        kind "SharedLib"
        targetdir "lib"
        extra_warnings()
        links { "common" }
        find_sources { "model/**.cpp" }

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
