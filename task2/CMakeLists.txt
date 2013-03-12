# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects

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

SET(task2_HEADERS ServerSelector.h FileWatcher.h)
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

# hack to show files in Qt Creator
FILE(GLOB QML_FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.qml")
add_custom_target( QML_Resources ALL SOURCES ${QML_FILES_LIST} )

FILE(GLOB HEADER_FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_custom_target( HEADER_Resources ALL SOURCES ${HEADER_FILES_LIST} )


# copy resources to build dir
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ui"
    COMMAND cmake -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/ui" "${CMAKE_CURRENT_BINARY_DIR}/ui")
list(APPEND copy_deps "${CMAKE_CURRENT_BINARY_DIR}/ui")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ca.crt"
    COMMAND cmake -E copy "${CMAKE_CURRENT_SOURCE_DIR}/ca.crt" "${CMAKE_CURRENT_BINARY_DIR}/ca.crt")
list(APPEND copy_deps "${CMAKE_CURRENT_BINARY_DIR}/ca.crt")


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sdc_client sdc_client.cpp FileWatcher.cpp ${out_file} ${task2_HEADERS_MOC} ${copy_deps})

target_link_libraries(sdc_client ${LIBS})
