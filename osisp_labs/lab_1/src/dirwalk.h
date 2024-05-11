#ifndef DIRWALK_H
#define DIRWALK_H
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIR_PATH "."
enum FileTypes {
  ONLY_FILE = 'f',
  ONLY_LINK = 'l',
  ONLY_DIR = 'd',
  SORTED = 's',
  ALL_TYPES = '-'

};
int compare(const void *first, const void *second);
void view_list_from_dir(const char *path, enum FileTypes types);
#endif