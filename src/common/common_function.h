#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Flags {
  bool b;
  bool e;
  bool E;
  bool t;
  bool T;
  int num;
  int end_file_symbol;
  char **pattern;
  int count_pattern;
  bool i;
  bool c;
  bool l;
  bool h;
  char *f;
  bool o;
  bool multi_files;
  bool n;
  bool s;
  bool v;
} Flags;

/**
 * Инициализация флагов
 * @param flags - флаги
 */
void init_flags(Flags *flags);

/**
 * Открытие файла
 * @param argv - массив файлов
 * @param flags - флаги
 * @param program_name
 * @param program_name
 * @param program_name
 * @param error -ошибка
 * @return указатель на открытый файл
 */
FILE *open_file(const char *argv, const Flags *flags, const char *program_name,
                bool *error);

#endif