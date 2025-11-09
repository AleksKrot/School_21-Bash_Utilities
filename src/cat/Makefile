CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=200809L
SRC_DIR = .
COMMON_DIR = ../common

.PHONY: all s21_cat test clean rebuild clang_format lint cppcheck valgrind

all: s21_cat test clang_format lint cppcheck valgrind

s21_cat:
		$(CC) $(CFLAG) $(SRC_DIR)/cat.c $(SRC_DIR)/cat_function.c $(COMMON_DIR)/common_function.c -o $(SRC_DIR)/s21_cat

test:
		$(CC) $(CFLAG) $(SRC_DIR)/cat_test.c $(SRC_DIR)/cat_function.c $(COMMON_DIR)/common_function.c -o $(SRC_DIR)/cat_test
		$(SRC_DIR)/cat_test

clean: 
		rm -f $(SRC_DIR)/s21_cat $(SRC_DIR)/cat_test $(SRC_DIR)/*.txt

rebuild: clean all

clang_format:
		clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h

lint: 
		clang-format -n $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h

cppcheck:
		cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h

valgrind: 
		valgrind --tool=memcheck --leak-check=yes $(SRC_DIR)/s21_cat -benst *.txt