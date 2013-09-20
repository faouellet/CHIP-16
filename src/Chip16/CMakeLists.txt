cmake_minimum_required (VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../bin")
set(DLL_3RD "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/lib/x86")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/lib/x86")

add_executable(Chip16 main.cpp)

target_link_libraries(Chip16 SDL2 SDL2main)

add_custom_command(TARGET Chip16
                   POST_BUILD 
    			   COMMAND ${CMAKE_COMMAND} -E copy 
				   "${PROJECT_SOURCE_DIR}/../3rd party/SDL2/lib/x86/SDL2.dll"
				   $<TARGET_FILE_DIR:Chip16>
				   COMMENT "Copying DLLs")
