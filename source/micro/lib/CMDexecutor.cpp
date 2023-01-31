#include "CMDexecutor.h"

void CMD_EXECUTOR::exec(const char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        // char buffer[BUFSIZ];
        // while (fgets(buffer, sizeof(buffer), pp) != 0) {}
        pclose(pp);
    }
}

void CMD_EXECUTOR::exec(char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        // char buffer[BUFSIZ];
        // while (fgets(buffer, sizeof(buffer), pp) != 0) {}
        pclose(pp);
    }
}

char* CMD_EXECUTOR::fetchExecOutput(char* arr, const char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount+2]!='\0'){elementCount++;}
//              elementCount = strlen(buffer);
//            arr = fillArray(elementCount, arr);
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';arr[elementCount] = '\0';
//            for(int i=0; i<elementCount; i++){arr[i]=buffer[i];} // Calculate the length of the char array
            for(int i=0; buffer[i]=='\0'; i++){arr[i]=buffer[i];}
            //arr[elementCount] = '\0';
            // waddstr(mainScreenWin, buffer);
            // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(arr)+2), 1, 0, 0, 13, arr);

        }
  //      else{arr=nullptr;}
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }
}


char* CMD_EXECUTOR::fetchExecOutput(char* arr, char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount]!='\0'){elementCount++;}
//            arr = fillArray(elementCount, arr);
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';arr[elementCount]='\0';
            for(int i=0; i<elementCount; i++){arr[i]=buffer[i];} // Calculate the length of the char array
            // waddstr(mainScreenWin, buffer);
            // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(arr)+2), 1, 0, 0, 13, arr);

        }
  //      else{arr=nullptr;}
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }
}

// CMD_EXECUTOR cmd_executor;
