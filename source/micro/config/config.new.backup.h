#ifndef HALKADM_CONFIG_H
#define HALKADM_CONFIG_H

#include <locale.h>
//#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>



class CONFIG{
public:

    // Language Variable Config
    char* loginBTN_text;
    char* logoutBTN_text;
    char* shutdownBTN_text;
    char* restartBTN_text;
    char* sleepBTN_text;
    char* calenderBTN_text;
    char* cpuStatusBTN_text;
    char* networkStatusBTN_text;
    char* refreshBTN_text;
    char* exitBTN_text;
    char* powerBTN_text;
    char* utilitiesBTN_text;
    char* desktopENVBTN_text;
    char* defaultDesktopENV_text;
    char* usernameFieldID_text;
    char* userpassFieldID_text;


    // Software Info
    char* package;

    // Default KeyValues
    char KEY_ESCAPE = '\x1b';
    int asciiColors[] = {
        COLOR_BLACK,
        COLOR_GREEN,
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_WHITE,
        COLOR_MAGENTA,
        COLOR_BLUE,
        COLOR_CYAN
    };
    int totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);
    int totalManualColors = 8;
    int totalRandomizedColors;
    char* newSelectedDesktopEnv = nullptr;
    char* SESSION_KEY = nullptr;
    int SESSION_KEY_LENGTH;
    int maxTitleBarItemTreeDepth = 2;
    char** titleBarSubItems;
    char** titleBarSubItemsCMD;
    int usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    int userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    int maxUsernameLen = 32;
    int maxUserpassLen = 255;
    int visibleAuthStrLen = 50;


    char* uuid;
    char* usrGroup;
    char* getUserFullname;
    char* usrHomeDir = nullptr;
    char* usrShell;
    char* getSystemUname;
    char* setUserDesktopEnv;
    char* currentUserDesktopEnv;
    char* availableUserDesktopEnv;
    char* getSystemBasicInfo;
    char* shutdown;
    char* sleep;
    char* restart;
    char* dateTime;
    char* calender;
    char* cpuStatus;
    char* networkStatus;


    // Alert Text
    char** loginFailed_text;
    char** loginSuccess_text;
    char** logoutFailed_text;
    char** logoutSuccess_text;
    char** emptyCredPassed;
    char** incorrectCred;

    // CMD Varables;
    char* uuidCMD;
    char* usrGroupCMD;
    char* getUserFullnameCMD;
    char* usrHomeDirCMD = nullptr;
    char* usrShellCMD;
    char* getSystemUnameCMD;
    char* setUserDesktopEnvCMD;
    char* currentUserDesktopEnvCMD;
    char* availableUserDesktopEnvCMD;
    char* getSystemBasicInfoCMD;
    char* shutdownCMD;
    char* sleepCMD;
    char* restartCMD;
    char* dateTimeCMD;
    char* calenderCMD;
    char* cpuStatusCMD;
    char* networkStatusCMD;

    int refresh=0;
    int exit=0;
};

//class DEFAULT_CONFIG{
//public:

    void load_default_CMD();
    void load_default_keyValues();
    void load_default_softwareInfo();
    void load_default_lang();
    void load_default_alertText();
//};

CONFIG config;
// DEFAULT_CONFIG default_config;

char package[] = "halkaDM:nano|testing";
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
    COLOR_GREEN,
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_WHITE,
    COLOR_MAGENTA,
    COLOR_BLUE,
    COLOR_CYAN
};

char* SESSION_KEY = nullptr;
int SESSION_KEY_LENGTH = 33;

int totalRandomizedColors = 10; // Must be loaded from the config file
int totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);
int totalManualColors = 8;

// char* currentDesktopENV;
char currentDesktopENV[] = "ubuntu";
char* usrHomeDir = nullptr; // getent passwd username | cut -d: -f6

int currentTitleID = 0; // not included

int maxSubItemElementLen=16; // Not Included

// Status {Visible_Text,...}
//        {SECTION, ...}
//        {KEY, ....}

char*** titleBarSubItems;

char powerSubItems[][3][15] = {
    {"Sleep", "Restart", "Shutdown"},
    {"CMD@sleep", "CMD@restart", "CMD@shutdown"}
};

char utilitiesSubItems[][5][20] = {
    {"Calender", "CPU Status", "Network Status", "Refresh", "Exit"},
    {"CMD@calender", "CMD@cpuStatus", "CMD@networkStatus"}
};

// ENV: DEFAULT

char titleBarItems[][30] = {
    "POWER",
    "Utilities",
    "ENV: DEFAULT"

};

/*char subItemsCmd[][10][55] = {
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

};*/

int maxTitleBarItemTreeDepth = 2;

int usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
int userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config

int maxUsernameLen = 32;
int maxUserpassLen = 255;

int visibleAuthStrLen = 50;

#endif
