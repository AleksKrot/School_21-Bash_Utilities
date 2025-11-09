#include "cat_function.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common_function.h"

void init_flags(Flags *flags) {
  flags->cat.b = false;
  flags->cat.e = false;
  flags->cat.E = false;
  flags->cat.n = false;
  flags->cat.s = false;
  flags->cat.t = false;
  flags->cat.T = false;
  flags->cat.v = false;
  flags->cat.num = 1;
  flags->cat.last_c_file = EOF;
  flags->error = false;
  flags->grep.s = false;
  flags->program_name = "cat";
}

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
        flags->cat.b = true;
        break;
      case 'e':
        flags->cat.e = flags->cat.v = true;
        break;
      case 'E':
        flags->cat.E = true;
        break;
      case 'n':
        flags->cat.n = true;
        break;
      case 's':
        flags->cat.s = true;
        break;
      case 't':
        flags->cat.t = flags->cat.v = true;
        break;
      case 'T':
        flags->cat.T = true;
        break;
      case 'v':
        flags->cat.v = true;
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
    int last_c = flags->cat.last_c_file;  // Для предыдущего символа
    int count_line_break = 0;  // Для подсчета количества переносов строк
    while ((c = getc(file)) != EOF) {
      int should_print = 1;
      if (flags->cat.s) {
        if (c == '\n' && last_c == '\n') {
          count_line_break++;
        } else {
          count_line_break = 0;
        }
        if (count_line_break > 1) {
          should_print = 0;
        }
      }
      if (should_print && (flags->cat.n || flags->cat.b) &&
          (last_c == EOF || last_c == '\n')) {
        if ((flags->cat.b && c != '\n') || (flags->cat.n && !flags->cat.b)) {
          printf("%6d\t", flags->cat.num++);
        }
      }
      if (should_print && (flags->cat.E || flags->cat.e) && c == '\n') {
        putchar('$');
      }
      if (should_print && (flags->cat.T || flags->cat.t) && c == '\t') {
        printf("^I");
        should_print = 0;
      }
      if (should_print && flags->cat.v) {
        show_nonprinting(c);
        should_print = 0;
      }
      if (should_print) {
        putchar(c);
      }
      last_c = c;
    }
    flags->cat.last_c_file = last_c;
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