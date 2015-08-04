cmake_minimum_required (VERSION 2.8)

set(DLL_3RD "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/lib/x86")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(SOURCES main.cpp commandlineinterface.cpp commandlineinterface.h)

# SDL
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/SDL2/include")
set(LINK_DIR "")
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(LINK_DIR "${PROJECT_SOURCE_DIR}/3rd party/SDL2/lib/x64")
else()
	set(LINK_DIR "${PROJECT_SOURCE_DIR}/3rd party/SDL2/lib/x86")
endif()
link_directories("${LINK_DIR}")

# Emulator
include_directories("${CMAKE_SOURCE_DIR}/src/EmulatorLib")
link_directories("${CMAKE_BINARY_DIR}/lib")

add_executable(EmulatorApp ${SOURCES})

target_link_libraries(EmulatorApp SDL2 SDL2main EmulatorLib)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_custom_command(TARGET EmulatorApp
                       POST_BUILD 
        			   COMMAND ${CMAKE_COMMAND} -E copy 
    				   "${LINK_DIR}/SDL2.dll"
    				   $<TARGET_FILE_DIR:EmulatorApp>
    				   COMMENT "Copying DLLs")
endif()

