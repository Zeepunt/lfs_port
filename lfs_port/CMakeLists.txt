set(sources src/lfs_port_linux.c)

add_library(lfs_port OBJECT ${sources})
target_include_directories(lfs_port PRIVATE include/lfs_port)
target_include_directories(lfs_port PUBLIC include)
target_link_libraries(lfs_port PUBLIC lfs)