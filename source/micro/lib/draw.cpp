#include "draw.h"


void DRAW::charArr(WINDOW *win, int y, int x, int colorID, const char* arr){
    wattron(win, COLOR_PAIR(colorID));
    mvwprintw(win, y, x, arr);
    wattroff(win, COLOR_PAIR(colorID));
}

void DRAW::charArr(WINDOW *win, int y, int x, int colorID, char* arr){
    wattron(win, COLOR_PAIR(colorID));
    mvwprintw(win, y, x, arr);
    wattroff(win, COLOR_PAIR(colorID));
}
