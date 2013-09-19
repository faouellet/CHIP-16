cmake_minimum_required (VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "../../bin")

set(SDL_INCLUDE_DIR "../../3rd party/SDL2/include")
set(SDL_LIBRARY "../../3rd party/SDL2/lib/SDL.dll")
include_directories(${SDL_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL_LIBRARY})
link_libraries(${LIBS})

add_executable(Chip16 main.cpp)
