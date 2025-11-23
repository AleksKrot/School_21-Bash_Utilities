#include "my_cat_function.h"

bool process_files(int argc, char *argv[], Flags *flags) {
  bool error = EXIT_SUCCESS;
  if (argc < 2) {
    printf("cat: No files specified\n");
    error = EXIT_FAILURE;
  } else {
    error = parse_arguments(argc, argv, flags);
  }
  return error;
}

bool parse_arguments(int argc, char *argv[], Flags *flags) {
  bool error = EXIT_SUCCESS;
  int opt = 0;
  const struct option long_opt[] = {{"number-nonblank", no_argument, 0, 'b'},
                                    {"number", no_argument, 0, 'n'},
                                    {"squeeze-blank", no_argument, 0, 's'},
                                    {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "beEnstT", long_opt, NULL)) != -1) {
    switch (opt) {
      case 'n':
        flags->n = true;
        break;
      case 'e':
        flags->e = flags->v = true;
        break;
      case 'E':
        flags->E = true;
        break;
      case 'b':
        flags->b = true;
        flags->n = false;
        break;
      case 's':
        flags->s = true;
        break;
      case 't':
        flags->t = flags->v = true;
        break;
      case 'T':
        flags->T = true;
        break;
      case 'v':
        flags->v = true;
        break;
      default:
        error = EXIT_FAILURE;
        cat_flag_error(optopt);
        opt = -1;
        break;
    }
  }
  return error;
}

void cat_flag_error(char invalid_opt) {
  printf("cat: invalid option -- '%c'\n", invalid_opt);
  printf("Try 'cat --help' for more information.\n");
}

bool print_file(const char *argv, Flags *flags, const char *program_name) {
  bool error = false;
  FILE *file = open_file(argv, flags, program_name, &error);
  if (!error) {
    int current_symbol;
    int previous_symbol = flags->end_file_symbol;
    int count_line_break = 0;
    while ((current_symbol = getc(file)) != EOF) {
      int should_print = 1;
      if (flags->s) {
        if (current_symbol == '\n' && previous_symbol == '\n') {
          count_line_break++;
        } else {
          count_line_break = 0;
        }
        if (count_line_break > 1) {
          should_print = 0;
        }
      }
      if (should_print && (flags->n || flags->b) &&
          (previous_symbol == EOF || previous_symbol == '\n')) {
        if ((flags->b && current_symbol != '\n') || (flags->n && !flags->b)) {
          printf("%6d\t", flags->num++);
        }
      }
      if (should_print && (flags->E || flags->e) && current_symbol == '\n') {
        putchar('$');
      }
      if (should_print && (flags->T || flags->t) && current_symbol == '\t') {
        printf("^I");
        should_print = 0;
      }
      if (should_print && flags->v) {
        show_nonprinting(current_symbol);
        should_print = 0;
      }
      if (should_print) {
        putchar(current_symbol);
      }
      previous_symbol = current_symbol;
    }
    flags->end_file_symbol = previous_symbol;
    fclose(file);
  }
  return error;
}

void show_nonprinting(int c) {
  if (c >= 128) {
    printf("M-");
    c = c - 128;
  }
  if (c == '\n' || c == '\t' || (c >= 32 && c <= 126)) {
    putchar(c);
  } else if (c < 32) {
    putchar('^');
    putchar(c + 64);
  } else if (c == 127) {
    putchar('^');
    putchar(c - 64);
  }
}