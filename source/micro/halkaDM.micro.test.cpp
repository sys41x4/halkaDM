#include "config/config.h"
#include "lib/cryptography.h"
#include "lib/security.h"
#include <locale.h>
#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>
// #include <openssl/evp.h>

using namespace std;

/*
// TitleBar Item ID List

1. Power
2. Utilities
3. <DateTime>
4. Desktop Manager

*/

// char desktopEnvironmentsSubItems[30][30];
char** desktopEnvironmentsSubItems;
int *titleBarItemTree;

int availableDesktopManagerCount;

// char desktopManagerSubItems[1][20];

WINDOW *mainScreenWin, *subItemListWindow, *loginColourMatrixWin, *authBox, *accountPicBox, *titleBar_subwin; // User Account Picture Box
WINDOW *messageBoxBorderWindow, *messageBox_msg;

int winMaxX, winMaxY;
int accountPicBoxMaxX, accountPicBoxMaxY, accountPicBoxMaxW, accountPicBoxMaxH;
int msgBoxMaxX, msgBoxMaxY, msgBoxMaxW, msgBoxMaxH;
int loginBoxMaxX, loginBoxMaxY;

char *username, *visible_username;
char *userpass, *visible_userpass;

int *loginColourMatrixConf;


char* fillArray(int c, char* arr){
    //int cols = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);
    //int cols1 = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);

    arr = static_cast<char*>(std::malloc(c * sizeof(char)));
    arr[0] = '\0';
    return arr;
}

char** fillArray(int r, int c, char** arr){
    //int cols = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);
    //int cols1 = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);

    arr = static_cast<char**>(std::malloc(r * sizeof(char*)));
    for (int i = 0; i < c; i++) {
        arr[i] = static_cast<char*>(std::malloc(c * sizeof(char)));
        arr[i][0] = '\0';
    }
    return arr;
}

char*** fillArray(int r, int rc, int c, char*** arr){
    //int cols = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);
    //int cols1 = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);

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


void freeArray(int r, char** arr){
    for (int i = 0; i < r; i++) {
        free(arr[i]);
    }
    free(arr);
}

void freeArray(int r, int c, char*** arr){
    for (int i = 0; i < r; i++) {
        //for(int j=0; j<c; j++){
          //  free(arr[i][j]);
        //}
        free(arr[i]);
    }
    free(arr);
}

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
   for(int i=1; i<=totalASCIIcolors; i++){
       init_pair(i, asciiColors[i+storedColorsStartIndex], asciiColors[i+storedColorsStartIndex]);
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
   for(int i=1; i<=totalRandomizedColors; i++){
       fg = asciiColors[rand() % 8];
       bg = asciiColors[rand() % 8];
       init_pair(i+totalASCIIcolors+totalManualColors, fg, bg);
   }


}

void createSessionKey(int len, char* arr){
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        // arr[i] = '!'+(rand() % 94);
        arr[i] = 'a'+(rand() % 25);
    }
    //unsigned char result[MD5_DIGEST_LENGTH];
    //MD5((unsigned char*)arr, strlen(arr), result);

    //for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    //    sprintf(&arr[i*2], "%02x", (unsigned int)result[i]);
    //}
}

void gen_randColorMap(WINDOW *win, int y, int x, int h, int w){
    // Draw Random bitmap
    int randColorID=1;
    int totalAvailableColour = totalASCIIcolors+totalManualColors+totalRandomizedColors;

    srand(time(NULL));
    for (int i = y; i < h; i++) {
        wmove(win, i, x);
        for (int j = 0; j < w; j++) {
              randColorID = rand()%totalAvailableColour;
   //         int fg = asciiColors[rand() % 8] + rand()%7;
   //         int bg = asciiColors[rand() % 8] * rand()%15;
   //         init_pair(randColorID, fg, bg);
            // wattron(win, COLOR_PAIR(color));
            wattron(win, COLOR_PAIR(randColorID));
            waddch(win, 'A' + rand() % 26);
            wattroff(win, COLOR_PAIR(randColorID));
            // wattroff(win, COLOR_PAIR(color));
        }
    }
    wrefresh(win);
}

void show_datetime(WINDOW *win, int y, int x){
    wmove(win, y, x);
    FILE *pp;
    cbreak();

    if ((pp = popen("/usr/bin/date | tr -s '\n' ' '", "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            waddstr(win, buffer);
        }
        wrefresh(win);
        pclose(pp);
    }
}


void execCMD(const char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        // char buffer[BUFSIZ];
        // while (fgets(buffer, sizeof(buffer), pp) != 0) {}
        pclose(pp);
    }
}

void execCMD(char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        // char buffer[BUFSIZ];
        // while (fgets(buffer, sizeof(buffer), pp) != 0) {}
        pclose(pp);
    }
}

void drawCMDStr(WINDOW *win, int y, int x, int show, int alignX, int is_cmd, int colorID, const char* msg){
    // wmove(win, y, x);
    FILE *pp;
    cbreak();
    // int strLen;
    /* if alignX==0 : left align
          alignX==1 : centre align
          alignX==2 : right align
    */

    if(is_cmd==0){
        if(show>0){
            if(alignX==0){wmove(win, y, x);}
            else if(alignX==1){
                wmove(win, y, x-(strlen(msg)/2));
            }
            else if(alignX==1){
                wmove(win, y, x-strlen(msg));
            }
            wattron(win, COLOR_PAIR(colorID));
            waddstr(win, msg);
            wattroff(win, COLOR_PAIR(colorID));
            wrefresh(win);
        }

    }
    else{

        if ((pp = popen(msg, "r")) != 0) {
            // if(show>0){
                char buffer[BUFSIZ];
                while (fgets(buffer, sizeof(buffer), pp) != 0) {
                    if(show>0){
                        if(alignX==1){
                            wmove(win, y, x-(strlen(buffer)/2));
                        }
                        else if(alignX==1){
                            wmove(win, y, x-strlen(buffer));
                        }
                        wattron(win, COLOR_PAIR(colorID));
                        waddstr(win, buffer);
                        wattroff(win, COLOR_PAIR(colorID));
                        wrefresh(win);
                    }
                }
                pclose(pp);
            //}
        }
    }
}

char* storeExecCMD(char* arr, const char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount]!='\0'){elementCount++;}
//            arr = fillArray(elementCount, arr);
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';
            for(int i=0; i<elementCount; i++){
                arr[i]=buffer[i];
            }
            waddstr(mainScreenWin, buffer);
            // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(arr)+2), 1, 0, 0, 13, arr);

        }
  //      else{arr=nullptr;}
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }

    //return arr;
}

char* storeExecCMD(char* arr, char* cmd){
    FILE *pp;
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            int elementCount=0;
            while(buffer[elementCount]!='\0'){elementCount++;}
            // arr = fillArray(elementCount, arr);
            arr = static_cast<char*>(std::malloc(elementCount * sizeof(char)));
            arr[0] = '\0';
            for(int i=0; i<elementCount; i++){
                arr[i]=buffer[i];
            }
            waddstr(mainScreenWin, buffer);
            // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(arr)+2), 1, 0, 0, 13, arr);
        }
//        else{arr=nullptr;}
        pclose(pp);
        return arr;
    }
    else{
        return nullptr;
    }

//    return arr;
}

void updateRequestedUSRENV(){
    FILE *pp;
    cbreak();
    char defaultEnv[] = "Default";
    char cmd[100] = "sudo cat /var/lib/AccountsService/users/";
    strcat(cmd, username);
    strcat(cmd, " 2>/dev/null | grep 'XSe*' | cut -d '=' -f 2");
    if ((pp = popen(cmd, "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            for(int i=5; i<strlen(buffer)+4; i++){titleBarItems[2][i]=buffer[i-5];};
            //int i=0;
            // while(buffer[i]!='\0'){titleBarItems[2][i+5]=buffer[i];i++;}
        }
        pclose(pp);
    }
}

void genProfilePicture(int h, int w, int y, int x){
    // Draw Random bitmap
    int randColorID=1;
    int totalAvailableColour = totalASCIIcolors+totalManualColors+totalRandomizedColors;

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


void messageBoxWindow(int h, int w, int y, int x, int is_cmd, const char* title, const char* msg){

   /* If is_cmd==1,  then the supplied char array, will be eecuted and the output will be printed in the message box
      else if is_cmd==0, then the message will directly be printed in the message box
   */

    FILE *pp;
    int ch;

    messageBoxBorderWindow = newwin(h, w, y, x);
    messageBox_msg = newwin(h-6, w-4, y+4, x+2);

    box(messageBoxBorderWindow, 0, 0);

    // Draw Message Box Title

    wattron(messageBoxBorderWindow, COLOR_PAIR(12));
    mvwprintw(messageBoxBorderWindow, 2, (w-(sizeof(title)/sizeof(title[0])))/2, title);
    wattroff(messageBoxBorderWindow, COLOR_PAIR(12));

    wrefresh(messageBoxBorderWindow);

    noecho();
    keypad(messageBox_msg, TRUE);

    wmove(messageBox_msg, 0, 0);

    if(is_cmd==1){

        // Print CMD Output
        char cmd[100] = "sudo python3 /etc/halkaDM/scripts/config.parser.py ";
        strcat(cmd, msg);

        if ((pp = popen(cmd, "r")) != 0) {
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
        if((ch == KEY_ESCAPE) || (ch == '\n') || (ch == 'q') || (ch == KEY_BACKSPACE) || (ch == 'w') || (ch == 'a') || (ch == 's') || (ch == 'd') || (ch == '4') || (ch == '8') || (ch == '2') || (ch == '6') || (ch == '5') || (ch == KEY_HOME) || (ch == KEY_EXIT)){ // If Enter is pressed

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
            // login_passField(win, (loginBoxMaxY/2), (loginBoxMaxX/4)+14);
        }
        else{}
    }while(1);
}


/*
void list_available_desktop_environments(WINDOW *win, int y, int x){
    wmove(win, y, x);
    FILE *pp;
    cbreak();

    int i=0;
    int j=0;

    if ((pp = popen("ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'", "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            for(int k=0;k<sizeof(buffer)/sizeof(buffer[0]);k++){
                if(buffer[k]=='\7'){i++;k++;j=0;}
                else if(buffer[k]=='\0'){break;}
                desktopEnvironmentsSubItems[i][j] = buffer[k];
                j++;
            }
        }
        pclose(pp);
    }
}
*/

void fill_available_desktop_environments(){
    FILE *pp;
    // Should be initiated only once during initial Memory allocation setup
    int rows=1, cols=0;
    int i=0, j=0;
    int k;

    if ((pp = popen("ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'", "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {

            for(k=0;k<sizeof(buffer)/sizeof(buffer[0]);k++){
                if(buffer[k]=='\7'){
                    rows++;k++;
                    if(j>cols){cols=j;}
                    j=0;
                }
                else if(buffer[k]=='\0'){
                    if(buffer[k-1]!='\7'){rows++;}
                    if(j>cols){cols=j;}
                    break;
                }
                j++;
            }

            desktopEnvironmentsSubItems = static_cast<char**>(std::malloc(rows * sizeof(char*)));
            for (int a = 0; a<rows; a++) {
                desktopEnvironmentsSubItems[a] = static_cast<char*>(std::malloc(cols * sizeof(char)));
                desktopEnvironmentsSubItems[a][0] = '\0';

                for(int b=0;b<cols;b++){

                    if(buffer[i]=='\7'||buffer[i]=='\0'){desktopEnvironmentsSubItems[a][b++]='\0';break;}
                    else{
                        desktopEnvironmentsSubItems[a][b] = buffer[i];
                    }
                    i++;
                }
                i++;
            }

        }
        pclose(pp);
    }
}

void subItemListWin(int maxY, int maxX, int minY, int minX, char*** charArray){
    // Generate New Window dynamically, on exit delete/free the window before breaking out from the function, to free up sys resource


    if ((maxY != 0) && (maxX != 0)){
        int ch='\0';
        maxY+=2;
        maxX+=2;
        noecho();

        do{
        subItemListWindow = newwin(maxY, maxX, minY, minX);
        keypad(subItemListWindow, TRUE);

        box(subItemListWindow, 0, 0);
            //int i=0;
            for(int i=0; i<(maxY-2); i++){
            //  while(charArray[0][i][0]!='\0'){
                if(i==titleBarItemTree[1]){wattron(subItemListWindow, COLOR_PAIR(13));}
                mvwprintw(subItemListWindow, i+1, 1, charArray[0][i]);
                if(i==titleBarItemTree[1]){wattroff(subItemListWindow, COLOR_PAIR(13));}
            }


            wrefresh(subItemListWindow);


                ch = wgetch(subItemListWindow);     /* refresh, accept single keystroke of input */
                if((ch == KEY_ESCAPE) || (ch==KEY_LEFT) || (ch=='a') || (ch == 'q') || (ch == KEY_HOME) || (ch == KEY_EXIT)){ // If Enter is pressed
                    freeArray(maxY-2, maxX-2, charArray);
                    wclear(subItemListWindow);
                    werase(subItemListWindow);
                    wrefresh(subItemListWindow);
                    break;
                }
                else if((ch == '\n') || (ch==KEY_RIGHT) || (ch == 'd') || (ch=='6') || (ch=='5')){
                    // Execute Message Window
//                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, charArray[0][titleBarItemTree[1]], subItemsCmd[titleBarItemTree[0]][titleBarItemTree[1]]);
                    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, charArray[0][titleBarItemTree[1]], charArray[1][titleBarItemTree[1]]);

                }
                else if((ch == '\t') || (ch==KEY_DOWN) || (ch == 's') || (ch=='2')){
                    wclear(subItemListWindow);
                    werase(subItemListWindow);

                    if(titleBarItemTree[1]>=(maxY-3)){titleBarItemTree[1]=0;}
                    else{titleBarItemTree[1]++;}
                }
                else if((ch == KEY_BACKSPACE) || (ch==KEY_UP) || (ch == 'w') || (ch=='8')){
                    wclear(subItemListWindow);
                    werase(subItemListWindow);

                    if(titleBarItemTree[1]<=0){titleBarItemTree[1]=(maxY-3);}
                    else{titleBarItemTree[1]--;}

                }
                else{}
            }while(1);
    }

}

// void draw_titlebar(WINDO *titlebar=titleBar_subwin, int itemID=-1);

int draw_titlebar(WINDOW *titlebar, int itemID=-1)
{

    int ch, titlebarCoordX, titlebarCoordY;
    int positionCoordX;
    int spacingX = 3;
    int titleBarItemCount = sizeof(titleBarItems)/sizeof(titleBarItems[0]);
    noecho();
    titleBarItemTree[0]=itemID;
    do{
        keypad(titlebar, TRUE);
        getmaxyx(titlebar, titlebarCoordY, titlebarCoordX);
        positionCoordX=0;

        if(titleBarItemTree[0]==0){wattron(titlebar, COLOR_PAIR(13));}
        wmove(titlebar, 1, positionCoordX+spacingX);
        waddstr(titlebar,titleBarItems[0]);
        if(titleBarItemTree[0]==0){wattroff(titlebar, COLOR_PAIR(13));}

        positionCoordX+=strlen(titleBarItems[0]);

        if(titleBarItemTree[0]==1){wattron(titlebar, COLOR_PAIR(13));}
        wmove(titlebar, 1, positionCoordX+(spacingX)*2);
        waddstr(titlebar, titleBarItems[1]);
        if(titleBarItemTree[0]==1){wattroff(titlebar, COLOR_PAIR(13));}

        if(titleBarItemTree[0]==2){wattron(titlebar, COLOR_PAIR(13));}
        wmove(titlebar, 1, titlebarCoordX-(strlen(titleBarItems[2])+spacingX));
        waddstr(titlebar, titleBarItems[2]);
        if(titleBarItemTree[0]==2){wattroff(titlebar, COLOR_PAIR(13));}


        wmove(titlebar, 1, titlebarCoordX/2);
        show_datetime(titlebar, 1, (titlebarCoordX/2)-16); // Show DateTime

        wrefresh(titlebar);

        ///////////////////////////

        if(titleBarItemTree[0]==-1){break;}

        ch = wgetch(titlebar);
        if((ch=='w') || (ch=='q') || (ch==KEY_UP) || (ch==KEY_ESCAPE) || (ch=='8')){
            titleBarItemTree[0]=-1;
        }
        else if((ch=='\t') || (ch==KEY_RIGHT) || (ch==' ') || (ch=='d') || (ch=='6')){
                if(titleBarItemTree[0]>=(titleBarItemCount-1)){titleBarItemTree[0]=0;}
                else{titleBarItemTree[0]++;}
        }
        else if((ch==KEY_BACKSPACE) || (ch==KEY_LEFT) || (ch=='a') || (ch=='4')){
                if(titleBarItemTree[0]<=0){titleBarItemTree[0]=titleBarItemCount-1;}
                else{titleBarItemTree[0]--;}
        }
        else if((ch=='\n') || (ch==KEY_DOWN) || (ch=='s') || (ch=='5') || (ch=='2')){
            if(titleBarItemTree[0]==0){
                titleBarItemTree[1]=0;

                int r =  sizeof(powerSubItems)/sizeof(powerSubItems[0]);
                int rc = sizeof(powerSubItems[0])/sizeof(powerSubItems[0][0]);
                int c = sizeof(powerSubItems[0][0])/sizeof(powerSubItems[0][0][0]);
                titleBarSubItems = fillArray(r, rc, c, titleBarSubItems);

                for(int i=0; i<r; i++){
                    for(int j = 0; j < rc; j++){
                        for(int k = 0; k < c; k++) {
                            titleBarSubItems[i][j][k] = powerSubItems[i][j][k];
                        }
                    }
                }
                subItemListWin(rc, c, titlebarCoordY, spacingX, titleBarSubItems);


            }
            else if(titleBarItemTree[0]==1){
                titleBarItemTree[1]=0;

                int r =  sizeof(utilitiesSubItems)/sizeof(utilitiesSubItems[0]);
                int rc = sizeof(utilitiesSubItems[0])/sizeof(utilitiesSubItems[0][0]);
                int c = sizeof(utilitiesSubItems[0][0])/sizeof(utilitiesSubItems[0][0][0]);
                titleBarSubItems = fillArray(r, rc, c, titleBarSubItems);

                for(int i=0; i<r; i++){
                    for(int j = 0; j < rc; j++){
                        for(int k = 0; k < c; k++) {
                            titleBarSubItems[i][j][k] = utilitiesSubItems[i][j][k];
                        }
                    }
                }

                subItemListWin(rc, c, titlebarCoordY, spacingX, titleBarSubItems);

            }
            else if(titleBarItemTree[0]==2){
                titleBarItemTree[1]=0;
                //int rows = sizeof(desktopEnvironmentsSubItems)/sizeof(desktopEnvironmentsSubItems[0]);
                int cols = sizeof(desktopEnvironmentsSubItems[0])/sizeof(desktopEnvironmentsSubItems[0][0]);
                //int rows = distance(desktopEnvironmentsSubItems, desktopEnvironmentsSubItems + sizeof(desktopEnvironmentsSubItems) / sizeof(desktopEnvironmentsSubItems[0]));

                int rows = 0;

                for (int i = 0; desktopEnvironmentsSubItems[i][0] != '\0'; i++) {
                    rows++;
                }

                /*char** arr = static_cast<char**>(std::malloc(rows * sizeof(char*)));

                for (int i = 0; i < rows; i++) {
                    arr[i] = static_cast<char*>(std::malloc(cols * sizeof(char)));
                    for (int j = 0; j < cols; j++) {
                        arr[i][j] = desktopEnvironmentsSubItems[i][j];
                    }
                }*/

                 // subItemListWin(rows, cols, titlebarCoordY, spacingX, desktopEnvironmentsSubItems);
            }
        }

    }while(1);

    return 0;

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

void authSuccess(){
    // On Auth Success Create Session File and source file for sourcing
    // Create New Session Key
    char cmd[250] = "getent passwd ";
    strcat(cmd, username);
    strcat(cmd, " | grep -v '/nologin' | cut -d: -f6 | tr -s '\n' '/'");
    usrHomeDir = storeExecCMD(usrHomeDir, cmd);

    for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}
    // Check if any home directory exist for the user & if it is valid
    if(usrHomeDir==nullptr){
        //drawCMDStr(mainScreenWin, 20, 0, 1, 0, 0, 13, "NULL");
        messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 0, "Login Failed", "Incorrect Credentials");
    }
    else{
        // drawCMDStr(mainScreenWin, 15, 0, 1, 0, 0, 13, usrHomeDir);
    //}


    // std::replace(usrHomeDir, usrHomeDir + strlen(usrHomeDir), '\7', '/');

        createSessionKey(SESSION_KEY_LENGTH-1, SESSION_KEY);


    //memset(cmd, '\0', sizeof(cmd)); // Clear cmd char* array
    //for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}
    // Create Session Directory
        char envSource[] = "env_source";
    // char cmd[200] = "mkdir -p ";
        strcpy(cmd, "mkdir -p ");
        strcat(cmd, usrHomeDir);
        strcat(cmd, ".halkaDM/");
        strcat(cmd, SESSION_KEY);
        strcat(cmd, " && echo 'NEW_USER=");
        strcat(cmd, username);
        strcat(cmd, "&&SESSION=");
        strcat(cmd, currentDesktopENV);
        strcat(cmd, "' > ");
        strcat(cmd, usrHomeDir);
        strcat(cmd, ".halkaDM/");
        strcat(cmd, SESSION_KEY);
        strcat(cmd, "/");
        strcat(cmd, envSource);
    // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(usrHomeDir)+2), 1, 0, 0, 13, cmd);
        execCMD(cmd);
    }
}

int authenticateButton(){

    wattron(authBox, COLOR_PAIR(13));
    mvwprintw(authBox, loginBoxMaxY-1, loginBoxMaxX-10, "LOGIN");
    wattron(authBox, COLOR_PAIR(13));

    wrefresh(authBox);

    int retCode;

    int ch;


    do{
        ch = wgetch(authBox);
        if((ch=='w') || (ch=='q') || (ch==KEY_UP) || (ch==KEY_ESCAPE) || (ch=='8')){retCode = 1;break;}
        else if((ch=='\t') || (ch==KEY_DOWN) || (ch==' ') || (ch=='s') || (ch=='2')){retCode = 3;break;}
        else if((ch==KEY_BACKSPACE) || (ch==KEY_LEFT) || (ch=='a') || (ch=='4')){retCode = 0;break;}
        else if((ch=='\n') || (ch==KEY_RIGHT) || (ch=='d') || (ch=='5') || (ch=='6')){authSuccess();retCode = 0;break;}
   // else{}
    }while(1);

    wattron(authBox, COLOR_PAIR(9));
    mvwprintw(authBox, loginBoxMaxY-1, loginBoxMaxX-10, "LOGIN");
    wattron(authBox, COLOR_PAIR(9));

    wrefresh(authBox);

    // Check if username is valid

    // Check if Password is valid for the mentioned username


    // If username and pass is valid then create session with authSuccess()
    // Else Notify that the credentials are not valid
    return retCode;
}

int login_passField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int userpassChrCount = 0;
    int userpassLengthMax = sizeof(userpass)/sizeof(userpass[0]);
    int ch;
    // char* visible_userpass;
    // char visible_userpass[50]={'\0'};
    // int finish=0;
    // cbreak();
    noecho();
    keypad(win, TRUE);

    do{
         wmove(win, y, x);

        if(userpassVisibilityConf[0]!=2 && strlen(userpass)>0){
            // visible_userpass = mask_authInput(0, userpassVisibilityConf, userpass);
            // maskStr2(visible_userpass, strlen(userpass), '*');
            halkadm_security.mask_authInput(0, userpassVisibilityConf, userpass, visible_userpass);
            wprintw(win, visible_userpass);
            userpassChrCount=strlen(userpass);
        }
        wrefresh(win);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if ((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            // authSuccess();
            break;
        }
        else if((userpassChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((userpassChrCount>=0) && (userpassChrCount<userpassLengthMax)) || ((userpassChrCount == userpassLengthMax) && (ch == KEY_BACKSPACE))){
            gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);

            if(ch == KEY_BACKSPACE){ // If backspace is pressed
                userpassChrCount-=1;
                userpass[userpassChrCount] = '\0';
                mvwprintw(win, y, x+userpassChrCount, " ");
            }
            else{
                userpass[userpassChrCount] = ch;
                userpassChrCount++;
            }
        }
    }while (1);

    return 2;

}

int login_userField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int usernameChrCount = 0;
    int usernameLengthMax = sizeof(username)/sizeof(username[0]);
    int ch;
    // char visible_username[50]={'\0'};
    noecho();
    keypad(win, TRUE);

    do{
        wmove(win, y, x);

        if(usernameVisibilityConf[0]!=2 && strlen(username)>0){
            halkadm_security.mask_authInput(0, usernameVisibilityConf, username, visible_username);
            wprintw(win, visible_username);
            usernameChrCount=strlen(username);
        }

        wrefresh(win);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            updateRequestedUSRENV();
            draw_titlebar(titleBar_subwin, -1);
            break;
        }
        else if((usernameChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((usernameChrCount>=0) && (usernameChrCount<usernameLengthMax)) || ((usernameChrCount == usernameLengthMax) && (ch == KEY_BACKSPACE))){
            gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);

            if(ch == KEY_BACKSPACE){ // If backspace is pressed
                usernameChrCount-=1;
                username[usernameChrCount] = '\0';
                // wmove(win, y, x+usernameChrCount);
                // waddch(win, ' ');
                mvwprintw(win, y, x+usernameChrCount, " ");
            }
            else{
                username[usernameChrCount] = ch;
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
    gen_randColorMap(authBox, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);

    // Auth Section //

    // Username Field
    mvwprintw(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX/4)+5, "USER : ");

    // Userpass Field
    mvwprintw(authBox, (loginBoxMaxY/2), (loginBoxMaxX/4)+5, "PASS : ");

    // Login Button
    mvwprintw(authBox, loginBoxMaxY-1, loginBoxMaxX-10, "LOGIN");


    wrefresh(authBox);
}


/*void createSessionKey() {
    std::random_device rd;
    for (int i = 0; i < sizeof(SESSION_KEY); ++i) {
        SESSION_KEY[i] = rd();
    }
    // create a hash object
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    // specify the hash function
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    // calculate the hash
    EVP_DigestUpdate(mdctx, SESSION_KEY, sizeof(SESSION_KEY));
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    // print the hash
    //cout << "Hash: ";
    //for(int i = 0; i < md_len; i++) cout << hex << (int)md_value[i];
    //cout << endl;
}*/

void freeMemory(){

    // This function must be called before exiting the program to clear the system allocated memory space

    // Free User SESSION_KEY allocated space
    free(SESSION_KEY);

    // Free Username Space
    free(username);
    free(visible_userpass);
    // Free Userpass Space
    free(userpass);
    free(visible_userpass);

    // TitleBar Item Tree Depth | Free Allocated Storage Space
    free(titleBarItemTree);

    // De-allocate space for config list for loginColourMatrix
    free(loginColourMatrixConf);

    // Free Allocated Space of The Environment Names in 2d char Array
    for (int i = 0; i < sizeof(desktopEnvironmentsSubItems)/sizeof(desktopEnvironmentsSubItems[0]); i++) {
        free(desktopEnvironmentsSubItems[i]);
    }
    free(desktopEnvironmentsSubItems);

}

/*void calculate_hash(AutoSeeded_RNG &rng) {
    rng.randomize(SESSION_KEY, SESSION_KEY_LENGTH);
    string hash = hash_string(SESSION_KEY, SESSION_KEY_LENGTH, "SHA-256");
    // cout << "Hash: " << hash << endl;
}*/

/*void createSessionKey(int len, char* arr){
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        // arr[i] = '!'+(rand() % 94);
        arr[i] = 'a'+(rand() % 25);
    }
    //unsigned char result[MD5_DIGEST_LENGTH];
    //MD5((unsigned char*)arr, strlen(arr), result);

    //for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    //    sprintf(&arr[i*2], "%02x", (unsigned int)result[i]);
    //}
}*/

void allocateMemory(){

    // Create SESSION_KEY | SIZE 33
    SESSION_KEY = static_cast<char*>(std::malloc(SESSION_KEY_LENGTH * sizeof(char)));
    // createSessionKey(SESSION_KEY_LENGTH-1, SESSION_KEY);

    // createSessionKey();
    // AutoSeeded_RNG rng; // cryptographically secure random number generator
    // calculate_hash(rng);


    // Allocate Username Space
    username = static_cast<char*>(std::malloc(maxUsernameLen * sizeof(char)));
    visible_username = static_cast<char*>(std::malloc(visibleAuthStrLen * sizeof(char)));
    username[0] = '\0';
    visible_username[0] = '\0';

    // Allocate Userpass Space
    userpass = static_cast<char*>(std::malloc(maxUserpassLen * sizeof(char)));
    visible_userpass = static_cast<char*>(std::malloc(visibleAuthStrLen * sizeof(char)));
    userpass[0] = '\0';
    visible_userpass[0] = '\0';

    // TitleBar Item Tree Depth | Storage Space Allocation
    titleBarItemTree = static_cast<int*>(std::malloc(maxTitleBarItemTreeDepth * sizeof(int)));

    // Allocate space for config list for loginColourMatrix
    loginColourMatrixConf = static_cast<int*>(std::malloc(4 * sizeof(int)));

    // Allocate & Fill Space For Environment Names
    fill_available_desktop_environments();
}


void initWindow(){
    // Allocate Memory Space

    allocateMemory();

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


    // Setup Colours
    // start_color();
    initColor();

    drawCMDStr(mainScreenWin, winMaxY-2, winMaxX-(strlen(package)+2), 1, 0, 0, 13, package);
    // drawCMDStr(mainScreenWin, winMaxY-3, winMaxX-(strlen(SESSION_KEY)+2), 1, 0, 0, 13, SESSION_KEY);
    drawCMDStr(mainScreenWin, winMaxY/2, winMaxX/2, 1, 1, 1, 13, "cat /etc/os-release | grep -w -E 'NAME=|VERSION=' | cut -d '=' -f 2 | cut -d '\"' -f 2 | tr -s '\n' ' '");
    drawCMDStr(mainScreenWin, (winMaxY/2)+1, winMaxX/2, 1, 1, 1, 13, "uname -n -o");

    refresh();

    box(titleBar_subwin, 0, 0);
    box(mainScreenWin, 0, 0);

    draw_titlebar(titleBar_subwin, -1);

    // List Available Desktop Managers
//     list_available_desktop_environments(mainScreenWin, winMaxY/2, winMaxX/2);

    //// UserName Visibility colorMap
    authChrVisibilityPattern(authBox, (loginBoxMaxY/2)-1, loginBoxMaxX-10, usernameVisibilityConf);

    //// UserPass Visibility colorMap
    authChrVisibilityPattern(authBox, (loginBoxMaxY/2), loginBoxMaxX-10, userpassVisibilityConf);


    // Set Login Matrix Config
    setLoginMatrixWindow(authBox); // The LoginMatrix to be shown in the provided window
    loginMatrixSetConfig(1, 1, loginBoxMaxY-1, (loginBoxMaxX/4)-2);


    // Draw Auth Box
    drawAuthBox(winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);

    // refreshing the window
    wrefresh(mainScreenWin);
    wrefresh(titleBar_subwin);
    wrefresh(accountPicBox);

}

int main(int argc, char **argv)
{
    initscr();
    start_color();
    initWindow();
    int id=0;
    do{
        genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);

        if(id==0){
            id = login_userField(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX/4)+14);
        }
        else if(id==1){
            id = login_passField(authBox, (loginBoxMaxY/2), (loginBoxMaxX/4)+14);
        }
        else if(id==2){
            //authSuccess();
            id = authenticateButton();
        }
        else if(id==3){
            id = draw_titlebar(titleBar_subwin, 0);
        }
    }while(1);

    getch();
    endwin();
    freeMemory();
    return 0;
}
