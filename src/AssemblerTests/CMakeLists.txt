cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS unit_test_framework)
include_directories("${CMAKE_SOURCE_DIR}/src/AssemblerLib")
include_directories("${CMAKE_SOURCE_DIR}/src/Utils")

if(WIN32)
    set( SOURCES 
        main.cpp 
        assembler_tests.h
        assembler_tests.cpp
        emitter_tests.h
        emitter_tests.cpp
    )

    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

    # Boost
    include_directories("${Boost_INCLUDE_DIR}")
    link_directories("${Boost_LIBRARY_DIRS}")
	
	# Assembler 
    link_directories("${CMAKE_BINARY_DIR}/lib")
    
    add_executable(AssemblerTests ${SOURCES}) 
    target_link_libraries(AssemblerTests ${Boost_LIBRARIES} AssemblerLib)
else()
    include(${PROJECT_SOURCE_DIR}/cmake/BoostTestHelpers.cmake)

    add_boost_test(assembler_tests.cpp AssemblerLib)
    add_boost_test(emitter_tests.cpp AssemblerLib)
endif()
