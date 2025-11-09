#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>

// Структура флагов cat
typedef struct {
  bool b;
  bool e;
  bool E;
  bool n;
  bool s;
  bool t;
  bool T;
  bool v;
  int num;          // Для нумерации строк
  int last_c_file;  // Последний символ в файле
} CatFlags;

// Структура флагов grep
typedef struct {
  char **pattern;
  int count_pattern;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  char *f;
  bool o;
  bool multifiles;
} GrepFlags;

// Структура флагов
typedef struct {
  CatFlags cat;
  GrepFlags grep;
  bool error;
  const char *program_name;
} Flags;

// Функция обработки аргументов и для вывода результата
bool process_files(int argc, char *argv[], Flags *flags);

// Обработка ошибок
void handle_flag_error(const Flags *flags, int *count, char invalid_opt,
                       bool *error);

// Функция сбора массива файлов
void collect_files(int argc, char *argv[], char *path_file[], int *count_files);

// Функция чтения файла
FILE *read_file(const char *path_file, const Flags *flags, bool *error);

#endif