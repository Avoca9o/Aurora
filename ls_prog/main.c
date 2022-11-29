#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void ls(DIR* cd);

int main() {
  char* cwd = getcwd(cwd, 1024);
  DIR* cd = opendir(cwd);
  ls(cd);
  free(cwd);
  closedir(cd);
}
