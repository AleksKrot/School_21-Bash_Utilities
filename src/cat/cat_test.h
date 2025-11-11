#ifndef CAT_TEST_H
#define CAT_TEST_H

#include <stdio.h>
#include <string.h>

#include "../common/common_function.h"

#define COMMAND "cat"
#define COMMAND_S21 "./s21_cat"
#define TEST_FILE1 "Test1.txt"
#define TEST_FILE2 "Test2.txt"
#define NON_EXISTENT_FILE "NonexistentTest.txt"

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
 * @param command - команда для стандартной функции cat
 * @param cat_output - вывод стандартной функции cat
 * @param command_s21 - команда для функции s21_cat
 * @param s21_cat_output - вывод функции s21_cat
 */
void create_output_files(const char *command, const char *cat_output,
                         const char *command_s21, const char *s21_cat_output);

/**
 * Сравнение выходных файлов
 * @param cat_output - вывод стандартной функции cat
 * @param s21_cat_output - вывод функции s21_cat
 */
void compare_files(const char *cat_output, const char *s21_cat_output);

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
 * Запуск тестов с длинными флагами
 */
void run_long_option_tests();

/**
 * Запуск тестов с комбинированными флагами
 */
void run_combination_tests();

/**
 * Запуск всех тестов
 */
void run_all_tests();

#endif