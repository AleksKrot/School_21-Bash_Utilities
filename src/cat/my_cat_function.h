#ifndef CAT_FUNCTION_H
#define CAT_FUNCTION_H

#include <getopt.h>
#include <stdlib.h>

#include "../common/common_function.h"

/**
 * Обработка аргументов и вывод результата
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool process_files(int argc, char *argv[], Flags *flags);

/**
 * Парсинг аргументов
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool parse_arguments(int argc, char *argv[], Flags *flags);

/**
 * Обработка ошибок
 * @param invalid_opt - ошибочный аргумент
 */
void cat_flag_error(char invalid_opt);

/**
 * Чтение и вывода результата
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool print_file(const char *argv, Flags *flags);

/**
 * /Отображение непечатаемых символов
 * @param symbol - символ
 */
void show_nonprinting(int symbol);

#endif