#ifndef HALKADM_DATA_HANDLING_H
#define HALKADM_DATA_HANDLING_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

class DATA{
public:
    int getCharFreq(char character, char* arr);
    int maxFlatItemLen(char character, char* arr);

    char* replaceStr(char* arrTo, char* arrFrom, const char* startBound, const char* endBound, const char* replacementOf, const char* replaceBy);
    char* replaceStr(char* arrTo, char* arrFrom, char startBound[2], char endBound[2], const char* replacementOf, const char* replaceBy);

    int getItemID(char seperator, char* arr, char* arrTocmp);
    char* getItemName(char seperator, char* arr, int itemID, char* item);

    char* flatKeyValue(char* arrTo, char seperator, const char* keyArr, const char* valueArr);
    char* getFlatKey(char* arrTo, char seperator, const char* flatKeyValueArr);
    char* getFlatValue(char* arrTo, char seperator, const char* flatKeyValueArr);

    char* cpArray(char* arrTo, const char* arrFrom);

    void freeArray(int r, char** arr);
    void freeArray(int r, int c, char*** arr);
    char* fillArray(int c, char* arr);
    char** fillArray(int r, int c, char** arr);
    char*** fillArray(int r, int rc, int c, char*** arr);
};

DATA data_handler;

#endif
