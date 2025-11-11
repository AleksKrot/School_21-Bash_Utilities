#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef CAT_FUNCTION_H
#include "../cat/cat_function.h"
#endif

#ifdef GREP_FUNCTION_H
#include "../grep/grep_function.h"
#endif

typedef struct Flags {
#ifdef CAT_FUNCTION_H
  bool b;
  bool e;
  bool E;
  bool t;
  bool T;
  int num;
  int last_c_file;
#endif
#ifdef GREP_FUNCTION_H
  char **pattern;
  int count_pattern;
  bool i;
  bool c;
  bool l;
  bool h;
  char *f;
  bool o;
  bool multi_files;
#endif
  bool n;
  bool s;
  bool v;
  bool error;
  const char *program_name;
} Flags;

/**
 * Вывод сообщения об ошибке
 */
void print_error();

/**
 * Обработка аргументов и вывод результата
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param flags - флаги
 * @return SUCCESS - выполнено успешно
 * @return FAILURE - произошла ошибка
 */
bool process_files(int argc, char *argv[], Flags *flags);

/**
 * Обработка ошибок
 * @param flags - флаги
 * @param count - количество аргументов
 * @param invalid_opt - ошибочный аргумент
 * @param error - ошибка
 */
void handle_flag_error(const Flags *flags, int *count, char invalid_opt,
                       bool *error);

/**
 * Сбор массива файлов
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 * @param path_file - путь к файлу
 * @param count_files - количество файлов
 */
void collect_files(int argc, char *argv[], char *path_file[], int *count_files);

/**
 * Открытие файла
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @param error -ошибка
 * @return указатель на открытый файл
 */
FILE *open_file(const char *path_file, const Flags *flags, bool *error);

#endif