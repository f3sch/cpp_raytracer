# Preamble
cmake_minimum_required(VERSION 3.20)
project(raytracer)
enable_testing()

# Project wide setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror") #enforce stringent build requirements
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #move runtime targets into one folder

# Externally provided content
# add_subdirectory(dependencies)
# include(packages)
find_package(OpenMP 4.0 REQUIRED)
find_package(Threads REQUIRED)

# Main targets built by this project
add_subdirectory(src)


# Testing and Installing
# option(BUILD_TESTS "Build unit tests." ON)
# if(BUILD_TESTS)
#   add_subdirectory(test)
# endif()

# # Scripts
# add_subdirectory(scripts)
