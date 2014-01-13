cmake_minimum_required (VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(SOURCES main.cpp)

add_executable(AssemblerApp ${SOURCES})

target_link_libraries(AssemblerApp AssemblerLib)
