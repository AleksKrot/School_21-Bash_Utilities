#include "cat_function.h"

bool parse_arguments(int argc, char *argv[], Flags *flags, char *path_file[],
                     int *count_files) {
  int opt;
  int count = 0;
  bool error = false;
  const struct option long_opt[] = {{"number-nonblank", no_argument, 0, 'b'},
                                    {"number", no_argument, 0, 'n'},
                                    {"squeeze-blank", no_argument, 0, 's'},
                                    {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "beEnstT", long_opt, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags->b = true;
        break;
      case 'e':
        flags->e = flags->v = true;
        break;
      case 'E':
        flags->E = true;
        break;
      case 'n':
        flags->n = true;
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
        flags->error = true;
        handle_flag_error(flags, &count, optopt, &error);
        opt = -1;
        break;
    }
  }
  collect_files(argc, argv, path_file, count_files);
  return error;
}

bool print_file(const char *path_file, Flags *flags) {
  bool error = false;
  FILE *file = open_file(path_file, flags, &error);
  if (!error) {
    int c;  // Для хранения текущего символа
    int last_c = flags->last_c_file;  // Для предыдущего символа
    int count_line_break = 0;  // Для подсчёта количества переносов строк
    while ((c = getc(file)) != EOF) {
      int should_print = 1;
      if (flags->s) {
        if (c == '\n' && last_c == '\n') {
          count_line_break++;
        } else {
          count_line_break = 0;
        }
        if (count_line_break > 1) {
          should_print = 0;
        }
      }
      if (should_print && (flags->n || flags->b) &&
          (last_c == EOF || last_c == '\n')) {
        if ((flags->b && c != '\n') || (flags->n && !flags->b)) {
          printf("%6d\t", flags->num++);
        }
      }
      if (should_print && (flags->E || flags->e) && c == '\n') {
        putchar('$');
      }
      if (should_print && (flags->T || flags->t) && c == '\t') {
        printf("^I");
        should_print = 0;
      }
      if (should_print && flags->v) {
        show_nonprinting(c);
        should_print = 0;
      }
      if (should_print) {
        putchar(c);
      }
      last_c = c;
    }
    flags->last_c_file = last_c;
    fclose(file);
  }
  return error;
}

void show_nonprinting(int c) {
  if (c != '\n' && c != '\t') {
    if (c < 32) {
      putchar('^');
      putchar(c + 64);
    } else if (c == 127) {
      putchar('^');
      putchar(c - 64);
    } else if (c > 127) {
      printf("M-");
      if (c - 128 < 32) {
        putchar('^');
        putchar(c - 128 + 64);
      } else if (c - 128 == 127) {
        putchar('^');
        putchar(c - 128 - 64);
      } else {
        putchar(c - 128);
      }
    } else {
      putchar(c);
    }
  } else {
    putchar(c);
  }
}