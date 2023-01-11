#ifndef halkaDM_micro_H
#define halkaDM_micro_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

char package[] = "halkaDM:nano";
char KEY_ESCAPE = '\x1b';

/*

// COLORS MAPOUT //

0: [1-8] : size:8 | Pre Setup Colours
1: [9-16] | [COLOR_BLACK, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_WHITE, COLOR_MAGENTA, COLOR_BLUE, COLOR_CYAN] : size:8 | Exclude the following for directly calling them in any case
2: [<totalRandomizedColors>] | <randomize <totalRandomizedColors> colors> : size <totalRandomizedColors> | Randomization will be done only once on initial loading of the executable, to reduce memory consumpti>2: [<totalRandomizedColors>+3] | [username visibility colourmap] : size:3
3: [<<totalRandomizedColors>+3>+3] | [userpass visibility colourmap] : size:3
4: [Profile Pic] : size:(Relative)maxX*maxY | 1D-array | available colour : 8+<static_Colors><totalRandomizedColors>
5: [Login Randomization colourMap] : size:(Relative)maxX*maxY | 1D-array | available colour : 8+<static_Colors>+<totalRandomizedColors>


*/

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

int totalRandomizedColors = 10; // Must be loaded from the config file
int totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);
int totalManualColors = 8;

int currentTitleID = 0;

int maxSubItemElementLen=16;


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

// ENV: DEFAULT

char titleBarItems[][30] = {
    "POWER",
    "Utilities",
    "ENV: DEFAULT"

};

char subItemsCmd[][10][55] = {
    {
        "echo Sleep_clked",
        "sudo shutdown -r now",
        "sudo /usr/sbin/shutdown -h now"
    },
    {
        "/usr/bin/cal",
        "/usr/bin/mpstat -P ALL",
        "/usr/sbin/tcpdump --list-interfaces"
    },
    {
        "ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev"
    }

};

int usernameVisibilityConf[3] = {0, 0, 0}; // {[0..2], [0..1], [0..1]}Default username_visibility_config
int userpassVisibilityConf[3] = {1, 0, 0}; // Default userpass_visibility_config
#endif
