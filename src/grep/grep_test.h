#ifndef GREP_TEST_H
#define GREP_TEST_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

// Структура для описания тестов
typedef struct {
  const char *description;
  const char *command;
  const char *command_s21;
} TestCase;

// Cлздание тестовых файлов
void create_test_files();

// Создание выходных файлов
void create_output_files(const char *command, const char *grep_output,
                         const char *command_s21, const char *s21_grep_output);

// Сравнение выходных файлов
void compare_files(const char *grep_output, const char *s21_grep_output);

// Для выполнения одного теста
void run_test_case(TestCase test);

// Для запуска тестов
void run_tests(const TestCase *tests, size_t num_tests);

// Для выполнения тестов с файлами без флагов
void run_basic_tests();

// Для выполнения тестов с флагами
void run_flag_tests();

// Для выполнения комбинированных флагов
void run_combination_tests();

// Для выполнения всех тестов
void run_all_tests();

#endif