#include "cat_function.h"

int main(const int argc, char *argv[]) {
  bool error = EXIT_SUCCESS;
  opterr = 0;
  Flags flags;
  init_flags(&flags);
  error = process_files(argc, argv, &flags);
  for (int i = 0; i < argc; i++) {
    if (print_file(argv[i], &flags) != 0) {
      error = true;
    }
  }
  return error;
}