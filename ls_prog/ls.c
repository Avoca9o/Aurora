#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

void ls(DIR* cd) {
  struct dirent* file;
  while ((file = readdir(cd)) != NULL) {
    printf("%s\n", file->d_name);
  }
  free(file);
}
