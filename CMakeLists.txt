cmake_minimum_required(VERSION 3.14)

project(lift-sqlite-adapter)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(lift-sqlite-adapter)
add_subdirectory(lift-ui)
add_subdirectory(ui-test)
