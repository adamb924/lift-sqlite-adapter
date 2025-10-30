cmake_minimum_required(VERSION 3.16)

project(lift-ui LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql)

add_library(lift-ui SHARED
    liftquerymodel.h
    liftquerymodel.cpp
    liftquerynode.h
    liftquerynode.cpp
    liftsearchform.h
    liftsearchform.cpp
)

# Use generator expressions to add the "d" suffix in Debug mode
find_library(MORTALENGINE_LIB_RELEASE NAMES mortalengine)
find_library(MORTALENGINE_LIB_DEBUG NAMES mortalengined)

message(STATUS "MORTALENGINE_LIB_RELEASE = ${MORTALENGINE_LIB_RELEASE}")
message(STATUS "MORTALENGINE_LIB_DEBUG = ${MORTALENGINE_LIB_DEBUG}")

find_library(RUDEMECHANICALS_LIB_RELEASE NAMES rude-mechanicals)
find_library(RUDEMECHANICALS_LIB_DEBUG NAMES rude-mechanicalsd)

message(STATUS "RUDEMECHANICALS_LIB_RELEASE = ${RUDEMECHANICALS_LIB_RELEASE}")
message(STATUS "RUDEMECHANICALS_LIB_DEBUG = ${RUDEMECHANICALS_LIB_DEBUG}")

target_include_directories(lift-ui PUBLIC
    "."
    ${CMAKE_PREFIX_PATH}/include/rude-mechanicals
    ${CMAKE_PREFIX_PATH}/include/mortal-engine
    ${CMAKE_PREFIX_PATH}/include/lift-sqlite-adapter
)

target_link_libraries(lift-ui
    PUBLIC
    lift-sqlite-adapter
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    $<$<CONFIG:Debug>:${MORTALENGINE_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${MORTALENGINE_LIB_RELEASE}>
    $<$<CONFIG:Debug>:${RUDEMECHANICALS_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${RUDEMECHANICALS_LIB_RELEASE}>
)

target_compile_definitions(lift-ui PRIVATE LIFTUI_LIBRARY)

install(TARGETS lift-ui
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

# this installs some non-public headers, but so be it
install(DIRECTORY "${CMAKE_SOURCE_DIR}/lift-ui"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING
        PATTERN "*.h"
)
