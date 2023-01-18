#ifndef HALKADM_DATA_HANDLING_H
#define HALKADM_DATA_HANDLING_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

class DATA{
public:
    void freeArray(int r, char** arr);
    void freeArray(int r, int c, char*** arr);
    char* fillArray(int c, char* arr);
    char** fillArray(int r, int c, char** arr);
    char*** fillArray(int r, int rc, int c, char*** arr);
};

#endif
