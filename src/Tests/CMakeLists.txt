cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD OFF)

set(BOOST_MIN_VERSION "1.46.0")
find_package(Boost ${BOOST_MIN_VERSION})

if(Boost_FOOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(Tests main.cpp) 
    target_link_libraries(Tests ${Boost_LIBRARIES})   
endif()