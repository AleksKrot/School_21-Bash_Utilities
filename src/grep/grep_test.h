#ifndef GREP_TEST_H
#define GREP_TEST_H

#define POSIX_C_SOURCE 200809L

#include <stdio.h>

typedef struct {
  const char *description;
  const char *command;
  const char *command_s21;
} TestCase;

/**
 * Создание тестовых файлов
 */
void create_test_files();

/**
 * Создание выходных файлов
 * @param command - команда для стандартной функции grep
 * @param grep_output - вывод стандартной функции grep
 * @param command_s21 - команда для функции s21_grep
 * @param s21_grep_output - вывод функции s21_grep
 */
void create_output_files(const char *command, const char *grep_output,
                         const char *command_s21, const char *s21_grep_output);

/**
 * Сравнение выходных файлов
 * @param grep_output - вывод стандартной функции grep
 * @param s21_grep_output - вывод функции s21_grep
 */
void compare_files(const char *grep_output, const char *s21_grep_output);

/**
 * Запуск одного теста
 * @param test - тест
 */
void run_test_case(TestCase test);

/**
 * Запуск группы тестов
 * @param tests - тесты
 * @param num_tests - количество тестов
 */
void run_tests(const TestCase *tests, size_t num_tests);

/**
 * Запуск тестов с файлами без флагов
 */
void run_basic_tests();

/**
 * Запуск тестов с флагами
 */
void run_flag_tests();

/**
 * Запуск тестов с комбинированными флагами
 */
void run_combination_tests();

/**
 * Запуск всех тестов
 */
void run_all_tests();

#endif