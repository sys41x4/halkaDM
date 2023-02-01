//#include <iostream> //for std::cout
#include "cryptography.h"
#include <openssl/md5.h>
//#include <string>
//#include "md5.h"
//using std::cout; using std::endl;
//#include "md5NEW.h"
//#include <iomanip> 
// #include <cmath>

char* HALKADM_CRYPTO::getMD5hash(const char* input, char* hashOut) {
//  unsigned char md5_result[MD5_DIGEST_LENGTH];
  unsigned char *md5_result;
 //free(hashOut);
  char result[MD5_DIGEST_LENGTH * 2 + 1];
//  result = static_cast<char*>(std::malloc(MD5_DIGEST_LENGTH * sizeof(char)));

  md5_result = MD5((const unsigned char*)input, strlen(input), NULL);

  for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(result + (i * 2), "%02x", (unsigned int)md5_result[i]);
  }
  hashOut = strdup(result);//free(result);
  return hashOut;
}
/*void HALKADM_CRYPTO::getMD5hash(const char* input, char* hashOut) {
//  unsigned char md5_result[MD5_DIGEST_LENGTH];
  unsigned char *md5_result;
 //free(hashOut);
  char result[MD5_DIGEST_LENGTH * 2 + 1];
//  result = static_cast<char*>(std::malloc(MD5_DIGEST_LENGTH * sizeof(char)));

  md5_result = MD5((const unsigned char*)input, strlen(input), NULL);

  for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(result + (i * 2), "%02x", (unsigned int)md5_result[i]);
  }
  hashOut = strdup(result);//free(result);

}*/
void HALKADM_CRYPTO::maskStr(int strLen, char character, char *arr){
    //static char str[50];
    // memset(arr, character, sizeof(arr[0])*strLen);
    int i;
    for(i=0; i<strLen; i++){arr[i]=character;}
    arr[i++]='\0';
}

void HALKADM_CRYPTO::generateRandomStr(int randomizeLen, int maxStrLen, const char *chrType, char *visibleStrOut){

    int i;
    int strLen=maxStrLen;

    if(randomizeLen!=0){
        srand(time(NULL));
        strLen = rand()%maxStrLen;
    }
    /*else{
        strLen = maxStrLen;
    }*/


    //////////////////////////
    if(strLen>49){strLen=49;}
    char charT;

    // int chrTypeLen = sizeof(chrType)/sizeof(chrType[0]);
    int chrTypeLen = strlen(chrType);

    srand(time(NULL));
    for(i=0; i<strLen; i++){

        // srand(time(NULL));
        /*if(chrTypeLen>1){charT = chrType[rand()%chrTypeLen];}
        else{charT = chrType[0];}*/
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
};
// HALKADM_CRYPTO halkadm_crypto;
