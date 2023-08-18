cmake_minimum_required(VERSION 3.13)  # CMake version check

# global vars cmake vars used for both projects
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SOURCE_FILES ${SRC_DIR}/main.c client.c ignore.c log.c storage.c track.c menu.c)

# prod build
project(zren)
add_executable(zren ${SOURCE_FILES})
target_include_directories(zren PUBLIC ${SRC_DIR})
set(C_STANDARD 99)
# gcc documentation recommends -O2 over -O3 because -O3 tends to be a little less predictable.  
# -Ofast was overlooked due to its optimizations not being valid for standard compliant programs.
set(GCC_COMPILE_RELEASE_OPTIONS "-O2")
target_compile_options(zren PUBLIC "$<$<CONFIG:RELEASE>:${GCC_COMPILE_RELEASE_OPTIONS}>")

# debug build
project(zren_debug)
add_executable(zren_debug ${SOURCE_FILES})
target_include_directories(zren_debug PUBLIC ${SRC_DIR})
set(C_STANDARD 99)
# note that -g3 is used instead of -ggdb3, this is for flexibility accross multiple debuggers.
set(GCC_COMPILE_DEBUG_OPTIONS "-O0 -Wall -g3")
target_compile_options(zren_debug PUBLIC "$<$<CONFIG:DEBUG>:${GCC_COMPILE_DEBUG_OPTIONS}>")
