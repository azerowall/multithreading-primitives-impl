cmake_minimum_required(VERSION 3.1.0)

project(mt-prim-impl)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
