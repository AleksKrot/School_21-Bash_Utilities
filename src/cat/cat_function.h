#ifndef CAT_FUNCTION_H
#define CAT_FUNCTION_H

#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "../common/common_function.h"

// Инициализация флагов
void init_flags(Flags *flags);

// Парсер аргументов
bool parse_arguments(int argc, char *argv[], Flags *flags, char *path_file[],
                     int *count_files);

// Функция чтения и вывода результата
bool print_file(const char *path_file, Flags *flags);

// Функция отображения непечатаемых символов
void show_nonprinting(int c);

#endif