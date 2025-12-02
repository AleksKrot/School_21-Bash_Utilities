#include "common_function.h"

void init_flags(Flags *flags) {
  flags->n = false;
  flags->s = false;
  flags->v = false;
  flags->b = false;
  flags->e = false;
  flags->E = false;
  flags->t = false;
  flags->T = false;
  flags->num = 1;
  flags->end_file_symbol = EOF;
  flags->pattern = NULL;
  flags->count_pattern = 0;
  flags->i = false;
  flags->c = false;
  flags->l = false;
  flags->h = false;
  flags->f = false;
  flags->o = false;
  flags->multi_files = false;
}

FILE *open_file(const char *argv, const Flags *flags, const char *program_name,
                bool *error) {
  FILE *file = NULL;
  file = fopen(argv, "r");
  if (file == NULL) {
    if (flags->s == false) {
      printf("%s: %s: No such file or directory\n", program_name, argv);
    }
    *error = EXIT_FAILURE;
  }
  return file;
}