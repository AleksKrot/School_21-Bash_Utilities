#ifndef GREP_FUNCTION_H
#define GREP_FUNCTION_H

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
 * Добавление шаблона
 * @param flags - флаги
 * @param pattern - шаблон
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool add_pattern(Flags *flags, const char *pattern);

/**
 * Сохранение шаблона в файл
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool pattern_for_file(const char *path_file, Flags *flags);

/**
 * Обработка случая шаблонов без флагов
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @param optind - количество шаблонов
 */
bool handle_empty_patterns(int argc, char *argv[], Flags *flags, int *optind);

/**
 * Обработка ошибок
 * @param invalid_opt - ошибочный аргумент
 */
void grep_flag_error(char invalid_opt);

/**
 * Обработка одного шаблона
 * @param line - строка
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @param line_number - номер строки
 * @param pattern - шаблон
 * @param line_has_match - совпадение линии
 * @param line_already_printed - линия уже выведена
 * @return SUCCESS - выполнено успешно
 * @return FAILURE - произошла ошибка
 */
bool handle_single_pattern(const char *line, const char *path_file,
                           const Flags *flags, int line_number,
                           const char *pattern, bool *line_has_match,
                           bool *line_already_printed);

/**
 * Вывод информации о названии файла и номере строки
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @param line_number - номер строки
 */
void print_file_info(const char *path_file, const Flags *flags,
                     int line_number);

/**
 * Чтение и вывода результата
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @return EXIT_SUCCESS - выполнено успешно
 * @return EXIT_FAILURE - произошла ошибка
 */
bool print_file(const char *argv, Flags *flags);

/**
 * Очистка флагов
 * @param flags - флаги
 */
void free_flags(Flags *flags);

#endif