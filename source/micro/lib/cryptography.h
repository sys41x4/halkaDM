#ifndef HALKADM_CRYPTO_H
#define HALKADM_CRYPTO_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

class HALKADM_CRYPTO{
public:
    // void init();
    // ~HALKADM_CRYPTO(void); // destructor
    void maskStr(int strLen, char character, char *arr);
    void generateRandomStr(int randomizeLen, int maxStrLen, const char *chrType, char *visibleStrOut);
//    void getMD5hash(const char* input, char* hashOut);
    char* getMD5hash(const char* input, char* hashOut);
};

//HALKADM_CRYPTO halkadm_crypto;

#endif
