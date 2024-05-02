
#include "dirwalk.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>


void view_list_from_dir(const char *dirname, enum FileTypes types) {

  DIR *dir = opendir(dirname);
  if (dir == NULL) {
    perror(" directory does not exist");
  }
  printf("read file from %s\n", dirname);

  char *filenames[512];
  int num_files = 0;
  struct dirent *entity;
  entity = readdir(dir);

  while (entity != NULL) {
    printf("%hhd %s %s\n", entity->d_type, dirname, entity->d_name);
    filenames[num_files] = entity->d_name;
    num_files++;
    if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 &&
        strcmp(entity->d_name, "..") != 0 && types == ONLY_DIR) {

      char path[256] = {0};

      strcat(path, dirname);
      strcat(path, "/");
      strcat(path, entity->d_name);

      view_list_from_dir(path, types);

    } else if (entity->d_type == DT_LNK && types == ONLY_LINK) {
      printf("%hhd %s %s\n", entity->d_type, dirname, entity->d_name);
    } else if (entity->d_type == DT_REG && types == ONLY_FILE) {
      printf("%hhd %s %s\n", entity->d_type, dirname, entity->d_name);
    }
    entity = readdir(dir);
  }
  if (types == SORTED) {
    qsort(filenames, num_files, sizeof(char *), compare);
    for (int i = 0; i < num_files; i++) {
      printf("%hhd %s %s\n", DT_REG, dirname, filenames[i]);
    }
  }
  closedir(dir);
}

int compare(const void *first, const void *second) {
  return strcoll(first, second);
}