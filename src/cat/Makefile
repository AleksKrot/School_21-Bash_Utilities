CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=200809L
SRC_DIR = .
COMMON_DIR = ../common
BUILD_DIR = ../../build

all: my_cat
.PHONY: all

my_cat:
		$(CC) $(CFLAG) $(SRC_DIR)/my_cat.c $(SRC_DIR)/my_cat_function.c $(COMMON_DIR)/common_function.c -o $(BUILD_DIR)/my_cat
.PHONY: my_cat

test:	my_cat
		$(CC) $(CFLAG) $(SRC_DIR)/my_cat_test.c $(SRC_DIR)/my_cat_function.c $(COMMON_DIR)/common_function.c -o $(BUILD_DIR)/my_cat_test
		cd $(BUILD_DIR)/ && ./my_cat_test
.PHONY: test

clean: 
		rm -f $(BUILD_DIR)/my_cat $(BUILD_DIR)/my_cat_test $(BUILD_DIR)/*.txt
.PHONY: clean

cfn:
		clang-format -n $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h
.PHONY: cfn

cfi:
		clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h
.PHONY: cfi

cppcheck:
		cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h
.PHONY: cppcheck

valgrind: test
		valgrind --tool=memcheck --leak-check=yes $(BUILD_DIR)/my_cat_test
.PHONY: valgrind