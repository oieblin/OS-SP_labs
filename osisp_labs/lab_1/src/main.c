#include "dirwalk.h"
#include <getopt.h>
#include <locale.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  setlocale(LC_COLLATE, "en_EN.UTF-8");
  char *path_to_dir = NULL;
  int path_index = 0;

  if (argc < 2 || argv[1][0] == '-') {
    path_to_dir = DIR_PATH;
  } else {
    path_to_dir = argv[1];
    printf("%s", argv[1]);
  }

  printf("Path to directory: %s\n", path_to_dir);

  int opt;
  enum FileTypes types;
  while ((opt = getopt(argc, argv, "ldfs")) != -1) {
    switch (opt) {
    case 'l':
      view_list_from_dir(path_to_dir, ONLY_LINK);
      break;
    case 'd':
      view_list_from_dir(path_to_dir, ONLY_DIR);
      break;
    case 'f':
      view_list_from_dir(path_to_dir, ONLY_FILE);
      break;
    case 's':
      view_list_from_dir(path_to_dir, SORTED);
      break;
    default:
      fprintf(stderr, "Usage: %s [dir][-l] [-d] [-f] [-s]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}