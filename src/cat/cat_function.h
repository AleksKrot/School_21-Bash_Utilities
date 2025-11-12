#ifndef CAT_FUNCTION_H
#define CAT_FUNCTION_H

#include <stdlib.h>
#include <getopt.h>

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
 * Чтение и вывода результата
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool print_file(const char *path_file, Flags *flags);

/**
 * /Отображение непечатаемых символов
 * @param c - символ
 */
void show_nonprinting(int c);

#endif