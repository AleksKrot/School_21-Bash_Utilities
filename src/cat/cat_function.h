#ifndef CAT_FUNCTION_H
#define CAT_FUNCTION_H

#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "../common/common_function.h"

/**
 * Инициализация флагов
 * @param flags - флаги
 */
void init_flags(Flags *flags);

/**
 * Парсинг аргументов
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @param path_file - путь к файлу
 * @param count_files - количество файлов
 * @return SUCCESS - выполнено успешно
 * @return FAILURE - произошла ошибка
 */
bool parse_arguments(int argc, char *argv[], Flags *flags, char *path_file[],
                     int *count_files);

/**
 * Чтение и вывода результата
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @return SUCCESS - выполнено успешно
 * @return FAILURE - произошла ошибка
 */
bool print_file(const char *path_file, Flags *flags);

/**
 * /Отображение непечатаемых символов
 * @param c - символ
 */
void show_nonprinting(int c);

#endif