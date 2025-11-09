#define _POSIX_C_SOURCE 200809L

#include "common_function.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "../cat/cat_function.h"

bool process_files(int argc, char *argv[], Flags *flags) {
  bool error = false;
  init_flags(flags);
  if (argc < 2) {
    if (strcmp(flags->program_name, "cat") == 0) {
      printf("%s: No files specified\n", flags->program_name);
    } else if (strcmp(flags->program_name, "grep") == 0) {
      printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n", flags->program_name);
      printf("Try '%s --help' for more information.\n", flags->program_name);
    }
    error = 1;
  } else {
    char *path_file[argc];
    int count_files = 0;
    if (parse_arguments(argc, argv, flags, path_file, &count_files) == 0) {
      for (int i = 0; i < count_files; i++) {
        if (print_file(path_file[i], flags) != 0) {
          error = true;
        }
      }
    } else {
      error = true;
    }
  }
  return error;
}

void handle_flag_error(const Flags *flags, int *count, char invalid_opt,
                       bool *error) {
  (*count)++;
  if (*count == 1) {
    if (invalid_opt == 'f' && strcmp(flags->program_name, "grep") == 0) {
      printf("%s: option requires an argument -- 'f'\n", flags->program_name);
    } else {
      printf("%s: invalid option -- '%c'\n", flags->program_name, invalid_opt);
    }
    if (strcmp(flags->program_name, "grep") == 0) {
      printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n", flags->program_name);
    }
    printf("Try '%s --help' for more information.\n", flags->program_name);
    *error = true;
  }
}

void collect_files(int argc, char *argv[], char *path_file[],
                   int *count_files) {
  for (int i = optind; i < argc; i++) {
    path_file[(*count_files)++] = argv[i];
  }
}

FILE *read_file(const char *path_file, const Flags *flags, bool *error) {
  FILE *file = NULL;
  file = fopen(path_file, "r");
  if (file == NULL) {
    if (flags->grep.s == false) {
      printf("%s: %s: No such file or directory\n", flags->program_name,
             path_file);
    }
    *error = true;
  }
  return file;
}