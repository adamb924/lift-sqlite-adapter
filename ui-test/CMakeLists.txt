cmake_minimum_required(VERSION 3.16)

project(ui-test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql)

# Use generator expressions to add the "d" suffix in Debug mode
find_library(MORTALENGINE_LIB_RELEASE NAMES mortalengine)
find_library(MORTALENGINE_LIB_DEBUG NAMES mortalengined)

message(STATUS "MORTALENGINE_LIB_RELEASE = ${MORTALENGINE_LIB_RELEASE}")
message(STATUS "MORTALENGINE_LIB_DEBUG = ${MORTALENGINE_LIB_DEBUG}")

find_library(RUDEMECHANICALS_LIB_RELEASE NAMES rude-mechanicals)
find_library(RUDEMECHANICALS_LIB_DEBUG NAMES rude-mechanicalsd)

message(STATUS "RUDEMECHANICALS_LIB_RELEASE = ${RUDEMECHANICALS_LIB_RELEASE}")
message(STATUS "RUDEMECHANICALS_LIB_DEBUG = ${RUDEMECHANICALS_LIB_DEBUG}")

add_executable(ui-test
    main.cpp
)

target_include_directories(ui-test PUBLIC
    "."
    ${CMAKE_PREFIX_PATH}/include/rude-mechanicals
    ${CMAKE_PREFIX_PATH}/include/mortal-engine
    ${CMAKE_PREFIX_PATH}/include/lift-sqlite-adapter
    ${CMAKE_PREFIX_PATH}/include/lift-ui
)

target_link_libraries(ui-test
    PUBLIC
    lift-sqlite-adapter
    lift-ui
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    $<$<CONFIG:Debug>:${MORTALENGINE_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${MORTALENGINE_LIB_RELEASE}>
    $<$<CONFIG:Debug>:${RUDEMECHANICALS_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${RUDEMECHANICALS_LIB_RELEASE}>
)
