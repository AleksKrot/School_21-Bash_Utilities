cmake_minimum_required(VERSION 3.28)
project(Bash_Utilities C)

set(CMAKE_C_STANDARD 11)

include_directories(src/cat)
include_directories(src/common)
include_directories(src/grep)

add_executable(my_cat_test
        src/cat/my_cat_test.c
        src/cat/my_cat_function.c
        src/cat/my_cat_function.h
        src/common/common_function.c
        src/common/common_function.h)

add_executable(my_grep_test
        src/grep/grep_test.c
        src/grep/grep_function.c
        src/grep/grep_function.h
        src/common/common_function.c
        src/common/common_function.h)

add_executable(my_cat
        src/cat/my_cat.c
        src/cat/my_cat_function.c
        src/cat/my_cat_function.h
        src/common/common_function.c
        src/common/common_function.h)