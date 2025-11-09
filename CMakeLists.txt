cmake_minimum_required(VERSION 3.28)
project(Bash_Utilities C)

set(CMAKE_C_STANDARD 11)

include_directories(src/cat)
include_directories(src/common)
include_directories(src/grep)

add_executable(Bash_Utilities
        src/cat/cat.c
        src/cat/cat_function.c
        src/cat/cat_function.h
        src/cat/cat_test.c
        src/cat/cat_test.h
        src/common/common_function.c
        src/common/common_function.h
        src/grep/grep.c
        src/grep/grep_function.c
        src/grep/grep_function.h
        src/grep/grep_test.c
        src/grep/grep_test.h)
