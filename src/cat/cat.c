#include "../common/common_function.h"
#include "cat_function.h"

int main(int argc, char *argv[]) {
  opterr = 0;
  Flags flags;
  bool error = process_files(argc, argv, &flags);
  return error;
}