#ifndef HALKADM_DRAW_H
#define HALKADM_DRAW_H

#include <locale.h>
#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>
#include "inputs.h"
#include "../config/config.h"

class DRAW{
public:

    void charArr(WINDOW *win, int x, int y, int colorID, const char* arr);
    void charArr(WINDOW *win, int x, int y, int colorID, char* arr);

};

/*    WINDOW *mainScreenWin;
    WINDOW *subItemListWindow;
    WINDOW *loginColourMatrixWin;
    WINDOW *authBox;
    WINDOW *accountPicBox;
    WINDOW *titleBar_subwin;
    WINDOW *messageBoxBorderWindow;
    WINDOW *messageBox_msg;
*/
#endif
