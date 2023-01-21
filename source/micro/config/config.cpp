#include "config.h"

void load_default_softwareInfo(){
    // Default Software Info
    config.package = "halkaDM:nano|testing";

};

void load_default_lang(){
    // Language Variable Config
    config.loginBTN_text = "LOGIN";
    config.logoutBTN_text = "LOGOUT";
    config.shutdownBTN_text = "Shutdown";
    config.restartBTN_text = "Restart";
    config.sleepBTN_text = "Sleep";
    config.calenderBTN_text = "Calender";
    config.cpuStatusBTN_text = "CPU Status";
    config.networkStatusBTN_text = "Network Status";
    config.refreshBTN_text = "Refresh";
    config.exitBTN_text = "Exit";
    config.powerBTN_text = "Power";
    config.utilitiesBTN_text = "Utilities";
    config.desktopENVBTN_text = "ENV";
    config.defaultDesktopENV_text = "Default";
    config.usernameFieldID_text = "USER";
    config.userpassFieldID_text = "PASS";

};

void load_default_keyValues(){
    // Default KeyValues
    /*config.KEY_ESCAPE = '\x1b';
    config.asciiColors[] = {
        COLOR_BLACK,
        COLOR_GREEN,
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_WHITE,
        COLOR_MAGENTA,
        COLOR_BLUE,
        COLOR_CYAN
    };
    config.totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);*/
    config.totalManualColors = 8;
    config.totalRandomizedColors = 10;
    // config.SESSION_KEY = nullptr;
    // config.SESSION_KEY_LENGTH = 32;
    config.maxTitleBarItemTreeDepth = 2;
    // char** titleBarSubItems;
    // char** titleBarSubItemsCMD;
    // config.usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    // config.userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    config.maxUsernameLen = 32;
    config.maxUserpassLen = 255;
    config.visibleAuthStrLen = 50;
};

void load_default_errorText(){
    // Error Text
    config.loginFailed_text = "LOGIN Failed";
    config.loginSuccess_text = "LOGIN Success";
    config.logoutFailed_text = "LOGOUT Failed";
    config.logoutSuccess_text = "LOGOUT Success";
    config.emptyCredPassed = "Empty Credentials Passed";
    config.incorrectCred = "Incorrect Credentials";
};


    // CMD Varables;
    char* getUserFullname;
    char* getSystemUname;
    char* usrHomeDir = nullptr;
    char* currentUserDesktopEnvCMD;
    char* availableUserDesktopEnvCMD;
    char* getSystemBasicInfo;
    char* shutdownCMD;
    char* sleepCMD;
    char* restartCMD;
    char* dateTimeCMD;
    char* calenderCMD;
    char* cpuStatusCMD;
    char* networkStatusCMD;
    int refresh=0;
    int exit=0;
