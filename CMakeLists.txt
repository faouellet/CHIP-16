cmake_minimum_required (VERSION 2.8)
project (Chip16) 

add_subdirectory (src)

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libstdc++" CACHE STRING "" FORCE)

