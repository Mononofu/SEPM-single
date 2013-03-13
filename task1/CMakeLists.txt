cmake_minimum_required(VERSION 2.6)
project(task1)

set(CMAKE_C_COMPILER "gcc-4.6")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-4.6")

set(CMAKE_CXX_FLAGS "-g -Wall -fpermissive")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ZeroCIce REQUIRED)
include_directories(${ZeroCIce_INCLUDE_DIR})
set(LIBS ${LIBS} ${ZeroCIceCore_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceUtil_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceSSL_LIBRARY})
set(LIBS ${LIBS} boost_program_options)

#begin snippet
#next lines are for creating .h & .cpp files out of .ice Files
set(out_dir "${CMAKE_CURRENT_SOURCE_DIR}")
set(out_file "${out_dir}/SecureDistributedChat.cpp")

add_custom_command(OUTPUT
"${out_file}"
COMMAND "${ZeroCIce_slice2cpp_BIN}"
  --add-header=cstddef
  -I /usr/share/Ice/slice/
  -I /usr/share/Ice-3.4.2/slice/
  -I /usr/share/slice/
  --stream
  --output-dir "${out_dir}"
  "${CMAKE_CURRENT_SOURCE_DIR}/SecureDistributedChat.ice"
)

#end snippet

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sdc_client sdc_client.cpp ${out_file})

target_link_libraries(sdc_client ${LIBS})
