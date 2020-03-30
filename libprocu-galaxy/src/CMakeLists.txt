#
# @version: 2020-03-07
# @created: 2020-03-01
#

cmake_minimum_required(VERSION 3.0.0)

project(libprocu-galaxy VERSION 0.00.04 LANGUAGES CXX)
set(PROJECT_DESCRIPTION "Header-only C++ library")
set(PROJECT_URL "https://openteq.wordpress.com/portfolio/libregaming/")

option(BUILD_EXAMPLE "Build example (build example demo)" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.hpp")

if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    SET(CMAKE_INSTALL_INCLUDEDIR include CACHE
        PATH "Output directory for header files")
endif()


include_directories(
    "ext"
)

if (BUILD_EXAMPLE)
    add_executable(gengalaxy gengalaxy.cpp)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Android")
        target_link_libraries(gengalaxy log atomic)
    endif()
endif (BUILD_EXAMPLE)

install(FILES include/aixlog.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
