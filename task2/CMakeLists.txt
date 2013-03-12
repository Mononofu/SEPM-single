cmake_minimum_required(VERSION 2.6)
project(task2)

set(CMAKE_CXX_FLAGS "-g -Wall -fpermissive")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ZeroCIce REQUIRED)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})


include_directories(${ZeroCIce_INCLUDE_DIR})
set(LIBS ${LIBS} ${ZeroCIceCore_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceUtil_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceSSL_LIBRARY})
set(LIBS ${LIBS} boost_program_options)
set(LIBS ${LIBS} ${QT_LIBRARIES})
set(LIBS ${LIBS} ${QT_QTDECLARATIVE_LIBRARY})

SET(task2_HEADERS ServerSelector.h)
QT4_WRAP_CPP(task2_HEADERS_MOC ${task2_HEADERS})


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

add_executable(sdc_client sdc_client.cpp ${out_file} ${task2_HEADERS_MOC})

target_link_libraries(sdc_client ${LIBS})
