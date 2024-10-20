file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.c src/*.cpp)

add_library(lfs OBJECT ${sources})
target_include_directories(lfs PRIVATE include/lfs)
target_include_directories(lfs PUBLIC include)