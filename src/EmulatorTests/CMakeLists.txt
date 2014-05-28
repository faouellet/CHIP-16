cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

set( SOURCES 
	main.cpp 
	interpreter_tests.cpp 
	interpreter_tests.h 
	gpu_tests.cpp 
	gpu_tests.h
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
    
    # SDL
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/include")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/lib/x86")
	
    # Emulator 
    include_directories("${CMAKE_SOURCE_DIR}/src/EmulatorLib")
    link_directories("${CMAKE_BINARY_DIR}/lib")
    
    add_executable(EmulatorTests ${SOURCES}) 
    target_link_libraries(EmulatorTests ${Boost_LIBRARIES} SDL2 EmulatorLib)
	
    add_custom_command(TARGET EmulatorTests
               POST_BUILD 
    		   COMMAND ${CMAKE_COMMAND} -E copy_directory
    			   "${PROJECT_SOURCE_DIR}/Chip16 Program Pack"
    			   $<TARGET_FILE_DIR:EmulatorTests>
    			   COMMENT "Copying test pack")
endif()
