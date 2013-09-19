cmake_minimum_required (VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "../../bin")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
link_libraries(${SDL2_LIBRARIES})

add_executable(Chip16 main.cpp)
