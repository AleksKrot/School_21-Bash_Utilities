#include "my_grep_function.h"

int main(int argc, char *argv[]) {
  bool error = EXIT_SUCCESS;
  opterr = 0;
  Flags flags;
  init_flags(&flags);
  error = process_files(argc, argv, &flags);
  for (int i = optind; i < argc; i++) {
    error = print_file(argv[i], &flags);
  }
  free_flags(&flags);
  return error;
}