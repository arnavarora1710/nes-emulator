cmake_minimum_required(VERSION 3.14)

project(nes-emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GCC_COVERAGE_COMPILE_FLAGS
        -g
        -O3
        -Wall
        -Wextra
        -Wpedantic
)

include(FetchContent)

FetchContent_Declare(
        lexy
        URL https://lexy.foonathan.net/download/lexy-src.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(lexy)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/6502-cpu
        ${CMAKE_CURRENT_SOURCE_DIR}/include/2C02-ppu
        ${CMAKE_CURRENT_SOURCE_DIR}/include/buses
        ${CMAKE_CURRENT_SOURCE_DIR}/include/memory-devices
        ${CMAKE_CURRENT_SOURCE_DIR}/include/third-party
)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${ALL_SRC})

target_compile_definitions(${PROJECT_NAME} PRIVATE
        PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE foonathan::lexy)

target_compile_options(${PROJECT_NAME} PRIVATE ${GCC_COVERAGE_COMPILE_FLAGS})