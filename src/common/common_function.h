#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#define POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  bool b;
  bool e;
  bool E;
  bool n;
  bool s;
  bool t;
  bool T;
  bool v;
  int num;
  int last_c_file;
} CatFlags;

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
  bool multi_files;
} GrepFlags;

typedef struct {
  CatFlags cat;
  GrepFlags grep;
  bool error;
  const char *program_name;
} Flags;

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