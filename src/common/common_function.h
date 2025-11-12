#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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
  const char *program_name;
} Flags;

/**
 * Инициализация флагов
 * @param flags - флаги
 */
void init_flags(Flags *flags);

/**
 * Вывод сообщения об ошибке
 */
void print_error();

/**
 * Обработка ошибок
 * @param flags - флаги
 * @param invalid_opt - ошибочный аргумент
 */
void handle_flag_error(const Flags *flags, char invalid_opt);

/**
 * Сбор массива файлов
 * @param argc - количество аргументов командной строки
 * @param argv - массив аргументов командной строки
 */
void collect_files(int argc, char *argv[]);

/**
 * Открытие файла
 * @param path_file - путь к файлу
 * @param flags - флаги
 * @param error -ошибка
 * @return указатель на открытый файл
 */
FILE *open_file(const char *path_file, const Flags *flags, bool *error);

#endif