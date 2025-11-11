#include "cat_test.h"

#include "../cat/cat_function.h"

void create_test_files() {
  FILE *f1 = fopen(TEST_FILE1, "w");
  FILE *f2 = fopen(TEST_FILE2, "w");
  if (f1 == NULL || f2 == NULL) {
    printf("Failed to create files\n");
    if (f1 != NULL) {
      fclose(f1);
    } else {
      fclose(f2);
    }
  } else {
    fprintf(f1,
            "\tSoft kitty, warm kitty,\n\n\n\tLittle ball of fur,\n\n\n\tHappy "
            "kitty, sleepy kitty,\n\n\n\tPurr! purr! purr!\n\n\n\x01\x7F\xFF");
    fclose(f1);

    fprintf(
        f2,
        "Freddy Krueger\nOne, Two, Freddy's coming for you,\nThree, Four, "
        "better lock your door,\n\n\nFive, Six, grab your crucifix,\nSeven, "
        "Eight, gonna stay up late,\nNine, Ten, never sleep "
        "again...\n\n\n\t\x05\x8F\xFF\n");
    fclose(f2);
  }
}

void create_output_files(const char *command, const char *cat_output,
                         const char *command_s21, const char *s21_cat_output) {
  char full_command[512];
  char full_command_s21[512];
  FILE *cmd_output = NULL;
  FILE *cmd_output_s21 = NULL;
  FILE *file = NULL;
  FILE *file_s21 = NULL;

  strcpy(full_command, command);
  strcat(full_command, " 2>&1");
  strcpy(full_command_s21, command_s21);
  strcat(full_command_s21, " 2>&1");

  cmd_output = popen(full_command, "r");
  cmd_output_s21 = popen(full_command_s21, "r");

  if (cmd_output != NULL && cmd_output_s21 != NULL) {
    file = fopen(cat_output, "w");
    file_s21 = fopen(s21_cat_output, "w");

    if (file != NULL && file_s21 != NULL) {
      int c;  // Для хранения текущего символа
      while ((c = getc(cmd_output)) != EOF) {
        fputc(c, file);
      }
      while ((c = getc(cmd_output_s21)) != EOF) {
        fputc(c, file_s21);
      }
    } else {
      printf("Failed to open output files\n");
    }

    if (file != NULL) {
      fclose(file);
    }
    if (file_s21 != NULL) {
      fclose(file_s21);
    }
  } else {
    printf("Failed to execute command\n");
  }

  if (cmd_output != NULL) {
    pclose(cmd_output);
  }
  if (cmd_output_s21 != NULL) {
    pclose(cmd_output_s21);
  }
}

void compare_files(const char *cat_output, const char *s21_cat_output) {
  FILE *file = NULL;
  FILE *file_s21 = NULL;
  int is_same = 1;  // 1 - файлы идентичны

  file = fopen(cat_output, "r");
  file_s21 = fopen(s21_cat_output, "r");

  if (file != NULL && file_s21 != NULL) {
    int ch1, ch2;
    while ((ch1 = fgetc(file)) != EOF && (ch2 = fgetc(file_s21)) != EOF &&
           is_same == 1) {
      if (ch1 != ch2) {
        is_same = 0;
      }
    }
  } else {
    printf("Failed to open output files");
    is_same = 0;
  }
  if (file != NULL) {
    fclose(file);
  }
  if (file_s21 != NULL) {
    fclose(file_s21);
  }

  if (is_same == 1) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
}

void run_test_case(TestCase test) {
  const char *cat_output = "cat_output.txt";
  const char *s21_cat_output = "s21_cat_output.txt";

  printf("Test: %s...\n", test.description);

  create_output_files(test.command, cat_output, test.command_s21,
                      s21_cat_output);
  compare_files(cat_output, s21_cat_output);
}

void run_tests(const TestCase *tests, size_t num_tests) {
  for (size_t i = 0; i < num_tests; i++) {
    run_test_case(tests[i]);
    printf("\n");
  }
}

void run_basic_tests() {
  TestCase tests[] = {
      {"Single file test", COMMAND " " TEST_FILE1, COMMAND_S21 " " TEST_FILE1},
      {"Multiple files test", COMMAND " " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " " TEST_FILE1 " " TEST_FILE2},
      {"Non-existent file", COMMAND " " NON_EXISTENT_FILE,
       COMMAND_S21 " " NON_EXISTENT_FILE}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_flag_tests() {
  TestCase tests[] = {
      {"Non-existent d-flag", COMMAND " -d " NON_EXISTENT_FILE,
       COMMAND_S21 " -d " NON_EXISTENT_FILE},
      {"Flag -b (number non-blank)", COMMAND " -b " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -b " TEST_FILE1 " " TEST_FILE2},
      {"Flag -e (show $ and non-printing)",
       COMMAND " -e " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -e " TEST_FILE1 " " TEST_FILE2},
      {"Flag -E (show $)", COMMAND " -E " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -E " TEST_FILE1 " " TEST_FILE2},
      {"Flag -n (number all)", COMMAND " -n " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -n " TEST_FILE1 " " TEST_FILE2},
      {"Flag -s (squeeze blank)", COMMAND " -s " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -s " TEST_FILE1 " " TEST_FILE2},
      {"Flag -t (show tabs and non-printing)",
       COMMAND " -t " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -t " TEST_FILE1 " " TEST_FILE2},
      {"Flag -T (show tabs)", COMMAND " -T " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -T " TEST_FILE1 " " TEST_FILE2}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_long_option_tests() {
  TestCase tests[] = {
      {"Long option --number-nonblank",
       COMMAND " --number-nonblank " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " --number-nonblank " TEST_FILE1 " " TEST_FILE2},
      {"Long option --number", COMMAND " --number " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " --number " TEST_FILE1 " " TEST_FILE2},
      {"Long option --squeeze-blank",
       COMMAND " --squeeze-blank " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " --squeeze-blank " TEST_FILE1 " " TEST_FILE2}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_combination_tests() {
  TestCase tests[] = {
      {"Flags -n and -s and -E and -T",
       COMMAND " -nsET " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -nsET " TEST_FILE1 " " TEST_FILE2},
      {"Flags -b and -s and e", COMMAND " -bse " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -bse " TEST_FILE1 " " TEST_FILE2},
      {"Flags -b and -n and t", COMMAND " -bnt " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -bnt " TEST_FILE1 " " TEST_FILE2},
      {"All flags", COMMAND " -benst " TEST_FILE1 " " TEST_FILE2,
       COMMAND_S21 " -benst " TEST_FILE1 " " TEST_FILE2},
  };
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_all_tests() {
  create_test_files();
  run_basic_tests();
  run_flag_tests();
  run_long_option_tests();
  run_combination_tests();
}

int main() {
  run_all_tests();
  return 0;
}