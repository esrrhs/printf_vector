cmake_minimum_required(VERSION 3.12)
project(printf_vector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(printf_vector INTERFACE)
target_include_directories(printf_vector INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

option(PRINTF_VECTOR_BUILD_EXAMPLE "Build example program" ON)
if(PRINTF_VECTOR_BUILD_EXAMPLE)
    add_executable(printf_vector_example main.cpp)
    target_link_libraries(printf_vector_example PRIVATE printf_vector)

    enable_testing()
    add_test(NAME printf_vector_example COMMAND printf_vector_example)
endif()
