cmake_minimum_required(VERSION 3.11)
project(chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(chip8
        src/main.c
        src/chip8.c
)

target_include_directories(chip8 PRIVATE include)