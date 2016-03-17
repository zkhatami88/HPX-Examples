cmake_minimum_required(VERSION 2.8)
project(test)

set(CMAKE_PREFIX_PATH "/home/zahra/HPX/lib/cmake/HPX")

find_package(HPX REQUIRED)

set(SOURCE_FILES main2.cpp)  

add_hpx_executable(test
        ESSENTIAL
        SOURCES ${SOURCE_FILES}
        COMPONENT_DEPENDENCIES iostreams)
