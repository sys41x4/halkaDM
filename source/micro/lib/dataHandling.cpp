#include "dataHandling.h"
#include <string>

/*char* DATA::replaceStr(char* arrTo, char* arrFrom, const char* startBound, const char* endBound, const char* replacementOf, const char* replaceBy){

    // StartBound and endBound Should Contain char array of size 2
    int replacementOfLen = 0;
    int replaceByLen = 0;
    int fullArrayLen=0;
    int replacementOccurence=0;

    while(replacementOf[replacementOfLen]!='\0'){replacementOfLen++;}
    while(replaceBy[replaceByLen]!='\0'){replaceByLen++;}

    while(arrFrom[fullArrayLen]!='\0'){
        if(arrFrom[fullArrayLen+replacementOfLen+1]!='\0' &&
            (arrFrom[fullArrayLen]==startBound[0]) && (arrFrom[fullArrayLen+1]==startBound[1]) &&
            (arrFrom[fullArrayLen+replacementOfLen+2]==endBound[0]) && (arrFrom[fullArrayLen+replacementOfLen+1+2]==endBound[1])
        ){replacementOccurence++;};
        fullArrayLen++;
    }
    int newArrayLen = (replacementOccurence*replaceByLen)+fullArrayLen-(replacementOccurence*(replacementOfLen+4));
    arrTo = static_cast<char*>(std::malloc(newArrayLen+1 * sizeof(char)));
    arrTo[newArrayLen] = '\0';
    int j=0;
    int replaceCompleteCount = 0;

    for(int i=0; i<fullArrayLen; i++){

        if( (replaceCompleteCount<=replacementOccurence) && (arrFrom[i+replacementOfLen+1]!='\0') &&
            (arrFrom[i]==startBound[0]) && (arrFrom[i+1]==startBound[1]) &&
            (arrFrom[i+replacementOfLen+2]==endBound[0]) && (arrFrom[i+replacementOfLen+1+2]==endBound[1])
        ){

             for(int k=0; k<replaceByLen; k++){arrTo[j+k] = replaceBy[k];}
             i+=replacementOfLen+3;
             j+=replaceByLen-1;
             replaceCompleteCount++;

         }
         else{arrTo[j]=arrFrom[i];}
         j++;
    }
    arrTo[newArrayLen]='\0';

    return arrTo;
}*/

char* DATA::replaceStr(const char* arrFrom, const char* replacementOf, const char* replaceBy) {
  // In this case if start and end bound is presend in arrFrom, then replacementOf should be bounded by them

  if (arrFrom == nullptr || replacementOf == nullptr || replaceBy == nullptr) {
    return nullptr;
  }

  std::string str(arrFrom);
  std::string searchStr(replacementOf);
  std::string replaceStr(replaceBy);

  std::size_t found = str.find(searchStr);
  if (found == std::string::npos) {
    return nullptr;
  }
  while (found != std::string::npos) {
    str.replace(found, searchStr.length(), replaceStr);
    found = str.find(searchStr, found + replaceStr.length());
  }
  char* data = static_cast<char*>(std::malloc(str.length()+1 * sizeof(char)));
  //char* data = new char[str.length() + 1];
  if (data == nullptr) {
    return nullptr;
  }
  strcpy(data, str.c_str());
  //data = strdup(str.c_str());
  return data;
}


char* DATA::replaceStr(char* arrTo, const char* arrFrom, const char* startBound, const char* endBound, const char* replacementOf, const char* replaceBy){
//char* DATA::replaceStr(char* arrTo, char* arrFrom, char startBound[2], char endBound[2], const char* replacementOf, const char* replaceBy){

    if(arrTo!=nullptr){std::free(arrTo);arrTo=nullptr;}
    if(arrFrom==nullptr){return nullptr;}

    // StartBound and endBound Should Contain char array of size 2
    int replacementOfLen = 0;
    int replaceByLen = 0;
    int fullArrayLen=0;
    int replacementOccurence=0;

    while(replacementOf[replacementOfLen]!='\0'){replacementOfLen++;}
    while(replaceBy[replaceByLen]!='\0'){replaceByLen++;}

/*
    arrTo = strdup(arrFrom);
   // NEW
    std::string str(arrTo);
    std::size_t found = str.find(replacementOf);
    while (found != std::string::npos) {
        str.replace(found, strlen(replacementOf), replaceBy);
        found = str.find("example", found + strlen(replaceBy));
    }

*/

    while(arrFrom[fullArrayLen]!='\0'){
        if(arrFrom[fullArrayLen+replacementOfLen+1]!='\0' &&
            (arrFrom[fullArrayLen]==startBound[0]) && (arrFrom[fullArrayLen+1]==startBound[1]) &&
            (arrFrom[fullArrayLen+replacementOfLen+2]==endBound[0]) && (arrFrom[fullArrayLen+replacementOfLen+1+2]==endBound[1])
        ){replacementOccurence++;};

        fullArrayLen++;
    }
    int newArrayLen = (replacementOccurence*replaceByLen)+fullArrayLen-(replacementOccurence*(replacementOfLen+4));
    arrTo = static_cast<char*>(std::malloc(newArrayLen+1 * sizeof(char)));
    arrTo[newArrayLen]='\0';
    int j=0;
    int replaceCompleteCount = 0;

    for(int i=0; i<fullArrayLen; i++){

        if( (replaceCompleteCount<=replacementOccurence) && (arrFrom[i+replacementOfLen+1]!='\0') &&
            (arrFrom[i]==startBound[0]) && (arrFrom[i+1]==startBound[1]) &&
            (arrFrom[i+replacementOfLen+2]==endBound[0]) && (arrFrom[i+replacementOfLen+1+2]==endBound[1])
        ){

             for(int k=0; k<replaceByLen; k++){arrTo[j+k] = replaceBy[k];}
             i+=replacementOfLen+3;
             j+=replaceByLen-1;
             replaceCompleteCount++;

         }
         else{arrTo[j]=arrFrom[i];}
         j++;
    }
    arrTo[newArrayLen]='\0';

    return arrTo;
}

char* DATA::replaceStr(const char* arrFrom, const char* startBound, const char* endBound, const char* replacementOf, const char* replaceBy){
    char* arr=nullptr;
    arr = replaceStr(arr, arrFrom, startBound, endBound, replacementOf, replaceBy);
    return arr;
}

/*char* DATA::replaceStr(char* arrTo, const char* arrFrom, const char* startBound, const char* endBound, const char* replacementOf, const char* replaceBy){
    arrTo = replaceStr(arrTo, arrFrom, startBound, endBound, replacementOf, replaceBy);
    return arrTo;
}*/

/*int DATA::getItemID(char seperator, const char* arr, const char* arrTocmp){
    if(arr==nullptr || arrTocmp==nullptr){return -1;}

    int arrTocmpLen=strlen(arrTocmp);
    int id=0, j=0;
    for(int i=0;arr[i]!='\0';i++){
        if(arr[i]==seperator){id++;j=0;i++;}

        if(arr[i]==arrTocmp[j]){
            j++;
//            if((arr[i+1]==seperator || (arr[i+1]=='\0')) && j==arrTocmpLen){
            if (j == arrTocmpLen && (arr[i + 1] == separator || arr[i + 1] == '\0')) {
                return id;
            }
        }
        else{j=0;}
    }
    return -1;
}
*/

int DATA::getItemID(char seperator, const char* arr, const char* arrTocmp){
    if(arr==nullptr || arrTocmp==nullptr){return -1;}

    char end[3];
    snprintf(end, 3, "%c\xff", seperator);
    char* data = replaceStr(arr, arrTocmp, end);
    if(data==nullptr){std::free(data);return -1;}
    int counter=-1;
    for(int i=0;data[i]!='\0';i++){
//        if(data[i]=='\xff' && (data[i+1]==seperator || data[i+1]=='\0')){break;}
        if(data[i]==seperator){counter++;}
        if(data[i]=='\xff' && (data[i+1]==seperator || data[i+1]=='\0')){std::free(data);return counter;}
    }
    std::free(data);
    return -1;
/*    int arrTocmpLen=strlen(arrTocmp);
    int id=0, j=0;
    for(int i=0;arr[i]!='\0';i++){
        if(arr[i]==seperator){id++;j=0;i++;}

        if(arr[i]==arrTocmp[j]){
            j++;
//            if((arr[i+1]==seperator || (arr[i+1]=='\0')) && j==arrTocmpLen){
            if (j == arrTocmpLen && (arr[i + 1] == separator || arr[i + 1] == '\0')) {
                return id;
            }
        }
        else{j=0;}
    }
    return -1;*/
}

char* DATA::getItemName(char seperator, char* arr, int itemID, char* item){

    if (item != nullptr) {
        //return nullptr;
        std::free(item);item=nullptr;
    }

    if (arr == nullptr) {
        return nullptr;
    }

    int itemLen=0, trackID=itemID;

    for(int i=0;arr[i]!='\0';i++){
        if(trackID==0 && arr[i]!=seperator){itemLen++;}
        if(arr[i]==seperator || arr[i+1]=='\0'){trackID--;}
        else if(trackID<0){trackID=0;break;}
    }

    if(trackID==0){
        //free(item);
        item = static_cast<char*>(std::malloc(itemLen+1 * sizeof(char)));
//        item[itemLen]='\0';
        if (item == nullptr) {
            return nullptr;
        }
        itemLen=0;

        for(int i=0;arr[i]!='\0';i++){
            if(itemID==0 && arr[i]!=seperator){item[itemLen]=arr[i];itemLen++;}
            if(itemID==0 && arr[i]==seperator){item[itemLen]='\0';}
            if(arr[i]==seperator || arr[i+1]=='\0'){itemID--;}
            else if(trackID<0){trackID=0;break;}
        }
        item[itemLen]='\0';
        if(item[0]=='\0'){std::free(item);return nullptr;}
        return item;
    }
    else{std::free(item);return nullptr;}
    return item;
}

int DATA::getCharFreq(char character, char* arr){
    int count=0;
    for(int i=0;arr[i]!='\0';i++){if(arr[i]==character){count++;}}
    return count;
}

int DATA::getCharFreq(char character, const char* arr){
    int count=0;
    for(int i=0;arr[i]!='\0';i++){if(arr[i]==character){count++;}}
    return count;
}

int DATA::maxFlatItemLen(char character, char* arr){
    int cnt=0;
    int maxLen=0;
    for(int i=0;arr[i]!='\0';i++){
        if(arr[i]==character || arr[i+1]=='\0'){if(maxLen<cnt){maxLen=cnt;}cnt=0;}
        else{cnt++;}
    }
    return maxLen;
}

int DATA::maxFlatItemLen(char character, const char* arr){
    int cnt=0;
    int maxLen=0;
    for(int i=0;arr[i]!='\0';i++){
        if(arr[i]==character || arr[i+1]=='\0'){if(maxLen<cnt){maxLen=cnt;}cnt=0;}
        else{cnt++;}
    }
    return maxLen;
}

//char* DATA::flatKeyValue(char* arrTo, char seperator, const char* keyArr, const char* valueArr){
char* DATA::flatKeyValue(char seperator, const char* keyArr, const char* valueArr){
//    int keylen=0, valuelen=0;
//    while(keyArr[keylen]!='\0'){keylen++;}
//    while(valueArr[valuelen]!='\0'){valuelen++;}
    if (keyArr == nullptr && valueArr == nullptr) {
        return nullptr;
    }
    int keylen=strlen(keyArr);
    int valuelen=strlen(valueArr);

    char* arrTo = static_cast<char*>(std::malloc(keylen+valuelen+3 * sizeof(char)));
    if (arrTo == nullptr) {
        return nullptr;
    }
//    snprintf(arrTo, keylen+valuelen+2, "%s%c%s", keyArr, seperator, valueArr);

    for(int i=0; i<=keylen; i++){arrTo[i] = keyArr[i];}
    arrTo[keylen+1]=seperator;
    for(int i=0; i<=valuelen; i++){arrTo[i+keylen+2] = valueArr[i];}
    arrTo[keylen+valuelen+2]='\0';
    return arrTo;
}

//char* DATA::flatKeyValue(char seperator, const char* keyArr, const char* valueArr){
char* DATA::flatKeyValue(char* arrTo, char seperator, const char* keyArr, const char* valueArr){
    if (keyArr == nullptr && valueArr == nullptr) {
        return nullptr;
    }
    if(arrTo!=nullptr){std::free(arrTo);arrTo=nullptr;}

    arrTo = flatKeyValue(seperator, keyArr, valueArr);

    return arrTo;
}



/*void DATA::flatKeyValue(char* &arrTo, char seperator, const char* keyArr, const char* valueArr){
    if(arrTo!=nullptr){
        free(arrTo);arrTo=nullptr;
    }
    int keylen=0, valuelen=0;
    while(keyArr[keylen]!='\0'){keylen++;}
    while(valueArr[valuelen]!='\0'){valuelen++;}

    arrTo = static_cast<char*>(std::malloc(keylen+valuelen+2 * sizeof(char)));

    for(int i=0; i<=keylen; i++){arrTo[i] = keyArr[i];}
    arrTo[keylen+1]=seperator;
    for(int i=0; i<=valuelen; i++){arrTo[i+keylen+2] = valueArr[i];}
}
*/
char* DATA::getFlatValue(char* arrTo, char seperator, const char* flatKeyValueArr){
    if (flatKeyValueArr == nullptr) {
        return nullptr;
    }

    if(arrTo!=nullptr){
        std::free(arrTo);arrTo=nullptr;
    }
    int keylen=0, valuelen=0;
    while(flatKeyValueArr[keylen]!=seperator){keylen++;}
    while(flatKeyValueArr[keylen+valuelen+1]!='\0'){valuelen++;}

    arrTo = static_cast<char*>(std::malloc(valuelen+1 * sizeof(char)));
    if (arrTo == nullptr) {
        return nullptr;
    }
    for(int i=0;i<=valuelen; i++){arrTo[i]=flatKeyValueArr[keylen+1+i];}
    arrTo[valuelen]='\0';
    return arrTo;
}

char* DATA::getFlatKey(char* arrTo, char seperator, const char* flatKeyValueArr){
    if (flatKeyValueArr == nullptr) {
        return nullptr;
    }

    if(arrTo!=nullptr){
        std::free(arrTo);arrTo=nullptr;
    }

    int keylen=0;
    while(flatKeyValueArr[keylen]!=seperator){keylen++;}
    arrTo = static_cast<char*>(std::malloc(keylen+1 * sizeof(char)));
    if (arrTo == nullptr) {
        return nullptr;
    }
//    arrTo[keylen+1]='\0';
    for(int i=0;i<keylen; i++){arrTo[i]=flatKeyValueArr[i];}
    arrTo[keylen]='\0';
    return arrTo;
}

char* DATA::cpArray(char* arrTo, const char* arrFrom){

    if (arrTo != nullptr) {
        std::free(arrTo);arrTo=nullptr;
    }
    if (arrFrom == nullptr) {
        return nullptr;
    }
    int len=0;
    while(arrFrom[len]!='\0'){len++;}
    arrTo = static_cast<char*>(std::malloc(len+1 * sizeof(char)));
    if (arrTo == nullptr) {
        return nullptr;
    }
    for(int i=0; i<len; i++){arrTo[i] = arrFrom[i];}
    arrTo[len]='\0';
    return arrTo;
}

char* DATA::fillArray(int c, char* arr){

    if(arr!=nullptr){std::free(arr);arr=nullptr;}

    arr = static_cast<char*>(std::malloc(c+1 * sizeof(char)));
    if(arr==nullptr){return nullptr;}
    arr[0]='\0';arr[c] = '\0';
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
        std::free(arr[i]);
    }
    std::free(arr);
}

void DATA::freeArray(int r, int c, char*** arr){
    for (int i = 0; i < r; i++) {
        std::free(arr[i]);
    }
    std::free(arr);
}
