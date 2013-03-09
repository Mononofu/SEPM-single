cmake_minimum_required(VERSION 2.6)
project(task1)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(sdc_client sdc_client.cpp)

target_link_libraries(sdc_client boost_program_options)
