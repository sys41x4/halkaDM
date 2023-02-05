#include "CMDexecutor.h"

void CMD_EXECUTOR::exec(const char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        pclose(pp);
    }
}

void CMD_EXECUTOR::exec(char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        pclose(pp);
    }
}

char* CMD_EXECUTOR::fetchExecOutput(char* &arr, const char* cmd){

    if(arr!=nullptr){
        free(arr);arr=nullptr;
    }

    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount+2]!='\0'){elementCount++;}
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';arr[elementCount] = '\0';
            for(int i=0; buffer[i]=='\0'; i++){arr[i]=buffer[i];}

        }
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }
    return arr;
}


char* CMD_EXECUTOR::fetchExecOutput(char* arr, char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount]!='\0'){elementCount++;}
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';arr[elementCount]='\0';
            for(int i=0; i<elementCount; i++){arr[i]=buffer[i];} // Calculate the length of the char array

        }
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }
}


char* CMD_EXECUTOR::fetchExecOutput(const char* cmd){
    char* arr=nullptr;

    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount+2]!='\0'){elementCount++;}
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';arr[elementCount] = '\0';
            for(int i=0; buffer[i]=='\0'; i++){arr[i]=buffer[i];}

        }
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }
    return arr;
}
