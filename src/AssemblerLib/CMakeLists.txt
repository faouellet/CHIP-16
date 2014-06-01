cmake_minimum_required (VERSION 2.8)

add_library( AssemblerLib STATIC
	assembler.h
	assembler.cpp
	emitter.h
	emitter.cpp
	)

find_package(Boost)

if(Boost_FOUND)
    # Boost
    include_directories("${Boost_INCLUDE_DIR}")
	
    set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
endif()
