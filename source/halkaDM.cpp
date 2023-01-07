#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
/*#include <stdlib.h>
#include <stdio.h>*/

using namespace std;

char KEY_ESCAPE = '\x1b';

int asciiColors[] = {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_WHITE,
    COLOR_MAGENTA,
    COLOR_BLUE,
    COLOR_CYAN
};

/*

// COLORS MAPOUT //

0: [1-8] : size:8 | Pre Setup Colours
1: [COLOR_BLACK, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_WHITE, COLOR_MAGENTA, COLOR_BLUE, COLOR_CYAN] : size:8 | Exclude the following for directly calling them in any case
2: [username visibility colourmap] : size:3
3: [userpass visibility colourmap] : size:3
4: [Profile Pic] : size:(Relative)maxX*maxY | 1D-array
5: [Login Randomization colourMap] : size:(Relative)maxX*maxY | 1D-array


*/

/*// Default starting static Colour index
init_pair(1, COLOR_WHITE, COLOR_BLACK);
init_pair(2, COLOR_WHITE. COLOR_RED);
init_pair(3, COLOR_BLACK, COLOR_YELLOW);
init_pair(4, COLOR_WHITE, COLOR_GREEN);
init_pair(5, COLOR_BLACK, COLOR_WHITE);
init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
init_pair(7, COLOR_WHITE, COLOR_BLUE);
init_pair(8, COLOR_BLACK, COLOR_CYAN);*/

int currentTitleID = 0;

char powerSubItems[][10] = {
    "Sleep",
    "Restart",
    "Shutdown"
};

char utilitiesSubItems[][15] = {
    "Calender",
    "CPU Status",
    "Network Status"
};

char titleBarItems[][16] = {
    "POWER",
    "Utilities",
    "Environment"

};

/*
// TitleBar Item ID List

1. Power
2. Utilities
3. <DateTime>
4. Desktop Manager

*/

int availableDesktopManagerCount;

char desktopManagerSubItems[1][20];

WINDOW *authBox, *accountPicBox, *titleBar_subwin; // User Account Picture Box

int winMaxX, winMaxY;
int accountPicBoxMaxX, accountPicBoxMaxY, accountPicBoxMaxW, accountPicBoxMaxH;
int msgBoxMaxX, msgBoxMaxY, msgBoxMaxW, msgBoxMaxH;
int loginBoxMaxX, loginBoxMaxY;

char username[32];
char userpass[255]; // no password limit

char masked_username[32];
char masked_userpass[50];


int usernameVisibilityConf[3] = {0, 0, 0}; // {[0..2], [0..1], [0..1]}Default username_visibility_config
int userpassVisibilityConf[3] = {0, 0, 0}; // Default userpass_visibility_config

WINDOW *loginColourMatrixWin;
int loginColourMatrixConf[] = {}; // {COORDINATE-Y, COORDINATE-X, HEIGHT, WIDTH}


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

void gen_randColorMap(WINDOW *win, int y, int x, int h, int w){
    // Draw Random bitmap
    int randColorID=1;
    // wmove(authBox, randPos, 1);
    // wmove(win, y, x);
    int colors[] = {
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_WHITE
    };

    srand(time(NULL));
    for (int i = y; i < h; i++) {
        wmove(win, i, x);
        for (int j = 0; j < w; j++) {
            int fg = asciiColors[rand() % 8] + rand()%7;
            int bg = asciiColors[rand() % 8] * rand()%15;
            init_pair(randColorID, fg, bg);
            // wattron(win, COLOR_PAIR(color));
            wattron(win, COLOR_PAIR(randColorID));
            waddch(win, 'A' + rand() % 26);
            wattroff(win, COLOR_PAIR(randColorID));
            // wattroff(win, COLOR_PAIR(color));
            randColorID++;
        }
    }
}

void draw_color_pixel(WINDOW *win, int y, int x, int h, int w, int fg, int bg){
    // int randColorID = 0;
    init_pair(1, fg+rand()%7, bg+rand()%15);
    for (int i = y; i < h; i++) {
        wmove(win, i, x);
        for (int j = 0; j < w; j++) {
            wattron(win, COLOR_PAIR(1));
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(1));
            //randColorID++;
        }
    }
}

void show_datetime(WINDOW *win, int y, int x){
    wmove(win, y, x);
    FILE *pp;
    cbreak();

    if ((pp = popen("date | tr -s '\n' ' '", "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            waddstr(win, buffer);
        }
        wrefresh(win);
        pclose(pp);
    }
}

int getStrLength(const char *str){
    int strLength = sizeof(str)/sizeof(str[0]);
    return strLength;
}

static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}

void messageBoxWindow(int h, int w, int y, int x, int is_cmd, const char* title, const char* msg){

   /* If is_cmd==1,  then the supplied char array, will be eecuted and the output will be printed in the message box
      else if is_cmd==0, then the message will directly be printed in the message box
   */

    WINDOW *messageBoxBorderWindow, *messageBox_msg;
    FILE *pp;
    int ch;

    messageBoxBorderWindow = newwin(h, w, y, x);
    messageBox_msg = newwin(h-6, w-4, y+4, x+2);

    box(messageBoxBorderWindow, 0, 0);
//   box(messageBox_msg, 0, 0);

    // Draw Message Box Title

    wattron(messageBoxBorderWindow, COLOR_PAIR(rand()%8));
    mvwprintw(messageBoxBorderWindow, 2, (w-(sizeof(title)/sizeof(title[0])))/2, title);
    wattroff(messageBoxBorderWindow, COLOR_PAIR(rand()%8));

    wrefresh(messageBoxBorderWindow);

    noecho();
    keypad(messageBox_msg, TRUE);

    wmove(messageBox_msg, 0, 0);

    if(is_cmd==1){

        if ((pp = popen(msg, "r")) != 0) {
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
        if((ch == KEY_ESCAPE) || (ch == '\n') || (ch == 'q') || (ch == KEY_BACKSPACE) || (ch == KEY_HOME) || (ch == KEY_EXIT)){ // If Enter is pressed
            delwin(messageBox_msg);
            delwin(messageBoxBorderWindow);
            break;
        }
        else if(ch == '\t'){
            // login_passField(win, (loginBoxMaxY/2), (loginBoxMaxX/4)+14);
        }
        else{}
    }while(1);
}

void list_available_desktop_environments(WINDOW *win, int y, int x){
    wmove(win, y, x);
    FILE *pp;
    cbreak();

    if ((pp = popen("ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev", "r")) != 0) {
        char buffer[BUFSIZ];
        while (fgets(buffer, sizeof(buffer), pp) != 0) {
            waddstr(win, buffer);
        }
        pclose(pp);
    }
}



void draw_titlebar(WINDOW *titlebar, int itemID=-1)
{

    int ch, titlebarCoordX, titlebarCoordY;
    int positionCoordX;
    int spacingX = 3;
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    do{
        getmaxyx(titlebar, titlebarCoordY, titlebarCoordX);
        positionCoordX=0;

        if(itemID==0){wattron(titlebar, COLOR_PAIR(5));}
        wmove(titlebar, 1, positionCoordX+spacingX);
        waddstr(titlebar,titleBarItems[0]);
        if(itemID==0){wattroff(titlebar, COLOR_PAIR(5));}

        positionCoordX+=getStrLength(titleBarItems[0]);

        if(itemID==1){wattron(titlebar, COLOR_PAIR(5));}
        wmove(titlebar, 1, positionCoordX+spacingX);
        waddstr(titlebar, titleBarItems[1]);
        if(itemID==1){wattroff(titlebar, COLOR_PAIR(5));}

        if(itemID==2){wattron(titlebar, COLOR_PAIR(5));}
        wmove(titlebar, 1, titlebarCoordX-((sizeof(titleBarItems[2])/sizeof(titleBarItems[2][0]))+spacingX));
        waddstr(titlebar, titleBarItems[2]);
        if(itemID==2){wattroff(titlebar, COLOR_PAIR(5));}


        wmove(titlebar, 1, titlebarCoordX/2);
        show_datetime(titlebar, 1, (titlebarCoordX/2)-16); // Show DateTime

        wrefresh(titlebar);

        ///////////////////////////

        if(itemID==-1){break;}
        ch = wgetch(titlebar);
        if(itemID==0){
            if(ch=='\t'){
                itemID=1;
            }
        }
        else if(itemID==1){
            if(ch=='\t'){
                itemID=2;
            }
        }
        else if(itemID==2){
            if(ch=='\t'){
                itemID=-1;
            }
        }


    }while(1);
}




char* maskStr(int strLen, char character='*');

char* maskStr(int strLen, char character){
    static char str[50];
    // memset(suppliedArray, character, sizeof(suppliedArray));
    for(int i=0;i<strLen;i++){str[i]=character;}
    return str;
}

char* generateRandomStr(int randomizeLen=0, int maxStrLen=10, const char *chrType="a");

char* generateRandomStr(int randomizeLen, int maxStrLen, const char *chrType){

    int strLen=maxStrLen;

    if(randomizeLen!=0){
        srand(time(NULL));
        strLen = rand()%maxStrLen;
    }
    /*else{
        strLen = maxStrLen;
    }*/


    //////////////////////////
    static char str[50];
    //char str[strLen];
    char charT;

    int chrTypeLen = sizeof(chrType)/sizeof(chrType[0]);

    for(int i=0; i<strLen; i++){

        srand(time(NULL));
        if(chrTypeLen>1){charT = chrType[rand()%chrTypeLen];}
        else{charT = chrType[0];}



        if(((int)charT>='a') && ((int)charT<='z')){
        // Lower Case Randomization
            str[i] = 'a'+rand()%26;
        }
        else if(((int)charT>='A') && ((int)charT<='Z')){
        // Upper Case Randomization
            str[i] = 'A'+rand()%26;
        }
        else if(((int)charT>='0') && ((int)charT<='9')){
        // Integer Randomization
            str[i] = '0'+rand()%9;
        }
        else if(
                (((int)charT>='!') && ((int)charT<='/')) || 
                (((int)charT>=':') && ((int)charT<='@')) || 
                (((int)charT>='[') && ((int)charT<='`')) || 
                (((int)charT>='{') && ((int)charT<=254))
        ){
        // Special Charaacter+symbol Randomization
            int specialCharType=rand()%3;

            if(specialCharType==0){
                str[i] = '!'+rand()%14;
            }
            else if(specialCharType==1){
                str[i] = ':'+rand()%6;
            }
            else if(specialCharType==2){
                str[i] = '['+rand()%5;
            }
            else if(specialCharType==3){
                str[i] = '{'+rand()%164;
            }
        }
        else{
            str[i] = ' ';
        }
    }

    return str;
}


char *mask_authInput(int authType=0, int *maskingConfig=userpassVisibilityConf, const char* str="PLACEHOLDER");

char *mask_authInput(int authType, int *maskingConfig, const char* str){
    /* maskField: "username" | For masking the username for visibility
       maskField: "userpass" | For masking the userpass for visibility*/
    char *maskedOutput; // const char* -> char*
    int maskingConfigLength, strLen;

    strLen = sizeof(str)/sizeof(str[0]);

    // Mask According to masking Config

    if(maskingConfig[0]>=2){strcpy(maskedOutput, " ");}
    else if(maskingConfig[2]==0){
        if(maskingConfig[1]==0){
            if(maskingConfig[0]==0){
                strcpy(maskedOutput, str);
                // maskedOutput = str;
            }
            else if(maskingConfig[0]==1){
                maskedOutput = maskStr(strLen, '*');
            }
        }
        else if(maskingConfig[1]==1){
            if(maskingConfig[0]==0){
                maskedOutput = generateRandomStr(0, strLen, "Aa0");
            }
            else if(maskingConfig[0]==1){
                maskedOutput = generateRandomStr(0, strLen, "!");
            }
        }
    }
    else if(maskingConfig[2]==1){
        srand(time(NULL));
        // strLen = rand()%(strLen+5);

        if(maskingConfig[1]==0){
            if(maskingConfig[0]==0){
                maskedOutput = generateRandomStr(1, strLen, "Aa0");
            }
            else if(maskingConfig[0]==1){
                maskedOutput = maskStr(strLen, '*'); // need review
            }
        }
        else if(maskingConfig[1]==1){
            if(maskingConfig[0]==0){
                maskedOutput = generateRandomStr(1, strLen, "Aa0");
            }
            else if(maskingConfig[0]==1){
                maskedOutput = generateRandomStr(1, strLen, "!");
            }
        }
    }
    else{
         maskedOutput = maskStr(strLen, '*');
    }

    return maskedOutput;
}



void user_pass_visibility(WINDOW *win, int y, int x){

    /*
     ------------------------------------------------------------------------------------------------------------------

     0 : COLOR_BLACK -> NONE | Common

     ----------------------------------
     1: COLOR_RED -> show chr
     2: COLOR_YELLOW -> mask chr | with *
     3: COLOR_GREEN -> hide chr
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

    for(int i=0; i<sizeof(userpassVisibilityConf)/sizeof(userpassVisibilityConf[0]); i++){
        wattron(win, COLOR_PAIR(asciiColors[userpassVisibilityConf[i]]));
        waddch(win, userpassVisibilityConf[i]);
        wattroff(win, COLOR_PAIR(asciiColors[userpassVisibilityConf[i]]));

    }

}

void genProfilePicture(int h, int w, int y, int x){
    // Draw Random bitmap
    int randColorID=1;
    // wmove(win, y, x);

    srand(time(NULL));
    for (int i = y; i < h; i++) {
        wmove(accountPicBox, i, x);
        for (int j = 0; j < w; j++) {
            int fg = asciiColors[rand() % 8] + rand()%7;
            int bg = asciiColors[rand() % 8] * rand()%15;
            init_pair(randColorID, fg, bg);
            // wattron(win, COLOR_PAIR(color));
            wattron(accountPicBox, COLOR_PAIR(randColorID));
            waddch(accountPicBox, 'A' + rand() % 26);
            wattroff(accountPicBox, COLOR_PAIR(randColorID));
            // wattroff(win, COLOR_PAIR(color));
            randColorID++;
        }
    }
    wrefresh(accountPicBox);
}

void login_passField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int userpassChrCount = 0;
    int userpassLengthMax = sizeof(userpass)/sizeof(userpass[0]);
    int ch;
    // int finish=0;
    // cbreak();
    noecho();
    keypad(win, TRUE);

    do{
        // wmove(win, y, x);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if ((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            // genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            //finish=1;
            break;
        }
        else if((userpassChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((userpassChrCount>=0) && (userpassChrCount<userpassLengthMax)) || ((userpassChrCount == userpassLengthMax) && (ch == KEY_BACKSPACE))){
            gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);
            // genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);

          if(ch == KEY_BACKSPACE){ // If backspace is pressed
                userpassChrCount--;
                userpass[userpassChrCount] = '\0';
                // wmove(win, y, x+userpassChrCount);
                // waddch(win, ' ');
                mvwprintw(win, y, x+userpassChrCount, " ");
            }
            else{
                userpass[userpassChrCount] = ch;
                userpassChrCount++;
            }
            // wmove(win, y, x);

            /*for(int i=0; i<userpassChrCount; i++){
                waddch(win, userpass[i]);
            }*/
            char* visible_userpass = mask_authInput(0, userpassVisibilityConf, userpass);
            mvwprintw(win, y, x, visible_userpass);

            wrefresh(win);
            // show_datetime(titlebar, 1, (titlebarCoordX/2)-16);
            // free(visible_userpass);
            /* process the command keystroke */
        }
    }while (1);
}

void login_userField(WINDOW *win, int y, int x){
    wmove(win, y, x);
    int usernameChrCount = 0;
    int usernameLengthMax = sizeof(username)/sizeof(username[0]);
    int ch;
    // cbreak();
    noecho();
    keypad(win, TRUE);

    do{
        // wmove(win, y, x);
        ch = wgetch(win);     /* refresh, accept single keystroke of input */
        if((ch == '\n') || (ch == '\t')){ // If Enter is pressed
            // genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            break;
        }
        else if((usernameChrCount==0) && (ch == KEY_BACKSPACE)){}
        else if(((usernameChrCount>=0) && (usernameChrCount<usernameLengthMax)) || ((usernameChrCount == usernameLengthMax) && (ch == KEY_BACKSPACE))){
            gen_randColorMap(loginColourMatrixWin, loginColourMatrixConf[0], loginColourMatrixConf[1], loginColourMatrixConf[2], loginColourMatrixConf[3]);
            // genProfilePicture(accountPicBoxMaxH-4, accountPicBoxMaxW-4, accountPicBoxMaxY+2, accountPicBoxMaxX+2);
            // genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
            if(ch == KEY_BACKSPACE){ // If backspace is pressed
                usernameChrCount--;
                username[usernameChrCount] = '\0';
                // wmove(win, y, x+usernameChrCount);
                // waddch(win, ' ');
                mvwprintw(win, y, x+usernameChrCount, " ");
            }
            else{
                username[usernameChrCount] = ch;
                usernameChrCount++;
            }
            // wmove(win, y, x);

            char* visible_username = mask_authInput(0, usernameVisibilityConf, username);
            mvwprintw(win, y, x, visible_username);

            wrefresh(win);
        }

        /* process the command keystroke */
    }while (1);
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



int main(int argc, char **argv)
{
    int bg, fg;
    /*int winMaxX, winMaxY;
    int accountPicBoxMaxX, accountPicBoxMaxY, accountPicBoxMaxW, accountPicBoxMaxH;
    int msgBoxMaxX, msgBoxMaxY, msgBoxMaxW, msgBoxMaxH;
    int loginBoxMaxX, loginBoxMaxY;*/
    initscr();


    // Default starting static Colour index
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_BLACK, COLOR_CYAN);


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

    char msgBoxTitle[msgBoxMaxW-4];

    // creating a window;
    // with height = 15 and width = 10
    // also with start x axis 10 and start y axis = 20
    // WINDOW *win = newwin(15, 17, 2, 10);
    WINDOW *win, *messageBox, *messageBox_msg;
    win = newwin(winMaxY, winMaxX, 0, 0);
    accountPicBox = subwin(win, accountPicBoxMaxH, accountPicBoxMaxW, accountPicBoxMaxY, accountPicBoxMaxX); // Account Picture Box
    // messageBox = subwin(win, msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX);
    messageBox = newwin(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX);

    // getmaxyx(messageBox, msgBoxMaxY, msgBoxMaxX); // test 

    // messageBox_msg = subwin(messageBox, msgBoxMaxY-2, msgBoxMaxX-4, 1, 2);
    // messageBox_msg = subwin(win, msgBoxMaxH-6, msgBoxMaxW-4, msgBoxMaxY+4, msgBoxMaxX+2);
    messageBox_msg = newwin(msgBoxMaxH-6, msgBoxMaxW-4, msgBoxMaxY+4, msgBoxMaxX+2);
    authBox = subwin(win, winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);
    titleBar_subwin = subwin(win, 3, winMaxX,0,0);

    getmaxyx(authBox, loginBoxMaxY, loginBoxMaxX);
    //authBox = newwin(winMaxY*0.75, winMaxX/2,0,0);
    refresh();

    // making box border with default border styles
    // box(win, 0, 0);
    // box(authBox, 0, 0);
    box(titleBar_subwin, 0, 0);
    box(accountPicBox, 0, 0);
    // box(messageBox, 0, 0);
    // box(messageBox_msg, 0, -2);
    box(win, 0, 0);
    // move and print in window
    /*mvwprintw(titleBar_subwin, 1, 1, "Shutdown");
    mvwprintw(titleBar_subwin, 1, 12, "Restart");
    mvwprintw(titleBar_subwin, 1, 22, "Hibernate");
    mvwprintw(titleBar_subwin, 1, 34, "Sleep");
    mvwprintw(titleBar_subwin, 1, winMaxX-18, "Desktop Manager");
    */

    draw_titlebar(titleBar_subwin, -1);

    // List Available Desktop Managers
    // list_available_desktop_managers(win, winMaxY/2, winMaxX/2);
    // show_datetime(titleBar_subwin, 1, (winMaxX/2)-15);
    // show_calender(win, winMaxY/8, (winMaxX/2)-10);
    // wrefresh(messageBox);
    //show_cpu_utilization(messageBox_msg, 0, 0);
    // show_network_interface_status(win, winMaxY/2, (winMaxX*0.75));
    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, "CPU Status", "mpstat -P ALL");
    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, "Calender", "cal");
    messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, "Network Status", "tcpdump --list-interfaces");


    // Login dialog box
    // mvwprintw(authBox, winMaxY*0.75, (winMaxX/2)-10, "USER : ");
    // mvwprintw(authBox, (winMaxY*0.75)+1, (winMaxX/2)-10, "PASS : ");
    /*mvwprintw(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX/4)+5, "USER : ");
    mvwprintw(authBox, (loginBoxMaxY/2), (loginBoxMaxX/4)+5, "PASS : ");


    // Login Button
    mvwprintw(authBox, loginBoxMaxY-1, loginBoxMaxX-10, "LOGIN");*/


    start_color();

    //// UserName Visibility colorMap
    user_pass_visibility(authBox, (loginBoxMaxY/2)-1, loginBoxMaxX-10);

    //// UserPass Visibility colorMap
    user_pass_visibility(authBox, (loginBoxMaxY/2), loginBoxMaxX-10);


    // Set Login Matrix Config
    setLoginMatrixWindow(authBox); // The LoginMatrix to be shown in the provided window
    loginMatrixSetConfig(1, 1, loginBoxMaxY-1, (loginBoxMaxX/4)-2);


    // Draw Auth Box
    drawAuthBox(winMaxY/8, winMaxX/2, winMaxY*0.75,winMaxX/4);

    // refreshing the window
    wrefresh(win);
    wrefresh(titleBar_subwin);
    wrefresh(accountPicBox);
    // wrefresh(authBox);


    // messageBoxWindow(msgBoxMaxH, msgBoxMaxW, msgBoxMaxY, msgBoxMaxX, 1, "Network Status", "tcpdump --list-interfaces");

    // wrefresh(win);
    // refresh();

    // generate new colour map on each input in user/password field

  //  do{
        genProfilePicture(accountPicBoxMaxH-1, accountPicBoxMaxW-4, 1, 2);
        login_userField(authBox, (loginBoxMaxY/2)-1, (loginBoxMaxX/4)+14);
        login_passField(authBox, (loginBoxMaxY/2), (loginBoxMaxX/4)+14);
        draw_titlebar(titleBar_subwin, 0);
    //}while(1);


    // show_datetime(titleBar_subwin, 1, (winMaxX/2)-16);
    // draw_titlebar(titleBar_subwin, 0);
    // wrefresh(titleBar_subwin);*/
    // delwin(messageBox);
    getch();
    endwin();
    return 0;
}
