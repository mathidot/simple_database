# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(db)

# 设置
set(CMAKE_CXX_STANDARD 11)
set(TEST "test" CACHE STRING "Some user-specified options")
add_subdirectory(src)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if("${TEST}" STREQUAL "db")
    add_executable(db main.c)
    target_link_libraries(db TableFunctions)
else()
    # Link runTests with what we want to test and tSSSShe GTest and pthread library
    add_executable(runTests test.cpp)
    target_link_libraries(runTests TableFunctions ${GTEST_LIBRARIES} pthread)
endif()
