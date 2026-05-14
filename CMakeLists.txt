cmake_minimum_required(VERSION 3.20)
project(PolinomicCalculator)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

set(MP2_LIBRARY "polinomic")
set(MP2_TESTS   "polinomic_test")
set(MP2_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${MP2_INCLUDE})

# BUILD
add_subdirectory(src)
add_subdirectory(gtest)
add_subdirectory(test)
add_subdirectory(main)
add_subdirectory(gui)

message(STATUS "")
message(STATUS "General configuration for ${PROJECT_NAME}")
message(STATUS "======================================")
message(STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message(STATUS "   Qt6: ${Qt6_FOUND}")
message(STATUS "")