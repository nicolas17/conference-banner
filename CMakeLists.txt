cmake_minimum_required(VERSION 2.8.11)

project(foss4g-banner)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Quick)

add_executable(foss4g-banner WIN32 main.cpp)

target_link_libraries(foss4g-banner Qt5::Quick)
