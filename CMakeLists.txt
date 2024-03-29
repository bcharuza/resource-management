cmake_minimum_required( VERSION 3.13.4 FATAL_ERROR )
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-includes)

message("------------------------------------")
include(Version)
git_tag_desc(version)
parse_version(version ${version})

message(STATUS "Project version: ${version_major}.${version_minor}.${version_patch}")
message(STATUS "Description: ${version_desc}")
project("Resource Management" VERSION ${version_major}.${version_minor}.${version_patch} LANGUAGES CXX )

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("--------------Examples--------------")
add_subdirectory(examples)
message("--------------Document--------------")
add_subdirectory(doc)
message("---------------Tests---------------")
enable_testing()
add_subdirectory(tests)
message("------------------------------------")
