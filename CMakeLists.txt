cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME assistant)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} WIN32 main.cpp
    sources/avatar.cpp)

include_directories(headers)

target_link_libraries(${PROJECT_NAME} d2d1)
target_link_libraries(${PROJECT_NAME} d3d11)


