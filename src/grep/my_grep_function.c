#include "my_grep_function.h"

bool process_files(int argc, char *argv[], Flags *flags) {
  bool error = EXIT_SUCCESS;
  if (argc < 2) {
      printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
      printf("Try 'grep --help' for more information.\n");
      error = EXIT_FAILURE;
  } else {
    error = parse_arguments(argc, argv, flags);
  }
  return error;
}

bool parse_arguments(int argc, char *argv[], Flags *flags) {
  bool error = EXIT_SUCCESS;
  int opt = 0;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        if ((error = add_pattern(flags, optarg)) == EXIT_FAILURE) {
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
        if ((error = pattern_for_file(optarg, flags)) == EXIT_FAILURE) {
          opt = -1;
        }
        break;
      case 'o':
        flags->o = true;
        break;
      default:
        error = EXIT_FAILURE;
        grep_flag_error(optopt);
        opt = -1;
        break;
    }
  }
  error = handle_empty_patterns(argc, argv, flags, &optind);
  if (argc - optind > 1) {
    flags->multi_files = true;
  }
  return error;
}

bool add_pattern(Flags *flags, const char *pattern) {
  bool error = EXIT_SUCCESS;
  char **temp = realloc(flags->pattern,
                        (flags->count_pattern + 1) * sizeof(char *));
  if (temp == NULL) {
    printf("Failed to allocate memory for pattern array");
    error = EXIT_FAILURE;
  } else {
    flags->pattern = temp;
    char *new_pattern = strdup(pattern);
    if (new_pattern == NULL) {
      printf("Failed to allocate memory for new pattern");
      free(flags->pattern);
      flags->pattern = NULL;
      flags->count_pattern = 0;
      error = EXIT_FAILURE;
    } else {
      flags->pattern[flags->count_pattern++] = new_pattern;
    }
  }
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

bool pattern_for_file(const char *path_file, Flags *flags) {
  bool error = EXIT_SUCCESS;
  const char *program_name = "grep";
  FILE *file = open_file(path_file, flags, program_name, &error);
  if (error == EXIT_SUCCESS) {
    size_t len = 0;
    ssize_t read;
    char *line = NULL;
    while ((read = getline(&line, &len, file)) != -1 && !error) {
      if (read > 0 && line[read - 1] == '\n') {
        line[read - 1] = '\0';
        read--;
      }
      if (read > 0) {
        error = add_pattern(flags, line);
      }
      if (line != NULL) {
        free(line);
      }
    }
    fclose(file);
  }
  return error;
}

void grep_flag_error(char invalid_opt) {
  if (invalid_opt == 'f') {
    printf("grep: option requires an argument -- 'f'\n");
  } else {
    printf("grep: invalid option -- '%c'\n", invalid_opt);
  }
  printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
  printf("Try 'grep --help' for more information.\n");
}

bool handle_empty_patterns(int argc, char *argv[], Flags *flags, int *optind) {
  bool error = EXIT_SUCCESS;
  if (flags->count_pattern == 0) {
    if (*optind < argc) {
      error = add_pattern(flags, argv[(*optind)++]);
    }
    if (error == EXIT_FAILURE) {
      printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
      printf("Try 'grep --help' for more information.\n");
    }
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

bool print_file(const char *argv, Flags *flags) {
  bool error = EXIT_SUCCESS;
  const char *program_name = "grep";
  FILE *file = open_file(argv, flags, program_name, &error);
  if (error == EXIT_SUCCESS) {
    size_t len = 0;
    ssize_t read;
    int line_number = 0;
    int match_count = 0;
    bool file_already_printed = false;
    char *line = NULL;
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
              line, argv, flags, line_number, pattern, &line_has_match,
              &line_already_printed);
        }
      }
      if (flags->v && !line_has_match) {
        match_count++;
        if (!flags->c && !flags->l && !line_already_printed) {
          print_file_info(argv, flags, line_number);
          printf("%s\n", line);
        }
      } else if (!flags->v && line_has_match) {
        match_count++;
      }
      if (flags->l && match_count > 0 && !file_already_printed) {
        printf("%s\n", argv);
        file_already_printed = true;
      }
    }
    free(line);
    if (flags->c && !file_already_printed) {
      print_file_info(argv, flags, line_number);
      printf("%d\n", match_count);
    }
    fclose(file);
  }
  return error;
}