#include "cat_function.h"

int main(const int argc, char *argv[]) {
  opterr = 0;
  Flags flags;
  bool error = process_files(argc, argv, &flags);
  return error;
}