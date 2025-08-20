cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "macOS")
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "" FORCE)
    set(CMAKE_XCODE_ATTRIBUTE_ARCHS "x86_64")
    set(CMAKE_XCODE_ATTRIBUTE_EXCLUDED_ARCHS "arm64")
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
    set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "x86_64")

else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Template VERSION 1.0.0)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    # Add any extra C++ source files here
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})
