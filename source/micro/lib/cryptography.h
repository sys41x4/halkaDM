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

};

void HALKADM_CRYPTO::generateRandomStr(int randomizeLen, int maxStrLen, const char *chrType, char *visibleStrOut){

    int i;
    int strLen=maxStrLen;

    if(randomizeLen!=0){
        srand(time(NULL));
        strLen = rand()%maxStrLen;
    }


    //////////////////////////
    if(strLen>49){strLen=49;}
    char charT;

    // int chrTypeLen = sizeof(chrType)/sizeof(chrType[0]);
    int chrTypeLen = strlen(chrType);

    srand(time(NULL));
    for(i=0; i<strLen; i++){

        // srand(time(NULL));
        charT = chrType[rand()%chrTypeLen];


        if(((int)charT>='a') && ((int)charT<='z')){
        // Lower Case Randomization
            visibleStrOut[i] = 'a'+rand()%26;
        }
        else if(((int)charT>='A') && ((int)charT<='Z')){
        // Upper Case Randomization
            visibleStrOut[i] = 'A'+rand()%26;
        }
        else if(((int)charT>='0') && ((int)charT<='9')){
        // Integer Randomization
            visibleStrOut[i] = '0'+rand()%9;
        }
        else if(
                (((int)charT>='!') && ((int)charT<='/')) ||
                (((int)charT>=':') && ((int)charT<='@')) ||
                (((int)charT>='[') && ((int)charT<='`')) ||
                (((int)charT>='{') && ((int)charT<='~'))
                // (((int)charT>='{') && ((int)charT<=254))
        ){
        // Special Charaacter+symbol Randomization
            int specialCharType=rand()%3;

            if(specialCharType==0){
                visibleStrOut[i] = '!'+rand()%14;
            }
            else if(specialCharType==1){
                visibleStrOut[i] = ':'+rand()%6;
            }
            else if(specialCharType==2){
                visibleStrOut[i] = '['+rand()%5;
            }
            else if(specialCharType==3){
                visibleStrOut[i] = '{'+rand()%3;
            }
        }
        else{
            visibleStrOut[i] = '\0';
        }
    }
    // str[strLen]='\0';
    visibleStrOut[i++]='\0';

//    return str;
}

void HALKADM_CRYPTO::maskStr(int strLen, char character, char *arr){
    //static char str[50];
    // memset(arr, character, sizeof(arr[0])*strLen);
    int i;
    for(i=0; i<strLen; i++){arr[i]=character;}
    arr[i++]='\0';
}

HALKADM_CRYPTO halkadm_crypto;

#endif
