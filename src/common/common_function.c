#include "common_function.h"

void init_flags(Flags *flags) {
#ifdef CAT_FUNCTION_H
  flags->b = false;
  flags->e = false;
  flags->E = false;
  flags->t = false;
  flags->T = false;
  flags->num = 1;
  flags->last_c_file = EOF;
  flags->program_name = "cat";
#endif
#ifdef GREP_FUNCTION_H
  flags->pattern = NULL;
  flags->count_pattern = 0;
  flags->i = false;
  flags->c = false;
  flags->l = false;
  flags->h = false;
  flags->f = NULL;
  flags->o = false;
  flags->multi_files = false;
  flags->program_name = "grep";
#endif
  flags->n = false;
  flags->s = false;
  flags->v = false;
  flags->error = false;
}

void print_error() {
#ifdef CAT_FUNCTION_H
  printf("cat: No files specified\n");
#endif
#ifdef GREP_FUNCTION_H
  printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
  printf("Try 'grep --help' for more information\n");
#endif
}

void handle_flag_error(const Flags *flags, char invalid_opt) {
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

void collect_files(int argc, char *argv[]) {
  for (int i = optind; i < argc; i++) {
    path_file[(*count_files)++] = argv[i];
  }
}

FILE *open_file(const char *path_file, const Flags *flags, bool *error) {
  FILE *file = NULL;
  file = fopen(path_file, "r");
  if (file == NULL) {
    if (flags->s == false) {
      printf("%s: %s: No such file or directory\n", flags->program_name,
             path_file);
    }
    *error = true;
  }
  return file;
}