//
// Created by wings on 18-4-21.
//
#ifndef MYSHCOMMAND_MYSHLIB_H
#define MYSHCOMMAND_MYSHLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#define MAX_DIR_DEPTH (256)

char *dir_stack[MAX_DIR_DEPTH];


char *find_name_byino(ino_t ino);
ino_t get_ino_byname(char *filename);
char* my_getcwd(char * buf, size_t size);




#endif //MYSHCOMMAND_MYSHLIB_H
