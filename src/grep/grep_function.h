#ifndef GREP_FUNCTION_H
#define GREP_FUNCTION_H

#define _POSIX_C_SOURCE 200809L

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../common/common_function.h"

// Инициализация флагов
void init_flags(Flags *flags);

// Функция для добавления шаблона
bool add_pattern(Flags *flags, const char *pattern);

// Функция для сохрания шаблона в файл
bool pattern_for_file(const char *path_file, Flags *flags);

// Функция для обработки случая шаблонов без флагов
void handle_empty_patterns(int argc, char *argv[], Flags *flags, int *optind);

// Парсер аргументов
bool parse_arguments(int argc, char *argv[], Flags *flags, char *path_file[],
                     int *count_files);

// Функция для обработки одного шаблона
bool handle_single_pattern(const char *line, const char *path_file,
                           const Flags *flags, int line_number,
                           const char *pattern, bool *line_has_match,
                           bool *line_already_printed);

// Функция для вывода информации о названии файла и номере строки
void print_file_info(const char *path_file, const Flags *flags,
                     int line_number);

// Функция чтения и вывода на экран результата
bool print_file(const char *path_file, Flags *flags);

// Функция для очистки флагов
void free_flags(Flags *flags);

#endif