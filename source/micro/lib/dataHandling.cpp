#include "dataHandling.h"

char* DATA::fillArray(int c, char* arr){

    arr = static_cast<char*>(std::malloc(c * sizeof(char)));
    arr[0] = '\0';
    return arr;
}

char** DATA::fillArray(int r, int c, char** arr){

    arr = static_cast<char**>(std::malloc(r * sizeof(char*)));
    for (int i = 0; i < c; i++) {
        arr[i] = static_cast<char*>(std::malloc(c * sizeof(char)));
        arr[i][0] = '\0';
    }
    return arr;
}

char*** DATA::fillArray(int r, int rc, int c, char*** arr){

    arr = static_cast<char***>(std::malloc(r * sizeof(char**)));
    for (int i = 0; i < rc; i++) {
        arr[i] = static_cast<char**>(std::malloc(rc * sizeof(char*)));
        for (int j = 0; j < c; j++) {
            arr[i][j] = static_cast<char*>(std::malloc(c * sizeof(char)));
            arr[i][j][0] = '\0';
        }
    }
    return arr;
}

void DATA::freeArray(int r, char** arr){
    for (int i = 0; i < r; i++) {
        free(arr[i]);
    }
    free(arr);
}

void DATA::freeArray(int r, int c, char*** arr){
    for (int i = 0; i < r; i++) {
        //for(int j=0; j<c; j++){
          //  free(arr[i][j]);
        //}
        free(arr[i]);
    }
    free(arr);
}
