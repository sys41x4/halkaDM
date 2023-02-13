#ifndef HALKADM_SECURITY_H
#define HALKADM_SECURITY_H
#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>


class HALKADM_SECURITY{
public:
    void mask_authInput(int authType, int *maskingConfig, int visibleStrOutMaxLen, const char* str, char* visibleStrOut);
};

#endif
