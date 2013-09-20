cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost)

if(Boost_FOOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(Tests main.cpp) 
    target_link_libraries(Tests ${Boost_LIBRARIES})   
endif()