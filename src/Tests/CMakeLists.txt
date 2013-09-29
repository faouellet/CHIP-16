cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

set(SOURCES main.cpp utils_tests.cpp emulator_tests.cpp emulator_tests.h cpu_tests.cpp cpu_tests.h gpu_tests.cpp gpu_tests.h spu_tests.cpp spu_tests.h)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

find_package(Boost)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    include_directories("${CMAKE_SOURCE_DIR}/Chip16")
    link_directories("${Boost_LIBRARY_DIRS}")
    add_executable(Tests ${SOURCES}) 
    target_link_libraries(Tests ${Boost_LIBRARIES})

	add_custom_command(TARGET Tests
                   POST_BUILD 
    			   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   "${PROJECT_SOURCE_DIR}/../Chip16 Program Pack"
				   $<TARGET_FILE_DIR:Tests>
				   COMMENT "Copying test pack")
endif()
