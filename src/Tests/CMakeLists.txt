cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
	link_directories("${Boost_LIBRARY_DIRS}")
    add_executable(Tests main.cpp) 
    target_link_libraries(Tests ${Boost_LIBRARIES})   
endif()
