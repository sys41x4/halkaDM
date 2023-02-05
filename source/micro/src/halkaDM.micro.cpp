#include "../config/config.h"
#include "../config/config.cpp"
#include "../lib/cryptography.h"
#include "../lib/cryptography.cpp"
#include "../lib/security.h"
#include "../lib/security.cpp"
#include "../lib/dataHandling.h"
#include "../lib/dataHandling.cpp"
#include "../lib/CMDexecutor.h"
#include "../lib/CMDexecutor.cpp"
#include "../lib/sessionManagement.h"
#include "../lib/sessionManagement.cpp"
#include "../lib/AuthManagement.h"
#include "../lib/AuthManagement.cpp"
#include "../lib/utils.h"
#include "../lib/utils.cpp"
#include "../lib/cpptoml.h"
#include <locale.h>
#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <security/pam_appl.h>
#include <xcb/xcb.h>
#include <utmp.h>
#include <unistd.h>
#include "../lib/draw.h"
#include "../lib/draw.cpp"
#include "../lib/pam.h"
#include "../lib/pam.c"
#include "../lib/inputs.h"
#include "../lib/inputs.cpp"
#include "../lib/cpptoml.h"
//#include <openssl/evp.h>
#include <openssl/md5.h>

using namespace std;




/*
// TitleBar Item ID List

1. Power
2. Utilities
3. <DateTime>
4. Desktop Manager

*/

int *titleBarItemTree;


WINDOW *mainScreenWin, *subItemListWindow, *loginColourMatrixWin, *authBox, *accountPicBox, *titleBar_subwin; // User Account Picture Box
WINDOW *messageBoxBorderWindow, *messageBox_msg;

int winMaxX, winMaxY;
int accountPicBoxMaxX, accountPicBoxMaxY, accountPicBoxMaxW, accountPicBoxMaxH;
int msgBoxMaxX, msgBoxMaxY, msgBoxMaxW, msgBoxMaxH;
int loginBoxMaxX, loginBoxMaxY;


int *loginColourMatrixConf;



// Import Classes
// HALKADM_CRYPTO halkadm_crypto;
// CMD_EXECUTOR cmd_executor;
// DATA data_handler;
/*HALKADM_SECURITY halkadm_security;
DRAW draw;
SESSION_MANAGEMENT session_management;
AUTH_MANAGEMENT auth_management;
*/
// HALKADM_CRYPTO halkadm_crypto;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_charArr(WINDOW *win, int y, int x, int colorID, const char* arr){
    wattron(win, COLOR_PAIR(colorID));
    mvwprintw(win, y, x, arr);
    wattroff(win, COLOR_PAIR(colorID));
}

void draw_charArr(WINDOW *win, int y, int x, int colorID, char* arr){
    wattron(win, COLOR_PAIR(colorID));
    mvwprintw(win, y, x, arr);
    wattroff(win, COLOR_PAIR(colorID));
}


// Test Codes
// Test Codes End







void setLoginMatrixWindow(WINDOW *win){
    loginColourMatrixWin = win;
}

void loginMatrixSetConfig(int y, int x, int h, int w){
    loginColourMatrixConf[0] = y;
    loginColourMatrixConf[1] = x;
    loginColourMatrixConf[2] = h;
    loginColourMatrixConf[3] = w;
}


void delete_window(WINDOW *win){
    delwin(win);
    free(win);
}

void initColor(){
   // must be called after initializing the frames

   /*

// COLORS MAPOUT //

0: [1-8] | [COLOR_BLACK, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_WHITE, COLOR_MAGENTA, COLOR_BLUE, COLOR_CYAN] : size:8 | Exclude the following for directly calling them in any case
1: [9-16/X] : size:8 | Pre Setup Colours | will be loaded from config file | minSize=8 | maxSize=USER_PREFERENCE
2: [<totalRandomizedColors>] | <randomize <totalRandomizedColors> colors> : size <totalRandomizedColors> | Randomization will be done only once on initial loading of the executable, to reduce memory consumpti>
3: [username visibility colourmap] : size:3 | will get colors from [9-16]
4: | [userpass visibility colourmap] : size:3 | will get colors from [9-16]
5: [Profile Pic] : size:(Relative)maxX*maxY | 1D-array | available colour : [1-8]+[9-16]+<totalRandomizedColors>
6: [Login Randomization colourMap] : size:(Relative)maxX*maxY | 1D-array | available colour : [1-8]+[9-16]+<totalRandomizedColors>


*/

   int fg, bg;
   int storedColorsStartIndex = 0;
   // int totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);
   // int totalManualColors = 8;

   // Default Stored Colour blocks | fg==bg
   for(int i=1; i<=config.totalASCIIcolors; i++){
       init_pair(i, config.asciiColors[i+storedColorsStartIndex-1], config.asciiColors[i+storedColorsStartIndex-1]);
   }

   // Manual Colour Setup
   init_pair(9, COLOR_WHITE, COLOR_BLACK);
   init_pair(10, COLOR_WHITE, COLOR_RED);
   init_pair(11, COLOR_BLACK, COLOR_YELLOW);
   init_pair(12, COLOR_WHITE, COLOR_GREEN);
   init_pair(13, COLOR_BLACK, COLOR_WHITE);
   init_pair(14, COLOR_WHITE, COLOR_MAGENTA);
   init_pair(15, COLOR_WHITE, COLOR_BLUE);
   init_pair(16, COLOR_BLACK, COLOR_CYAN);

   // Initial Colour Randomization
   srand(time(NULL));
   for(int i=1; i<=config.totalRandomizedColors; i++){
       fg = config.asciiColors[rand() % 8];
       bg = config.asciiColors[rand() % 8];
       init_pair(i+config.totalASCIIcolors+config.totalManualColors, fg, bg);
   }


}

void gen_randColorMap(WINDOW *win, int y, int x, int h, int w){
    // Draw Random bitmap
    int randColorID=1;
    int totalAvailableColour = config.totalASCIIcolors+config.totalManualColors+config.totalRandomizedColors;

    srand(time(NULL));
    for (int i = y; i < h; i++) {
        wmove(win, i, x);
        for (int j = 0; j < w; j++) {
            randColorID = rand()%totalAvailableColour;
            wattron(win, COLOR_PAIR(randColorID));
            waddch(win, 'A' + rand() % 26);
            wattroff(win, COLOR_PAIR(randColorID));
        }
    }
    wrefresh(win);
}

void drawCMDStr(WINDOW *win, int y, int x, int show, int alignX, int is_cmd, int colorID, const char* msg){
//    FILE *pp;
    cbreak();
    // int strLen;
    /* if alignX==0 : left align
          alignX==1 : centre align
          alignX==2 : right align
    */
    int positionX=0;

    if(is_cmd==0){
        if(show>0){
            if(alignX==0){positionX=x;}
            else if(alignX==1){
                positionX = x-(strlen(msg)/2);
                // wmove(win, y, x-(strlen(msg)/2));
            }
            else if(alignX==2){
                positionX = x-strlen(msg);
                wmove(win, y, x-strlen(msg));
            }
            /*wattron(win, COLOR_PAIR(colorID));
            waddstr(win, msg);
            wattroff(win, COLOR_PAIR(colorID));*/
            draw_charArr(win, y, positionX, colorID, msg);
            wrefresh(win);
        }

    }
    else{
        FILE *pp;

        if ((pp = popen(msg, "r")) != 0) {
            // if(show>0){
                char buffer[BUFSIZ];
                while (fgets(buffer, sizeof(buffer), pp) != 0) {
                    if(show>0){
                        if(alignX==0){positionX = x;}
                        else if(alignX==1){
                            positionX = x-(strlen(buffer)/2);
                            //positionX = x-(sizeof(buffer)/sizeof(buffer[0]))/2;
                            //wmove(win, y, x-(strlen(buffer)/2));
                        }
                        else if(alignX==2){
                            // wmove(win, y, x-strlen(buffer));
                            // positionX = x-(sizeof(buffer)/sizeof(buffer[0]));
                            positionX = x-strlen(buffer);
                        }
                        draw_charArr(win, y, positionX, colorID, buffer);
                        /*wattron(win, COLOR_PAIR(colorID));
                        waddstr(win, buffer);
                        wattroff(win, COLOR_PAIR(colorID));*/
                        wrefresh(win);
                    }
                }
                pclose(pp);
            //}
        }
    }
}

void show_datetime(WINDOW *win, int y, int x){
    drawCMDStr(win, y, x, 1, 1, 1, 9, config.dateTimeCMD);
}

void genProfilePicture(int h, int w, int y, int x){
    // Draw Random bitmap
    int randColorID=1;
    int totalAvailableColour = config.totalASCIIcolors+config.totalManualColors+config.totalRandomizedColors;

    box(accountPicBox, 0, 0);
    srand(time(NULL));
    for (int i = y; i < h; i++) {
        wmove(accountPicBox, i, x);
        for (int j = 0; j < w; j++) {
           randColorID = rand()%totalAvailableColour;

            wattron(accountPicBox, COLOR_PAIR(randColorID));
            waddch(accountPicBox, 'A' + rand() % 26);
            wattroff(accountPicBox, COLOR_PAIR(randColorID));
        }
    }
    wrefresh(accountPicBox);
}
void messageBoxWindow(int h, int w, int y, int x, int is_cmd, int colorID, char* title, char* msg){

   /* If is_cmd==1,  then the supplied char array, will be eecuted and the output will be printed in the message box
      else if is_cmd==0, then the message will directly be printed in the message box
   */

    FILE *pp;
    int ch;

    messageBoxBorderWindow = newwin(h, w, y, x);
    messageBox_msg = newwin(h-6, w-4, y+4, x+2);

    box(messageBoxBorderWindow, 0, 0);

    // Draw Message Box Title

    draw_charArr(messageBoxBorderWindow, 2, (w-(sizeof(title)/sizeof(title[0])))/2, colorID, title);

    wrefresh(messageBoxBorderWindow);

    noecho();
    keypad(messageBox_msg, TRUE);

    wmove(messageBox_msg, 0, 0);

    if(is_cmd==1){

        if ((pp = popen(msg, "r")) != 0){
            char buffer[BUFSIZ];
            while (fgets(buffer, sizeof(buffer), pp) != 0) {

                waddstr(messageBox_msg, buffer);
            }
            wrefresh(messageBox_msg);
            pclose(pp);
        }
    }
    else{
        mvwprintw(messageBox_msg, 0, 0, msg);
        wrefresh(messageBox_msg);
    }


    do{
        ch = wgetch(messageBox_msg);     /* refresh, accept single keystroke of input */
        if((ch == config.KEY_ESCAPE) || (ch == '\n') || (ch == '\t') || (ch == 'q') || (ch == KEY_BACKSPACE) || (ch == 'w') || (ch == 'a') || (ch == 's') || (ch == 'd') || (ch == '4') || (ch == '8') || (ch == '2') || (ch == '6') || (ch == '5') || (ch == KEY_HOME) || (ch == KEY_EXIT)){ // If Enter is pressed

            wclear(messageBox_msg);
            werase(messageBox_msg);
            // delwin(messageBox_msg);

            wclear(messageBoxBorderWindow);
            werase(messageBoxBorderWindow);
            // delwin(messageBoxBorderWindow);

            wrefresh(messageBox_msg);
            wrefresh(messageBoxBorderWindow);

            // delwin(messageBox_msg);
            // delwin(messageBoxBorderWindow);

            genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            break;
        }
        else if(ch == '\t'){
            // login_passField(win, (loginBoxMaxY/2), (loginBoxMaxX*0.25)+14);
        }
        else{}
    }while(1);
}

void messageBoxWindow(int h, int w, int y, int x, int is_cmd, int colorID, const char* title, const char* msg){
    char* arr = strdup(title);
    char* arr2 = strdup(msg);
    //free(title);free(msg);
    messageBoxWindow(h, w, y, x, is_cmd, colorID, arr, arr2);
    free(arr);free(arr2);//arr=nullptr;arr2=nullptr;
    //delete arr; delete arr2;
}

void messageBoxWindow(int h, int w, int y, int x, int is_cmd, int colorID, char seperator, char* flatKeyValueArr){

    char* msgBoxKey=nullptr;
    char* msgBoxValue=nullptr;

    data_handler.getFlatKey(msgBoxKey, seperator, flatKeyValueArr);
    data_handler.getFlatValue(msgBoxValue, seperator, flatKeyValueArr);
    messageBoxWindow(h, w, y, x, is_cmd, colorID, msgBoxKey, msgBoxValue);
    if(msgBoxKey!=nullptr){free(msgBoxKey);}//msgBoxKey=nullptr;}
    if(msgBoxValue!=nullptr){free(msgBoxValue);}//msgBoxValue=nullptr;}
    //delete msgBoxKey; delete msgBoxValue;

}

void messageBoxWindow(int h, int w, int y, int x, int is_cmd, int colorID, char seperator, const char* flatKeyValueArr){
    char* arr = strdup(flatKeyValueArr);
    //free(flatKeyValueArr);
    messageBoxWindow(h, w, y, x, is_cmd, colorID, seperator, arr);
    if(arr!=nullptr){free(arr);}//arr=nullptr;}
//    delete arr;
}

int getSelectedSubItemID(int minY, int minX, int highlightedItemIndex, int colorID, char seperator, char* itemArray){
    // Generate New Window dynamically, on exit delete/free the window before breaking out from the function, to free up sys resource
    if(itemArray!=nullptr || sizeof(itemArray)/sizeof(itemArray[0])>0){
        int maxY = data_handler.getCharFreq(seperator, itemArray) + 1;
        if(highlightedItemIndex<=maxY-2 && highlightedItemIndex>=0){
            int maxX = data_handler.maxFlatItemLen(seperator, itemArray) + 1;
            int colorActivated = 0;
            int currentLineCnt;

//            subItemListWindow = newwin(maxY+1, maxX+1, minY, minX);
//            keypad(subItemListWindow, TRUE);
//            box(subItemListWindow, 0, 0);

            int ch;

            do{
            currentLineCnt=0;
            colorActivated=0;

            subItemListWindow = newwin(maxY+1, maxX+1, minY, minX);
            keypad(subItemListWindow, TRUE);
            box(subItemListWindow, 0, 0);

            wmove(subItemListWindow, currentLineCnt+1, 1);

            for(int i=0; (itemArray[i]!=seperator && itemArray[i+1]!='\0') || itemArray[i+1]!='\0' ; i++){

                if(itemArray[i]==seperator){currentLineCnt++;wmove(subItemListWindow, currentLineCnt+1, 1);i++;}

                if(currentLineCnt==highlightedItemIndex && colorActivated==0){wattron(subItemListWindow, COLOR_PAIR(colorID));colorActivated=1;}
                else if(currentLineCnt>highlightedItemIndex && colorActivated==1){wattroff(subItemListWindow, COLOR_PAIR(colorID));colorActivated=0;}
                waddch(subItemListWindow, itemArray[i]);

            }
            wrefresh(subItemListWindow);

            ch = wgetch(subItemListWindow);     /* refresh, accept single keystroke of input */
                if((ch == config.KEY_ESCAPE) || (ch==KEY_LEFT) || (ch=='a') || (ch == 'q') || (ch == KEY_HOME) || (ch == KEY_EXIT)){ // If Enter is pressed
                    // data_handler.freeArray(maxY-2, maxX-2, itemArray);
                    wclear(subItemListWindow);
                    werase(subItemListWindow);
                    wrefresh(subItemListWindow);
                    return -1;
                    //break;
                }
                else if((ch == '\n') || (ch==KEY_RIGHT) || (ch == 'd') || (ch=='6') || (ch=='5')){
                    // Execute Message Window
                    //messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, charArray[0][titleBarItemTree[1]], charArray[1][titleBarItemTree[1]]);

                    wclear(subItemListWindow);
                    werase(subItemListWindow);
                    wrefresh(subItemListWindow);

                    return highlightedItemIndex;

                }
                else if((ch == '\t') || (ch==KEY_DOWN) || (ch == 's') || (ch=='2')){
                    wclear(subItemListWindow);
                    werase(subItemListWindow);

                    // if(titleBarItemTree[1]>=(maxY-3)){titleBarItemTree[1]=0;}
                    if(highlightedItemIndex>=(maxY-2)){highlightedItemIndex=0;}
                    else{highlightedItemIndex++;}
                }
                else if((ch == KEY_BACKSPACE) || (ch==KEY_UP) || (ch == 'w') || (ch=='8')){
                    wclear(subItemListWindow);
                    werase(subItemListWindow);

                    if(highlightedItemIndex<=0){highlightedItemIndex=maxY-2;}
                    else{highlightedItemIndex--;}


                }
            }while(1);

        }

    }
    return -1;
}

char* getSelectedSubItemNameByID(char seperator, char* itemArray, char* itemName, int itemNumber){
    if(itemNumber<0){return nullptr;}
    //int itemNumber = 0;
    int currentLineCnt=0;
    // int totalChar=0;
    //itemNumber = getSelectedSubItemID(minY, minX, highlightedItemIndex, colorID, seperator, itemArray);

    /*int cnt=0;
    int maxLen=0;
    for(int i=0;itemArray[i]!='\0';i++){
        if(itemArray[i]==seperator || arr[i+1]=='\0'){if(maxLen<cnt){maxLen=cnt;}cnt=0;}
        else{cnt++;}
    }*/
//    int itemLen = data_handler.maxFlatItemLen(seperator, itemArray);
    int itemLen=0;
    int currentLineCntindex=0;
    for(int i=0; itemArray[i]!='\0' ; i++){
        if(itemArray[i]==seperator){currentLineCnt++;i++;}

        if(currentLineCnt==itemNumber){
            currentLineCntindex = i;
//            while(itemArray[i+itemLen]!=seperator || (itemArray[i+itemLen]!=seperator && itemArray[i+itemLen+1]!='\0')){itemLen++;} // (itemArray[i+j]!=seperator && itemArray[i+j+1]!='\0')
            while(1){if(itemArray[i+itemLen]==seperator || itemArray[i+itemLen]=='\0'){break;}else{itemLen++;}}
//            for(itemLen=0; itemArray[i+itemLen]!='\0'; itemLen++){if(itemArray[i+itemLen]==seperator || itemArray[i+itemLen]=='\0'){break;}}
            break;
        }
    }

//    currentLineCnt=0;
    itemName = static_cast<char*>(std::malloc(itemLen * sizeof(char)));
    itemName[itemLen]='\0';
    wrefresh(mainScreenWin);

    for(int i=currentLineCntindex; itemArray[i]!='\0' ; i++){
        if(itemArray[i]==seperator){currentLineCnt++;i++;}

        if(currentLineCnt==itemNumber){
            // mvwaddch(mainScreenWin, 0, 1, 'a'+itemLen);wrefresh(mainScreenWin);
            for(int j=0; j<itemLen; j++){itemName[j]=itemArray[i+j];}

            break;
        }
    }
    // mvwprintw(mainScreenWin, 0, 2, itemName);wrefresh(mainScreenWin);
    // mvwaddch(mainScreenWin, 0, 0, 'a'+itemNumber);
    // wrefresh(mainScreenWin);
    // itemName = strdup("hola");
    return itemName;
}

char* getSelectedSubItemName(int minY, int minX, int highlightedItemIndex, int colorID, char seperator, char* itemArray, char* itemName){

    int itemNumber = 0;
    int currentLineCnt=0;
    // int totalChar=0;
    itemNumber = getSelectedSubItemID(minY, minX, highlightedItemIndex, colorID, seperator, itemArray);
    if(itemNumber==-1){return nullptr;}

    /*int cnt=0;
    int maxLen=0;
    for(int i=0;itemArray[i]!='\0';i++){
        if(itemArray[i]==seperator || arr[i+1]=='\0'){if(maxLen<cnt){maxLen=cnt;}cnt=0;}
        else{cnt++;}
    }*/
//    int itemLen = data_handler.maxFlatItemLen(seperator, itemArray);
    int itemLen=0;
    int currentLineCntindex=0;
    for(int i=0; itemArray[i]!='\0' ; i++){
        if(itemArray[i]==seperator){currentLineCnt++;i++;}

        if(currentLineCnt==itemNumber){
            currentLineCntindex = i;
//            while(itemArray[i+itemLen]!=seperator || (itemArray[i+itemLen]!=seperator && itemArray[i+itemLen+1]!='\0')){itemLen++;} // (itemArray[i+j]!=seperator && itemArray[i+j+1]!='\0')
            while(1){if(itemArray[i+itemLen]==seperator || itemArray[i+itemLen]=='\0'){break;}else{itemLen++;}}
//            for(itemLen=0; itemArray[i+itemLen]!='\0'; itemLen++){if(itemArray[i+itemLen]==seperator || itemArray[i+itemLen]=='\0'){break;}}
            break;
        }
    }

//    currentLineCnt=0;
    itemName = static_cast<char*>(std::malloc(itemLen * sizeof(char)));
    itemName[itemLen]='\0';
//    wrefresh(mainScreenWin);

    for(int i=currentLineCntindex; itemArray[i]!='\0' ; i++){
        if(itemArray[i]==seperator){currentLineCnt++;i++;}

        if(currentLineCnt==itemNumber){
//             mvwaddch(mainScreenWin, 0, 1, 'a'+itemLen);wrefresh(mainScreenWin);
            for(int j=0; j<itemLen; j++){itemName[j]=itemArray[i+j];}

            break;
        }
    }
    // mvwprintw(mainScreenWin, 0, 2, itemName);wrefresh(mainScreenWin);
    // mvwaddch(mainScreenWin, 0, 0, 'a'+itemNumber);
    // wrefresh(mainScreenWin);
    // itemName = strdup("hola");
    return itemName;
}


int draw_titlebar(WINDOW *titlebar, int colorID, int active, int itemID=-1)
{

    int ch, titlebarCoordX, titlebarCoordY;
    int positionCoordX;
    int spacingX = 3;
    noecho();
    char* cmd=nullptr;
    int itemIndex;
    char* title=nullptr;
    char* itemName=nullptr;

    int inactiveColorID = 9;
    int currentColorID = 9;
    titleBarItemTree[0]=itemID;

    do{
        keypad(titlebar, TRUE);

        getmaxyx(titlebar, titlebarCoordY, titlebarCoordX);
        positionCoordX=0;

//        if(active==0){wattron(titlebar, COLOR_PAIR(colorID));}
        if(active==0 || titleBarItemTree[0]==0){currentColorID = colorID;}
        draw_charArr(titlebar, 1, positionCoordX+spacingX, currentColorID, config.powerBTN_text);
        currentColorID = inactiveColorID;

        positionCoordX+=strlen(config.powerBTN_text);

        if(active==0 || titleBarItemTree[0]==1){currentColorID = colorID;}
        draw_charArr(titlebar, 1, positionCoordX+(spacingX)*2, currentColorID, config.utilitiesBTN_text);
        currentColorID = inactiveColorID;

        if(active==0 || titleBarItemTree[0]==2){wattron(titlebar, COLOR_PAIR(colorID));}
        wmove(titlebar, 1, titlebarCoordX-(strlen(user.XDG_SESSION_TYPE_NAME)+3+strlen(user.XDG_SESSION_NAME)+spacingX));
        waddstr(titlebar, user.XDG_SESSION_TYPE_NAME);
        if(active==0 || titleBarItemTree[0]==2){wattroff(titlebar, COLOR_PAIR(colorID));}

        waddstr(titlebar, " : ");

        if(active==0 || titleBarItemTree[0]==3){wattron(titlebar, COLOR_PAIR(colorID));}
        wmove(titlebar, 1, titlebarCoordX-(strlen(user.XDG_SESSION_NAME)+spacingX));
        waddstr(titlebar, user.XDG_SESSION_NAME);
        if(active==0 || titleBarItemTree[0]==3){wattroff(titlebar, COLOR_PAIR(colorID));}

        //wmove(titlebar, 1, titlebarCoordX/2);
//        show_datetime(titlebar, 1, (titlebarCoordX/2)-16); // Show DateTime
        drawCMDStr(mainScreenWin, 1, (titlebarCoordX/2), 1, 1, 1, 9, config.dateTimeCMD); // Show DateTime


//        if(active==0){wattroff(titlebar, COLOR_PAIR(colorID));}
        wrefresh(titlebar);

        ///////////////////////////
        if(active==0 || titleBarItemTree[0]<0){break;}
     //   else if(titleBarItemTree[0]==-2){return 99;}


        ch = wgetch(titlebar);
        if((ch=='w') || (ch=='q') || (ch==KEY_UP) || (ch==config.KEY_ESCAPE) || (ch=='8')){
            titleBarItemTree[0]=-1;
        }
        else if((ch=='\t') || (ch==KEY_RIGHT) || (ch==' ') || (ch=='d') || (ch=='6')){
                if(titleBarItemTree[0]>=(config.titleBarHoverableItemCount-1)){titleBarItemTree[0]=0;}
                else{titleBarItemTree[0]++;}
        }
        else if((ch==KEY_BACKSPACE) || (ch==KEY_LEFT) || (ch=='a') || (ch=='4')){
                if(titleBarItemTree[0]<=0){titleBarItemTree[0]=config.titleBarHoverableItemCount-1;}
                else{titleBarItemTree[0]--;}
        }
        else if((ch=='\n') || (ch==KEY_DOWN) || (ch=='s') || (ch=='5') || (ch=='2')){
            if(titleBarItemTree[0]==0){
                titleBarItemTree[1]=0;

                itemIndex = getSelectedSubItemID(titlebarCoordY, spacingX, 0, 13, '\7', config.powerList_text);
                if(itemIndex!=-1){
                    title = getSelectedSubItemNameByID('\7', config.powerList_text, title, itemIndex);

                    if(itemIndex==0){// Sleep
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.sleepCMD);
                    }
                    else if(itemIndex==1){// Restart
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.restartCMD);
                    }
                    else if(itemIndex==2){// Shutdown
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.shutdownCMD);
                    }
                    free(title);title=nullptr;
                }

                //char* title = getSelectedSubItemNameByID('\7', config.powerList_text, title, itemIndex);
                //messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, title, title);
                //free(title);


            }
            else if(titleBarItemTree[0]==1){
                titleBarItemTree[1]=0;

                itemIndex =  getSelectedSubItemID(titlebarCoordY, spacingX, 0, 13, '\7', config.utilitiesList_text);
                if(itemIndex!=-1){
                    title = getSelectedSubItemNameByID('\7', config.utilitiesList_text, title, itemIndex);

                    if(itemIndex==0){// Show Calender
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.calenderCMD);
                    }
                    else if(itemIndex==1){// Show CPU Status
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.cpuStatusCMD);
                    }
                    else if(itemIndex==2){// Show Network Status
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, config.networkStatusCMD);
                    }
                    else if(itemIndex==3){// Refresh Windows
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, 12, title, title);
                    }
                    else if(itemIndex==4){// Exit Application
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 10, "Exit", "Exiting Display Manager :/");

                        titleBarItemTree[0]=-2;
                    }
                    free(title);title=nullptr;
                }


            }

            else if(titleBarItemTree[0]==2){
                titleBarItemTree[1]=0;

                itemIndex = getSelectedSubItemID(titlebarCoordY, spacingX, 0, 13, '\7', config.currentUserDesktopEnvComProtocol);
                title = strdup("XDG Session Type");

                if(itemIndex!=-1){
                    if(titleBarItemTree[0]==2){wattron(titlebar, COLOR_PAIR(1));}
                    wmove(titlebar, 1, titlebarCoordX-(strlen(user.XDG_SESSION_TYPE_NAME)+3+strlen(user.XDG_SESSION_NAME)+spacingX));
                    waddstr(titlebar, user.XDG_SESSION_TYPE_NAME);
                    if(titleBarItemTree[0]==2){wattroff(titlebar, COLOR_PAIR(1));}
                    waddstr(titlebar, " : ");

                    free(user.XDG_SESSION_TYPE_NAME);
                    itemName = getSelectedSubItemNameByID('\7', config.currentUserDesktopEnvComProtocol, title, itemIndex);
                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, title, itemName);
                    user.XDG_SESSION_TYPE = itemIndex;
                    user.XDG_SESSION_TYPE_NAME = strdup(itemName);
                    free(title);free(itemName);itemName=nullptr;title=nullptr;
                }
            }

            else if(titleBarItemTree[0]==3){
                titleBarItemTree[1]=0;


                if(user.XDG_SESSION_TYPE==DS_XINITRC || user.XDG_SESSION_TYPE==DS_XORG){
                    cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[", "]$", "Xprotocol", config.xsessions);
                    //free(cmd);
                }
                else if(user.XDG_SESSION_TYPE==DS_WAYLAND){
                    cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[", "]$", "Xprotocol", config.waylandsessions);
                    //free(cmd);
                }

                if(user.XDG_SESSION_TYPE!=DS_SHELL && user.XDG_SESSION_TYPE!=DS_DEFAULT){
                    if(config.availableUserDesktopEnv!=nullptr){free(config.availableUserDesktopEnv);config.availableUserDesktopEnv=nullptr;}
                    config.availableUserDesktopEnv = cmd_executor.fetchExecOutput(config.availableUserDesktopEnv, cmd);
                    free(cmd);cmd=nullptr;
                    itemName = getSelectedSubItemName(titlebarCoordY, spacingX, 0, 13, '\7', config.availableUserDesktopEnv, itemName);
                    title = strdup("Selected Environment");

                    if(itemName!=nullptr){
//                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, title, itemName);


/*                    if(titleBarItemTree[0]==2){wattron(titlebar, COLOR_PAIR(1));}
                    wmove(titlebar, 1, titlebarCoordX-(strlen(user.XDG_SESSION_NAME)+strlen(user.XDG_SESSION_TYPE_NAME)+3+spacingX));
                    waddstr(titlebar, user.XDG_SESSION_TYPE_NAME);
                    waddstr(titlebar, " : ");
                    waddstr(titlebar, user.XDG_SESSION_NAME);
                    if(titleBarItemTree[0]==2){wattroff(titlebar, COLOR_PAIR(1));}
*/

                        if(titleBarItemTree[0]==3){wattron(titlebar, COLOR_PAIR(1));}
                        wmove(titlebar, 1, titlebarCoordX-(strlen(user.XDG_SESSION_TYPE_NAME)+3+strlen(user.XDG_SESSION_NAME)+spacingX));
                        waddstr(titlebar, user.XDG_SESSION_TYPE_NAME);
                        waddstr(titlebar, " : ");
                        waddstr(titlebar, user.XDG_SESSION_NAME);
                        if(titleBarItemTree[0]==3){wattroff(titlebar, COLOR_PAIR(1));}

                        free(user.XDG_SESSION_NAME);
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, title, itemName);
                    //user.XDG_SESSION_NAME = data_handler.cpArray(user.XDG_SESSION_NAME, itemName);
                        user.XDG_SESSION_NAME = strdup(itemName);
                    }
                    free(itemName);free(title);itemName=nullptr;title=nullptr;
                }
            }
        }

    }while(1);

    if(itemName!=nullptr){free(itemName);}
    if(title!=nullptr){free(title);}
    if(cmd!=nullptr){free(cmd);}

    if(titleBarItemTree[0]==-2){return 99;}

    return 0;

}

void updateRequestedUSRENV(){
    if(user.usernameVerified){

        draw_titlebar(titleBar_subwin, 1, 0, -1);

        char* cmd=nullptr;
        if(user.XDG_SESSION_TYPE==DS_DEFAULT){
           cmd = data_handler.replaceStr(config.getUserDesktopEnvTypeCMD, "$[", "]$", "USER", user.username);
           user.XDG_SESSION_TYPE_NAME = cmd_executor.fetchExecOutput(user.XDG_SESSION_TYPE_NAME, cmd);
           if(user.XDG_SESSION_TYPE_NAME==nullptr){user.XDG_SESSION_TYPE_NAME=config.default_text;}
           free(cmd);cmd=nullptr;

           user.XDG_SESSION_TYPE = data_handler.getItemID('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE_NAME);
           //trackID=user.XDG_SESSION_TYPE;
           if(user.XDG_SESSION_TYPE==-1 || user.XDG_SESSION_TYPE==DS_DEFAULT){
               user.XDG_SESSION_TYPE=DS_XORG;
               user.XDG_SESSION_TYPE_NAME=data_handler.getItemName('\7', config.currentUserDesktopEnvComProtocol, DS_XORG, user.XDG_SESSION_TYPE_NAME);
           //trackID=-1;
           }
        }


        FILE *pp;
        cbreak();

        cmd = data_handler.replaceStr(config.currentUserDesktopEnvCMD, "$[", "]$", "USER", user.username);
        user.XDG_SESSION_NAME = cmd_executor.fetchExecOutput(user.XDG_SESSION_NAME, cmd);
        if(user.XDG_SESSION_TYPE_NAME==nullptr){user.XDG_SESSION_TYPE_NAME=config.default_text;}
/*        if ((pp = popen(cmd, "r")) != 0) {
            char buffer[BUFSIZ];
            //free(user.XDG_SESSION_NAME);
            while (fgets(buffer, sizeof(buffer), pp) != 0) {
               for(int i=0; buffer[i+1]!='\0'; i++){user.XDG_SESSION_NAME[i] = buffer[i];}
            }
            pclose(pp);
        }
*/
        free(cmd);//cmd=nullptr;


//        delete cmd;
    }
}

void authChrVisibilityPattern(WINDOW *win, int y, int x, int* arr){

    /*
     ------------------------------------------------------------------------------------------------------------------

     0 : COLOR_BLACK -> NONE | Common
     1: COLOR_GREEN -> hide chr

     ------------------------------------
     2: COLOR_RED -> show chr
     3: COLOR_YELLOW -> mask chr | with *
     ----------------------------------

     ----------------------------------
     4: COLOR_WHITE -> input chr == output chr && output chr selection is not randomized
     5: COLOR_MAGENTA -> input chr != output chr && output chr selection is randomized
     ----------------------------------

     ----------------------------------
     6: COLOR_BLUE -> assuming the length of output string == length of input string && output string may/may not be masked
     7: COLOR_CYAN -> assuming the length of output string != length of input string && output string may/may not be masked
     ----------------------------------
     -------------------------------------------------------------------------------------------------------------------

     MAX COLOR PIXEL LENGTH -> 3
     MIN COLOR PIXEL LENGTH -> 1

     -------------------------------------------------------------------------------------------------------------------

     <output_string_type> : visible chr/masked chr/hidden chr [COLOR_RED/COLOR_YELLOW/COLOR_GREEN]
     <output_string_randomization> : true/false [COLOR_MAGENTA/COLOR_WHITE]
     <output_string_length_randomization> : true/false [COLOR_CYAN/COLOR_BLUE]

     serial: <output_string_type><output_string_randomization><output_string_length_randomization>

     -------------------------------------------------------------------------------------------------------------------
    */

    wmove(win, y, x);
    int j=2;
    // while(arr[i]!='\0'){
    for(int i=0; i<=(sizeof(arr)/sizeof(arr[0])); i++){
        // if(i>0){j+2}
        wattron(win, COLOR_PAIR(arr[i]+j));
        waddch(win, arr[i]);
        wattroff(win, COLOR_PAIR(arr[i]+j));
        j+=2;

    //}
    }
    wrefresh(win);

}

void filluserFullName(char* username){
    if(user.userFullName!=nullptr){
        draw_charArr(mainScreenWin, (winMaxY*0.75)-2,(winMaxX/2)-(strlen(user.userFullName)/2), 1, user.userFullName);
        free(user.userFullName);user.userFullName=nullptr;
//        user.userFullName = nullptr;
    }
    // free(user.userFullName);

    if(user.usernameVerified){
        char* cmd = data_handler.replaceStr(config.getUserFullnameCMD, "$[", "]$", "USER", username);
    //draw_charArr(mainScreenWin, (winMaxY*0.75)-2, 0 , 13, cmd);
        user.userFullName = cmd_executor.fetchExecOutput(user.userFullName, cmd);
        free(cmd);//cmd=nullptr;delete cmd;
    }
    if(user.userFullName!=nullptr){
        draw_charArr(mainScreenWin, (winMaxY*0.75)-2,(winMaxX/2)-(strlen(user.userFullName)/2), 13, user.userFullName);
        wrefresh(mainScreenWin);
    }
//    else{free(user.userFullName);}
//    wrefresh(mainScreenWin);
}

int authenticateButton(){

    draw_charArr(authBox, loginBoxMaxY-1, loginBoxMaxX-10, 13, config.loginBTN_text);

    wrefresh(authBox);

    int retCode;

    int ch;


    do{
        ch = wgetch(authBox);
        if((ch=='w') || (ch=='q') || (ch==KEY_UP) || (ch==config.KEY_ESCAPE) || (ch=='8')){retCode = 1;break;}
        else if((ch==KEY_BACKSPACE) || (ch==KEY_LEFT) || (ch=='a') || (ch=='4')){retCode = 0;break;}
        else if((ch=='\t') || (ch==KEY_DOWN) || (ch==' ') || (ch=='s') || (ch=='2')){
              retCode = 3;break;
//            if(user.usernameVerified){retCode = 3;break;}
//            else{retCode = 0;break;}
        }
        else if((ch=='\n') || (ch==KEY_RIGHT) || (ch=='d') || (ch=='5') || (ch=='6')){

            if(strlen(user.username)<=0 || strlen(user.userpass)<=0){
                messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 11, '\6', config.emptyCredPassed);
            }
            else if(user.usernameVerified && chkPAMAuthStatus(user.username, user.userpass)==PAM_SUCCESS && auth_management.authCheck(config.usrHomeDir, user.username, user.userpass)==1){
//            else if(auth_management.authCheck(config.usrHomeDir, username, userpass)==1){
//            else if(chkPAMAuthStatus(username, userpass)==PAM_SUCCESS){
                //session_management.createSessionKey(SESSION_KEY_LENGTH-1, SESSION_KEY);
                // chkpamAuth(username, userpass);
              // session_management.createSession(currentDesktopENV, config.usrHomeDir, username);
                //int PAMAuthStatus = chkPAMAuthStatus(username, userpass);
                //if(PAMAuthStatus==PAM_SUCCESS){
                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, '\6', config.loginSuccess_text);

                    int session_status = initiateSession(user.username, user.userpass);
                    //wrefresh(mainScreenWin);
                    if(session_status==1){
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 11, "Session Status", "Session Exited Successfully");
                    }
                    else if(session_status==0){
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 10, "Session Status", "Session Initialization Failed");
                    }
                    wrefresh(mainScreenWin);
                    //auth(desktop, username, userpass);
//                    cmd_executor.exec("su sys41x4 -c 'export DISPLAY=:0; startx'");
//                       cmd_executor.exec("");
/*                    int result = system("startx");
                    if (result != 0) {
                        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, "Login Failed", "Failed to start Xsession");
                    }
*/
                //}
                //else{messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 10, '\6', config.loginFailed_text);}*/
//                  mvwaddch(mainScreenWin, winMaxY-2, 0, 'a'+PAMAuthStatus);
//                  wrefresh(mainScreenWin);
//                messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 13, "status", 'a'+);
//                draw_charArr(mainScreenWin, winMaxY-2, winMaxX-(strlen(package)+2), 12, "xzc");
//                wrefresh(mainScreenWin);
            }
            else{
                messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 10, '\6', config.incorrectCred);
            }


            retCode = 0;break;
        }
   // else{}
    }while(1);

    draw_charArr(authBox, loginBoxMaxY-1, loginBoxMaxX-10, 9, config.loginBTN_text);

    wrefresh(authBox);

    // Check if user.username is valid

    // Check if Password is valid for the mentioned user.username


    // If user.username and pass is valid then create session with authSuccess()
    // Else Notify that the credentials are not valid
    return retCode;
}

int login_passField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int userpassChrCount = 0;
    int userpassLengthMax = sizeof(user.userpass)/sizeof(user.userpass[0]);
    int ch;
    // char* user.visible_userpass;
    // char user.visible_userpass[50]={'\0'};
    // int finish=0;
    // cbreak();
    noecho();
    keypad(win, TRUE);

    do{
         wmove(win, y, x);

        if(config.userpassVisibilityConf[0]!=2 && strlen(user.userpass)>0){
            // user.visible_userpass = mask_authInput(0, userpassVisibilityConf, user.userpass);
            // maskStr2(user.visible_userpass, strlen(user.userpass), '*');
            halkadm_security.mask_authInput(0, config.userpassVisibilityConf, config.visibleAuthStrLen, user.userpass, user.visible_userpass);
            wprintw(win, user.visible_userpass);
            userpassChrCount=strlen(user.userpass);
        }
        wrefresh(win);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if ((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            if(strlen(user.userpass)>0){
                gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);
            }
            break;
        }
        else if((userpassChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((userpassChrCount>=0) && (userpassChrCount<config.maxUserpassLen)) || ((userpassChrCount == config.maxUserpassLen) && (ch == KEY_BACKSPACE))){

            if(ch == KEY_BACKSPACE){ // If backspace is pressed
                userpassChrCount-=1;
                user.userpass[userpassChrCount] = '\0';
                mvwprintw(win, y, x+userpassChrCount, " ");
            }
//            else{
            else if(auth_management.chkCharAllowence(ch)==1){
                user.userpass[userpassChrCount] = ch;
                userpassChrCount++;
            }
        }
    }while (1);

    return 2;

}

int login_userField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int usernameChrCount = 0;
    int usernameLengthMax = sizeof(user.username)/sizeof(user.username[0]);
    int ch;
    // char user.visible_username[50]={'\0'};
    noecho();
    keypad(win, TRUE);

    do{
        wmove(win, y, x);

        if(config.usernameVisibilityConf[0]!=2 && strlen(user.username)>0){
            halkadm_security.mask_authInput(0, config.usernameVisibilityConf, config.visibleAuthStrLen, user.username, user.visible_username);
            wprintw(win, user.visible_username);
            usernameChrCount=strlen(user.username);
        }

        wrefresh(win);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            if(strlen(user.username)>0){
                user.usernameVerified = auth_management.usernameCheck(user.username);
                if(user.usernameVerified){
//               if(1){

/*                user.usernameHash = halkadm_crypto.getMD5hash(user.username, user.usernameHash);
                if(user.usernameHash!=nullptr){
                    char* title = strdup("Username MD5 Hash");
                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, 12, title, user.usernameHash);
                }
*/
                    gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);
                    filluserFullName(user.username);
//                    draw_charArr(mainScreenWin, (winMaxY/8)-2, (winMaxX/2)-(strlen("asdascasc")/2), 13, "asdascasc");
                    updateRequestedUSRENV();
                    draw_titlebar(titleBar_subwin, 13, 1, -1);
                }
            }
            break;
        }
        else if((usernameChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((usernameChrCount>=0) && (usernameChrCount<config.maxUsernameLen)) || ((usernameChrCount == config.maxUsernameLen) && (ch == KEY_BACKSPACE))){

            if(ch == KEY_BACKSPACE){ // If backspace is pressed
                usernameChrCount--;
                user.username[usernameChrCount] = '\0';
                // wmove(win, y, x+usernameChrCount);
                // waddch(win, ' ');
                mvwprintw(win, y, x+usernameChrCount, " ");
            }
            else if(auth_management.chkCharAllowence(ch)==1){
                    user.username[usernameChrCount] = ch;
                    usernameChrCount++;
            }
        }

        /* process the command keystroke */
    }while (1);

    return 1;

}


void drawAuthBox(int maxY, int maxX, int minY, int minX){

    // Draw Window
    // authBox = subwin(win, winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);
    /*loginBoxMinX = winMaxX/4;
    loginBoxMaxX = winMaxX/2;
    loginBoxMinY = winMaxY*0.75;
    loginBoxMaxY = winMaxY/8;*/

    // authBox = newwin(loginBoxMaxY, loginBoxMaxX, loginBoxMinY, loginBoxMinX);
    box(authBox, 0, 0);

    // Draw Auth Map
    //gen_randColorMap(authBox, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);

    // Auth Section //

    // Username Field
    mvwprintw(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX*0.25)+5, config.usernameFieldID_text);
    waddstr(authBox, " :");
    // Userpass Field
    mvwprintw(authBox, (loginBoxMaxY/2), (loginBoxMaxX*0.25)+5, config.userpassFieldID_text);
    waddstr(authBox, " :");
    // Login Button
    mvwprintw(authBox, loginBoxMaxY-1, loginBoxMaxX-10, config.loginBTN_text);


    wrefresh(authBox);
}


void freeMemory(){

    // This function must be called before exiting the program to clear the system allocated memory space

    // Free User SESSION_KEY allocated space
  //  free(SESSION_KEY);

/*
    // Free Username Space
    free(user.username);
    free(user.visible_username);
    // Free Userpass Space
    free(user.userpass);
    free(user.visible_userpass);
*/
    // TitleBar Item Tree Depth | Free Allocated Storage Space
    free(titleBarItemTree);

    // De-allocate space for config list for loginColourMatrix
    free(loginColourMatrixConf);

    user.deallocate();
    config.deallocate();

}

void allocateMemory(){
    // Create SESSION_KEY | SIZE 33
//    SESSION_KEY = static_cast<char*>(std::malloc(SESSION_KEY_LENGTH * sizeof(char)));
    // createSessionKey(SESSION_KEY_LENGTH-1, SESSION_KEY);

    // createSessionKey();
    // AutoSeeded_RNG rng; // cryptographically secure random number generator
    // calculate_hash(rng);

    // define max visible Username/Userpass Length

//    config.visibleAuthStrLen =  (winMaxX*0.25) - ((loginBoxMaxX*0.75)-strlen(config.usernameFieldID_text)-20);
      config.visibleAuthStrLen = (loginBoxMaxX-12)-((loginBoxMaxX*0.25)+14);
//    config.visibleAuthStrLen = 50;
    user.allocate();

    // TitleBar Item Tree Depth | Storage Space Allocation
    titleBarItemTree = static_cast<int*>(std::malloc(config.maxTitleBarItemTreeDepth * sizeof(int)));

    // Allocate space for config list for loginColourMatrix
    loginColourMatrixConf = static_cast<int*>(std::malloc(4 * sizeof(int)));

    // Allocate & Fill Space For Environment Names
    //config.availableUserDesktopEnv = strdup("Default");


}


void initWindow(){
    // Allocate Memory Space

//    allocateMemory();

    // initscr(); // Initialize Curses Screen Base

    getmaxyx(stdscr, winMaxY, winMaxX);



    // Account Pic coordinate and oeiention settings
    accountPicBoxMaxH = winMaxY*0.375;
    accountPicBoxMaxW = winMaxX/4;
    accountPicBoxMaxY = winMaxY/8;
    accountPicBoxMaxX = winMaxX*0.375;

    msgBoxMaxY=winMaxY/4; // Message Box Coordinates Y
    msgBoxMaxX=winMaxX/4; // Message Box Coordinates X
    msgBoxMaxH=winMaxY/2; //  Message Box Length H
    msgBoxMaxW=winMaxX/2; // Message Box Length W

    // Allocate Windows and subWindows //
    // Create Main Window
    mainScreenWin = newwin(winMaxY, winMaxX, 0, 0);
    accountPicBox = subwin(mainScreenWin, accountPicBoxMaxH, accountPicBoxMaxW, accountPicBoxMaxY, accountPicBoxMaxX); // Account Picture Box
    messageBox_msg = newwin(msgBoxMaxH-6, msgBoxMaxW-4, msgBoxMaxY+4, msgBoxMaxX+2);
    authBox = subwin(mainScreenWin, winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);
    titleBar_subwin = subwin(mainScreenWin, 3, winMaxX,0,0);

    getmaxyx(authBox, loginBoxMaxY, loginBoxMaxX);


    // Allocate Memory of Storage variables
    allocateMemory();

    // Setup Colours
    // start_color();
    initColor();


    draw_charArr(mainScreenWin, winMaxY-2, winMaxX-(strlen(config.package)+2), 13, config.package);
    drawCMDStr(mainScreenWin, winMaxY/2, winMaxX/2, 1, 1, 1, 13, config.getSystemBasicInfoCMD);
    drawCMDStr(mainScreenWin, (winMaxY/2)+1, winMaxX/2, 1, 1, 1, 13, config.getSystemUnameCMD);
//    draw_charArr(mainScreenWin, winMaxY-3, winMaxX-(strlen(config.capslock)+2), 13, config.desktopENVBTN_text);
    refresh();

    box(titleBar_subwin, 0, 0);
    box(mainScreenWin, 0, 0);

    draw_titlebar(titleBar_subwin, 13, 1, -1);

    // List Available Desktop Managers
//     list_available_desktop_environments(mainScreenWin, winMaxY/2, winMaxX/2);

    //// UserName Visibility colorMap
    authChrVisibilityPattern(authBox, (loginBoxMaxY/2)-1, loginBoxMaxX-10, config.usernameVisibilityConf);

    //// UserPass Visibility colorMap
    authChrVisibilityPattern(authBox, (loginBoxMaxY/2), loginBoxMaxX-10, config.userpassVisibilityConf);


    // Set Login Matrix Config
    setLoginMatrixWindow(authBox); // The LoginMatrix to be shown in the provided window
    loginMatrixSetConfig(1, 1, loginBoxMaxY-1, (loginBoxMaxX*0.25)-2);


    // Draw Auth Box
    drawAuthBox(winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);

    // refreshing the window
    wrefresh(mainScreenWin);
    wrefresh(titleBar_subwin);
    wrefresh(accountPicBox);

}

int main(int argc, char **argv)
{
    // load Default config Data
    config.load_default_CMD();
    config.load_default_keyValues();
    config.load_default_softwareInfo();
    //config.load_default_lang();
    config.error_default_lang();
    config.load_default_alertText();
    config.allocate();


    initscr();
    start_color();
    initWindow();
    int id=0;
    genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
    // Draw Auth Map
    gen_randColorMap(authBox, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);

    switch_tty();
    while(1){
        // genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);

        if(id==0){
            id = login_userField(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX*0.25)+14);
        }
        else if(id==1){
            id = login_passField(authBox, (loginBoxMaxY/2), (loginBoxMaxX*0.25)+14);
        }
        else if(id==2){
            //authSuccess();
            id = authenticateButton();
        }
        else if(id==3){
            id = draw_titlebar(titleBar_subwin, 13, 1, 0);
        }
        else if(id == 99){break;}
    }

//    getch();
    endwin();
    freeMemory();
    return 0;
}

