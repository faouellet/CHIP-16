cmake_minimum_required (VERSION 2.8)

add_library( EmulatorLib STATIC 
        constants.h
		dynarec.cpp
		dynarec.h
		instruction.cpp
		instruction.h
		interpreter.cpp
		interpreter.h
		emulator.cpp 
		emulator.h
		cpu.cpp 
		cpu.h
		gpu.cpp 
		gpu.h
		spu.cpp 
		spu.h
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/include")
	   