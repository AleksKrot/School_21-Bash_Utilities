#include "grep_function.h"

#include <stdlib.h>

#include "../common/common_function.h"

void init_flags(Flags *flags) {
  flags->grep.pattern = NULL;
  flags->grep.count_pattern = 0;
  flags->grep.i = false;
  flags->grep.v = false;
  flags->grep.c = false;
  flags->grep.l = false;
  flags->grep.n = false;
  flags->grep.h = false;
  flags->grep.s = false;
  flags->grep.f = NULL;
  flags->grep.o = false;
  flags->error = false;
  flags->grep.multi_files = false;
  flags->program_name = "grep";
}

bool add_pattern(Flags *flags, const char *pattern) {
  bool error = false;
  char **temp = realloc(flags->grep.pattern,
                        (flags->grep.count_pattern + 1) * sizeof(char *));
  if (temp == NULL) {
    printf("Failed to allocate memory for pattern array");
    error = true;
  } else {
    flags->grep.pattern = temp;
    char *new_pattern = strdup(pattern);
    if (new_pattern == NULL) {
      printf("Failed to allocate memory for new pattern");
      free(flags->grep.pattern);
      flags->grep.pattern = NULL;
      flags->grep.count_pattern = 0;
      error = true;
    } else {
      flags->grep.pattern[flags->grep.count_pattern] = new_pattern;
      flags->grep.count_pattern++;
    }
  }
  return error;
}

bool pattern_for_file(const char *path_file, Flags *flags) {
  bool error = false;
  char *line = NULL;
  FILE *file = open_file(path_file, flags, &error);
  if (!error) {
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1 && !error) {
      if (read > 0 && line[read - 1] == '\n') {
        line[read - 1] = '\0';
        read--;
      }
      if (read > 0) {
        if (add_pattern(flags, line) == true) {
          error = true;
        }
      }
    }
    fclose(file);
  }
  if (line != NULL) {
    free(line);
  }
  return error;
}

void handle_empty_patterns(int argc, char *argv[], Flags *flags, int *optind) {
  if (!flags->grep.count_pattern) {
    if (*optind < argc) {
      if (add_pattern(flags, argv[*optind])) {
        flags->error = true;
      }
      (*optind)++;
    } else if (!flags->error) {
      printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
      printf("Try 'grep --help' for more information.\n");
      flags->error = true;
    }
  }
}

bool parse_arguments(int argc, char *argv[], Flags *flags, char *path_file[],
                     int *count_files) {
  int opt;
  int count = 0;
  bool error = false;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        if (add_pattern(flags, optarg)) {
          opt = -1;
        }
        break;
      case 'i':
        flags->grep.i = true;
        break;
      case 'v':
        flags->grep.v = true;
        break;
      case 'c':
        flags->grep.c = true;
        break;
      case 'l':
        flags->grep.l = true;
        break;
      case 'n':
        flags->grep.n = true;
        break;
      case 'h':
        flags->grep.h = true;
        break;
      case 's':
        flags->grep.s = true;
        break;
      case 'f':
        if (pattern_for_file(optarg, flags)) {
          opt = -1;
        }
        break;
      case 'o':
        flags->grep.o = true;
        break;
      default:
        flags->error = true;
        handle_flag_error(flags, &count, optopt, &error);
        opt = -1;
        break;
    }
  }
  handle_empty_patterns(argc, argv, flags, &optind);
  collect_files(argc, argv, path_file, count_files);
  if (*count_files > 1) {
    flags->grep.multi_files = true;
  }
  return error;
}

void print_file_info(const char *path_file, const Flags *flags,
                     int line_number) {
  if (flags->grep.multi_files && !flags->grep.h) {
    printf("%s:", path_file);
  }
  if (flags->grep.n && !flags->grep.c) {
    printf("%d:", line_number);
  }
}

bool handle_single_pattern(const char *line, const char *path_file,
                           const Flags *flags, int line_number,
                           const char *pattern, bool *line_has_match,
                           bool *line_already_printed) {
  regex_t regex;
  int reg_flags = REG_EXTENDED;
  if (flags->grep.i) {
    reg_flags = reg_flags | REG_ICASE;
  }
  if (regcomp(&regex, pattern, reg_flags) == 0) {
    regmatch_t match;
    const char *ptr = line;
    int match_result = regexec(&regex, ptr, 1, &match, 0);
    while (!match_result && !(*line_already_printed)) {
      *line_has_match = true;
      if (!flags->grep.v && !flags->grep.c && !flags->grep.l) {
        print_file_info(path_file, flags, line_number);
        if (flags->grep.o) {
          printf("%.*s\n", (int)(match.rm_eo - match.rm_so), ptr + match.rm_so);
        } else {
          printf("%s\n", line);
          *line_already_printed = true;
        }
      }
      ptr = ptr + match.rm_eo;
      if (match.rm_so == match.rm_eo) {
        ptr = (*ptr == '\0') ? ptr : ptr + 1;
      }
      match_result = regexec(&regex, ptr, 1, &match, 0);
    }
    regfree(&regex);
  }
  return *line_has_match;
}

bool print_file(const char *path_file, Flags *flags) {
  bool error = false;
  char *line = NULL;
  FILE *file = open_file(path_file, flags, &error);
  if (!error) {
    size_t len = 0;
    ssize_t read;
    int line_number = 0;
    int match_count = 0;
    bool file_already_printed = false;
    while ((read = getline(&line, &len, file)) != -1 &&
           (!flags->grep.l || !file_already_printed)) {
      line_number++;
      if (read > 0 && line[read - 1] == '\n') {
        line[read - 1] = '\0';
        read--;
      }
      bool line_already_printed = false;
      bool line_has_match = false;
      for (int i = 0; i < flags->grep.count_pattern && !line_already_printed;
           i++) {
        const char *pattern = flags->grep.pattern[i];
        if (pattern != NULL) {
          line_has_match = handle_single_pattern(
              line, path_file, flags, line_number, pattern, &line_has_match,
              &line_already_printed);
        }
      }
      if (flags->grep.v && !line_has_match) {
        match_count++;
        if (!flags->grep.c && !flags->grep.l && !line_already_printed) {
          print_file_info(path_file, flags, line_number);
          printf("%s\n", line);
        }
      } else if (!flags->grep.v && line_has_match) {
        match_count++;
      }
      if (flags->grep.l && match_count > 0 && !file_already_printed) {
        printf("%s\n", path_file);
        file_already_printed = true;
      }
    }
    if (flags->grep.c && !file_already_printed) {
      print_file_info(path_file, flags, line_number);
      printf("%d\n", match_count);
    }
    fclose(file);
  }
  free(line);
  return error;
}

void free_flags(Flags *flags) {
  if (flags->grep.pattern != NULL) {
    for (int i = 0; i < flags->grep.count_pattern; i++) {
      free(flags->grep.pattern[i]);
    }
    free(flags->grep.pattern);
    flags->grep.pattern = NULL;
    flags->grep.count_pattern = 0;
  }
  if (flags->grep.f != NULL) {
    free(flags->grep.f);
    flags->grep.f = NULL;
  }
}