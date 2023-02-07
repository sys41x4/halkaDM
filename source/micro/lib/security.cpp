#include "security.h"
#include "cryptography.h"

void HALKADM_SECURITY::mask_authInput(int authType, int *maskingConfig, int visibleStrOutMaxLen, const char* str, char* &visibleStrOut){
    /* maskField: "username" | For masking the username for visibility
       maskField: "userpass" | For masking the userpass for visibility*/
    // int maskingConfigLength, strLen;

    // strLen = sizeof(str)/sizeof(str[0]);
    int strLen = strlen(str);

    // Mask According to masking Config

    if(maskingConfig[0]>=2){visibleStrOut[0]='\0';}
    else if(maskingConfig[2]==0){

        if(maskingConfig[1]==0){
            if(maskingConfig[0]==0){
                // strcpy(maskedOutput, str);
                // maskedOutput = str;
                //strcpy(visibleStrOut, str);
                if(strLen>visibleStrOutMaxLen){
                    int startIndex = strLen-visibleStrOutMaxLen;
                     for(int i=startIndex; i<strLen; i++){visibleStrOut[i-startIndex]=str[i];}
                     visibleStrOut[strLen]='\0';
                    //visibleStrOut = strdup(str);
                }
                else{strcpy(visibleStrOut, str);}
            }
            else if(maskingConfig[0]==1){
                // maskedOutput = maskStr(strLen, '*');
                halkadm_crypto.maskStr(strLen, '*', visibleStrOut);
            }
        }
        else if(maskingConfig[1]==1){
            if(maskingConfig[0]==0){
                // maskedOutput = generateRandomStr(0, strLen, "Aa0");
                halkadm_crypto.generateRandomStr(0, strLen, "Aa0", visibleStrOut);
            }
            else if(maskingConfig[0]==1){
                // maskedOutput = generateRandomStr(0, strLen, "!");
                halkadm_crypto.generateRandomStr(0, strLen, "!", visibleStrOut);
            }
        }
    }
    else if(maskingConfig[2]==1){

        if(maskingConfig[1]==0){
            if(maskingConfig[0]==0){
                // maskedOutput = generateRandomStr(1, strLen, "Aa0");
                halkadm_crypto.generateRandomStr(1, strLen, "Aa0", visibleStrOut);
            }
            else if(maskingConfig[0]==1){
                // maskedOutput = maskStr(strLen, '*'); // need review
                halkadm_crypto.maskStr(strLen, '*', visibleStrOut);
            }
        }
        else if(maskingConfig[1]==1){
            if(maskingConfig[0]==0){
                // maskedOutput = generateRandomStr(1, strLen, "Aa0");
                halkadm_crypto.generateRandomStr(1, strLen, "Aa0", visibleStrOut);
            }
            else if(maskingConfig[0]==1){
                // maskedOutput = generateRandomStr(1, strLen, "!");
                halkadm_crypto.generateRandomStr(1, strLen, "!", visibleStrOut);
            }
        }
    }
    else{
         halkadm_crypto.maskStr(strLen, '*', visibleStrOut);
    }

//    return maskedOutput;
}
