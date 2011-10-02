solution "TestCPP"
	configurations { "Release" }
	files "src/*.cpp"
	includedirs "include"
	flags { "ExtraWarnings", "FatalWarnings" }
	
	project "app"
		language "C++"
		kind "ConsoleApp"
		targetdir "bin"
