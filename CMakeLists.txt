cmake_minimum_required(VERSION 2.8)
project(test)

include_directories(.)
include_directories(./include)

aux_source_directory(./ TEST_SRC_LIST)

add_executable(test ${TEST_SRC_LIST})
