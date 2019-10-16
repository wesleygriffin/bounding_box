cmake_minimum_required (VERSION 3.14)
project(bounding_box LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/catch2)
include(CTest)
include(Catch)

add_executable (bounding_box bounding_box.cpp)
target_compile_features(bounding_box PRIVATE cxx_std_17)
catch_discover_tests(bounding_box)