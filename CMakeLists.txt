cmake_minimum_required(VERSION 3.14)

project(nes-emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GCC_COVERAGE_COMPILE_FLAGS -g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/6502-cpu
        ${CMAKE_CURRENT_SOURCE_DIR}/include/third-party
)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${ALL_SRC})

target_compile_definitions(${PROJECT_NAME} PRIVATE
        PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)
