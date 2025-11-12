#include "grep_function.h"

#include <stdlib.h>

bool add_pattern(Flags *flags, const char *pattern) {
  bool error = false;
  char **temp = realloc(flags->pattern,
                        (flags->count_pattern + 1) * sizeof(char *));
  if (temp == NULL) {
    printf("Failed to allocate memory for pattern array");
    error = true;
  } else {
    flags->pattern = temp;
    char *new_pattern = strdup(pattern);
    if (new_pattern == NULL) {
      printf("Failed to allocate memory for new pattern");
      free(flags->pattern);
      flags->pattern = NULL;
      flags->count_pattern = 0;
      error = true;
    } else {
      flags->pattern[flags->count_pattern] = new_pattern;
      flags->count_pattern++;
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
  if (!flags->count_pattern) {
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

bool parse_arguments(int argc, char *argv[], Flags *flags) {
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
        flags->i = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'f':
        if (pattern_for_file(optarg, flags)) {
          opt = -1;
        }
        break;
      case 'o':
        flags->o = true;
        break;
      default:
        flags->error = true;
        handle_flag_error(flags, optopt);
        opt = -1;
        break;
    }
  }
  handle_empty_patterns(argc, argv, flags, &optind);
  collect_files(argc, argv);
  if (*count_files > 1) {
    flags->multi_files = true;
  }
  return error;
}

void print_file_info(const char *path_file, const Flags *flags,
                     int line_number) {
  if (flags->multi_files && !flags->h) {
    printf("%s:", path_file);
  }
  if (flags->n && !flags->c) {
    printf("%d:", line_number);
  }
}

bool handle_single_pattern(const char *line, const char *path_file,
                           const Flags *flags, int line_number,
                           const char *pattern, bool *line_has_match,
                           bool *line_already_printed) {
  regex_t regex;
  int reg_flags = REG_EXTENDED;
  if (flags->i) {
    reg_flags = reg_flags | REG_ICASE;
  }
  if (regcomp(&regex, pattern, reg_flags) == 0) {
    regmatch_t match;
    const char *ptr = line;
    int match_result = regexec(&regex, ptr, 1, &match, 0);
    while (!match_result && !(*line_already_printed)) {
      *line_has_match = true;
      if (!flags->v && !flags->c && !flags->l) {
        print_file_info(path_file, flags, line_number);
        if (flags->o) {
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
           (!flags->l || !file_already_printed)) {
      line_number++;
      if (read > 0 && line[read - 1] == '\n') {
        line[read - 1] = '\0';
        read--;
      }
      bool line_already_printed = false;
      bool line_has_match = false;
      for (int i = 0; i < flags->count_pattern && !line_already_printed;
           i++) {
        const char *pattern = flags->pattern[i];
        if (pattern != NULL) {
          line_has_match = handle_single_pattern(
              line, path_file, flags, line_number, pattern, &line_has_match,
              &line_already_printed);
        }
      }
      if (flags->v && !line_has_match) {
        match_count++;
        if (!flags->c && !flags->l && !line_already_printed) {
          print_file_info(path_file, flags, line_number);
          printf("%s\n", line);
        }
      } else if (!flags->v && line_has_match) {
        match_count++;
      }
      if (flags->l && match_count > 0 && !file_already_printed) {
        printf("%s\n", path_file);
        file_already_printed = true;
      }
    }
    if (flags->c && !file_already_printed) {
      print_file_info(path_file, flags, line_number);
      printf("%d\n", match_count);
    }
    fclose(file);
  }
  free(line);
  return error;
}

void free_flags(Flags *flags) {
  if (flags->pattern != NULL) {
    for (int i = 0; i < flags->count_pattern; i++) {
      free(flags->pattern[i]);
    }
    free(flags->pattern);
    flags->pattern = NULL;
    flags->count_pattern = 0;
  }
  if (flags->f != NULL) {
    free(flags->f);
    flags->f = NULL;
  }
}