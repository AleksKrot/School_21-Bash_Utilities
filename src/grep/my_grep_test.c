#include "my_grep_test.h"

void create_test_files() {
  FILE *f1 = fopen(TEST_FILE1, "w");
  FILE *f2 = fopen(TEST_FILE2, "w");
  FILE *f3 = fopen(EMPTY_FILE, "w");
  FILE *f4 = fopen(PATTERN_FILE, "w");

  if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL) {
    printf("Failed to create files\n");
    if (f1 != NULL) {
      fclose(f1);
    }
    if (f2 != NULL) {
      fclose(f2);
    }
    if (f3 != NULL) {
      fclose(f3);
    }
    if (f4 != NULL) {
      fclose(f4);
    }
  } else {
    fprintf(f1,
            "\tSoft kitty, warm kitty,\n\n\n\tLittle ball of fur,\n\n\n\tHappy "
            "kitty, sleepy kitty,\n\n\n\tPurr! purr! purr!\n");
    fclose(f1);

    fprintf(f2,
            "Once there was a little kitty\nWhiter than snow;\nIn a barn she "
            "used to frolic,\nLong time ago.\n\n\n"
            "In the barn a little mouse\nRan to and fro:\nFor she heard the "
            "kitty coming,\nLong time ago.\n\n\n"
            "Two eyes had little kitty.\nBlack as a shoe;\nAnd they spied the "
            "little mouse,\nLong time ago.\n\n\n"
            "Four paws had little kitty,\nPaws soft as dough,\nAnd they caught "
            "the little mouse,\nLong time ago.\n\n\n"
            "Nine teeth had little kitty,\nAll in a row;\nAnd they bit the "
            "little mouse,\nLong time ago.\n\n\n"
            "When the teeth bit little mouse,\nLittle mouse cried "
            "\"Oh!\"\nBut she got away from kitty,\nLong time ago.\n\n\n"
            "Elizabeth Prentiss\n");
    fclose(f2);

    fprintf(f4, "kitty\nLittle\nthe");
    fclose(f4);
  }
}

void create_output_files(const char *command, const char *my_command) {
  char full_command[512];
  char my_full_command[512];
  FILE *cmd_output = NULL;
  FILE *my_cmd_output = NULL;
  FILE *file = NULL;
  FILE *my_file = NULL;

  strcpy(full_command, command);
  strcat(full_command, " 2>&1");
  strcpy(my_full_command, my_command);
  strcat(my_full_command, " 2>&1");

  cmd_output = popen(full_command, "r");
  my_cmd_output = popen(my_full_command, "r");

  if (cmd_output != NULL && my_cmd_output != NULL) {
    file = fopen(OUTPUT_FILE, "w");
    my_file = fopen(MY_OUTPUT_FILE, "w");

    if (file != NULL && my_file != NULL) {
      int c;
      while ((c = getc(cmd_output)) != EOF) {
        fputc(c, file);
      }
      while ((c = getc(my_cmd_output)) != EOF) {
        fputc(c, my_file);
      }
    } else {
      printf("Failed to open output files\n");
    }

    if (file != NULL) {
      fclose(file);
    }
    if (my_file != NULL) {
      fclose(my_file);
    }
  } else {
    printf("Failed to execute command\n");
  }

  if (cmd_output != NULL) {
    pclose(cmd_output);
  }
  if (my_cmd_output != NULL) {
    pclose(my_cmd_output);
  }
}

void compare_files(void) {
  FILE *file = NULL;
  FILE *my_file = NULL;
  int is_same = 1;  // 1 - файлы идентичны

  file = fopen(OUTPUT_FILE, "r");
  my_file = fopen(MY_OUTPUT_FILE, "r");

  if (file != NULL && my_file != NULL) {
    int ch1, ch2;
    while ((ch1 = fgetc(file)) != EOF && (ch2 = fgetc(my_file)) != EOF &&
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
  if (my_file != NULL) {
    fclose(my_file);
  }

  if (is_same == 1) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
}

void run_test_case(TestCase test) {
  printf("Test: %s...\n", test.description);

  create_output_files(test.command, test.my_command);
  compare_files();
}

void run_tests(const TestCase *tests, size_t num_tests) {
  for (size_t i = 0; i < num_tests; i++) {
    run_test_case(tests[i]);
    printf("\n");
  }
}

void run_basic_tests() {
  TestCase tests[] = {
      {"Basic search in single file", COMMAND " " PATTERN_1 " " TEST_FILE1,
       MY_COMMAND " " PATTERN_1 " " TEST_FILE1},
      {"Basic search in multiple files",
       COMMAND " " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Search non-existent file", COMMAND " " PATTERN_1 " " NON_EXISTENT_FILE,
       MY_COMMAND " " PATTERN_1 " " NON_EXISTENT_FILE},
      {"Search empty file", COMMAND " " PATTERN_1 " " EMPTY_FILE,
       MY_COMMAND " " PATTERN_1 " " EMPTY_FILE}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_flag_tests() {
  TestCase tests[] = {
      {"Non-existent t-flag", COMMAND " -t " PATTERN_1 " " TEST_FILE2,
       MY_COMMAND " -t " PATTERN_1 " " TEST_FILE2},
      {"Flag -i (ignore register)",
       COMMAND " -i " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -i " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -e (pattern)",
       COMMAND " -e " PATTERN_1 " -e " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -e " PATTERN_1 " -e " PATTERN_2 " " TEST_FILE1
                   " " TEST_FILE2},
      {"Flag -v (invert match)",
       COMMAND " -v " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -v " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -c (count matches)",
       COMMAND " -c " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -c " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -l (files with matches)",
       COMMAND " -l " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -l " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -n (line numbers)",
       COMMAND " -n " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -n " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -s (suppress error messages)",
       COMMAND " -s " PATTERN_1 " " NON_EXISTENT_FILE,
       MY_COMMAND " -s " PATTERN_1 " " NON_EXISTENT_FILE},
      {"Flag -h (suppress file names)",
       COMMAND " -h " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -h " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -o (only matching parts)",
       COMMAND " -o " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -o " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flag -f (read patterns from file)",
       COMMAND " -f " PATTERN_FILE " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -f " PATTERN_FILE " " TEST_FILE1 " " TEST_FILE2}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_combination_tests() {
  TestCase tests[] = {
      {"Multi-flags -iv",
       COMMAND " -iv " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -iv " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -in",
       COMMAND " -in " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -in " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -ic",
       COMMAND " -ic " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -ic " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -il",
       COMMAND " -il " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -il " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -ih",
       COMMAND " -ih " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -ih " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -io",
       COMMAND " -io " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -io " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2},
      {"Multi-flags -vn",
       COMMAND " -vn " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -vn " PATTERN_2 " " TEST_FILE1 " " TEST_FILE2},
      {"Flags -c + -l",
       COMMAND " -c -l " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -c -l " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flags -n + -o",
       COMMAND " -n -o " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2,
       MY_COMMAND " -n -o " PATTERN_1 " " TEST_FILE1 " " TEST_FILE2},
      {"Flags -e + -f",
       COMMAND " -e " PATTERN_1 " -f " PATTERN_FILE " " TEST_FILE1
               " " TEST_FILE2,
       MY_COMMAND " -e " PATTERN_1 " -f " PATTERN_FILE " " TEST_FILE1
                   " " TEST_FILE2},
      {"Flags -h + -s",
       COMMAND " -h -s " PATTERN_1 " " NON_EXISTENT_FILE " " TEST_FILE1,
       MY_COMMAND " -h -s " PATTERN_1 " " NON_EXISTENT_FILE " " TEST_FILE1}};
  run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}

void run_all_tests() {
  create_test_files();
  run_basic_tests();
  run_flag_tests();
  run_combination_tests();
}

int main() {
  run_all_tests();
  return 0;
}