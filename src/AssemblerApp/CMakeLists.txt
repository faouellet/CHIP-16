cmake_minimum_required (VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(SOURCES main.cpp)

include_directories("${CMAKE_SOURCE_DIR}/src/AssemblerLib")
link_directories("${CMAKE_BINARY_DIR}/lib")

add_executable(AssemblerApp ${SOURCES})

target_link_libraries(AssemblerApp AssemblerLib)
