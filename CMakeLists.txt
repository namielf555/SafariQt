cmake_minimum_required(VERSION 3.16)
project(SafariQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)

qt_add_executable(SafariQt main.cpp)

target_link_libraries(SafariQt PRIVATE Qt6::Widgets Qt6::WebEngineWidgets)