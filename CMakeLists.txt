cmake_minimum_required(VERSION 3.21)

project(small_vector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

if(NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release|RelWithDebInfo)$")
    message(WARNING "The specified build type [${CMAKE_BUILD_TYPE}] is not recognized. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SMALL_VECTOR_FLAGS} -permissive- -W4 -WX -diagnostics:caret")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SMALL_VECTOR_FLAGS} -Wall -Wextra -Wpedantic -Werror -pedantic-errors -g")
endif()

add_library(small_vector INTERFACE)
target_include_directories(small_vector SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(small_vector INTERFACE "cxx_std_20")
target_compile_options(small_vector INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:-Zc:preprocessor>" "$<$<CXX_COMPILER_ID:MSVC>:-Zc:__cplusplus>")
target_link_options(small_vector INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:-NATVIS:${CMAKE_CURRENT_SOURCE_DIR}/small_vector.natvis>")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/benchmark")
