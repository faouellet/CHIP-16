cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

set( SOURCES 
	main.cpp 
	assembler_tests.h
	assembler_tests.cpp
	emitter_tests.h
	emitter_tests.cpp
	)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") 
endif()

find_package(Boost COMPONENTS unit_test_framework)

if(Boost_FOUND)
    # Boost
    include_directories("${Boost_INCLUDE_DIR}")
    link_directories("${Boost_LIBRARY_DIRS}")
	
	# Assembler 
    include_directories("${CMAKE_SOURCE_DIR}/src/AssemblerLib")
    link_directories("${CMAKE_BINARY_DIR}/lib")
    
    add_executable(AssemblerTests ${SOURCES}) 
    target_link_libraries(AssemblerTests ${Boost_LIBRARIES} AssemblerLib)
endif()
