#ifndef CAT_TEST_H
#define CAT_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND "cat"
#define MY_COMMAND "./my_cat"
#define OUTPUT_FILE "cat_output.txt"
#define MY_OUTPUT_FILE "my_cat_output.txt"
#define TEST_FILE1 "Test1.txt"
#define TEST_FILE2 "Test2.txt"
#define NON_EXISTENT_FILE "NonexistentTest.txt"

typedef struct {
  const char *description;
  const char *command;
  const char *my_command;
} TestCase;

/**
 * Создание тестовых файлов
 */
void create_test_files();

/**
 * Создание выходных файлов
 * @param command - команда для стандартной функции cat
 * @param my_command - команда для функции my_cat
 */
void create_output_files(const char *command, const char *my_command);

/**
 * Сравнение выходных файлов
 */
void compare_files();

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