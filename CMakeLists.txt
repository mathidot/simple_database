# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(db)

# 设置
set(CMAKE_CXX_STANDARD 11)
set(TEST "db" CACHE STRING "Some user-specified options")
add_subdirectory(src)

add_executable(db main.c)
target_link_libraries(db TableFunctions)
