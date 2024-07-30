# Special thanks to: https://github.com/ttroy50/cmake-examples

cmake_minimum_required(VERSION 3.10)

project(scs VERSION 0.1.0)

set(
        SOURCES
        source/scs/arrayhelper.c
        source/scs/scs.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC ${PROJECT_BINARY_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)


target_compile_options(
        ${PROJECT_NAME}
        PRIVATE -Wall -Werror -Wextra
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)


add_subdirectory(examples)
add_subdirectory(test)
