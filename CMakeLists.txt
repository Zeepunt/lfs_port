cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(lfs_port LANGUAGES C CXX)

add_subdirectory(lfs)
add_subdirectory(lfs_port)

add_executable(mklfs mklfs.c)
target_link_libraries(mklfs lfs lfs_port)

add_executable(lfs_simulator main.c)
target_link_libraries(lfs_simulator lfs lfs_port)