#include "config.h"
#include "../lib/dataHandling.h"
// #include "../lib/dataHandling.cpp"
/*void load_default_softwareInfo(){
    // Default Software Info
    config.package = "halkaDM:nano|testing";

};

void load_default_lang(){
    // Language Variable Config
    config.loginBTN_text = " LOGIN ";
    config.logoutBTN_text = " LOGOUT ";
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

};*/


void load_default_softwareInfo(){
    // Default Software Info
    config.package = data_handler.cpArray(config.package, "halkaDM:nano|testing");

};

void load_default_lang(){
    // Language Variable Config
    config.loginBTN_text = data_handler.cpArray(config.loginBTN_text, " LOGIN ");
    config.logoutBTN_text = data_handler.cpArray(config.logoutBTN_text, " LOGOUT ");
    /*config.shutdownBTN_text = data_handler.cpArray(config.shutdownBTN_text, "Shutdown");
    config.restartBTN_text = data_handler.cpArray(config.restartBTN_text, "Restart");
    config.sleepBTN_text = data_handler.cpArray(config.sleepBTN_text, "Sleep");*/
    /*config.powerList_text = data_handler.cpArray(config.powerList_text, "Sleep\7Restart\7Shutdown\7");
    config.calenderBTN_text = data_handler.cpArray(config.calenderBTN_text, "Calender");
    config.cpuStatusBTN_text = data_handler.cpArray(config.cpuStatusBTN_text, "CPU Status");
    config.networkStatusBTN_text = data_handler.cpArray(config.networkStatusBTN_text, "Network Status");
    config.refreshBTN_text = data_handler.cpArray(config.refreshBTN_text, "Refresh");
    config.exitBTN_text = data_handler.cpArray(config.exitBTN_text, "Exit");*/
    config.powerList_text = data_handler.cpArray(config.powerList_text, "Sleep\7Restart\7Shutdown\7");
    config.utilitiesList_text = data_handler.cpArray(config.utilitiesList_text, "Calender\7CPU Status\7Network Status\7Refresh\7Exit\7");
    config.powerBTN_text = data_handler.cpArray(config.powerBTN_text, "Power");
    config.utilitiesBTN_text = data_handler.cpArray(config.utilitiesBTN_text, "Utilities");
    config.desktopENVBTN_text = data_handler.cpArray(config.desktopENVBTN_text, "ENV");
    config.currentDesktopENV_text = data_handler.cpArray(config.currentDesktopENV_text, "Default");
    config.usernameFieldID_text = data_handler.cpArray(config.usernameFieldID_text, "USER");
    config.userpassFieldID_text = data_handler.cpArray(config.userpassFieldID_text, "PASS");

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

void load_default_alertText(){
    // Alert Text
    config.loginFailed_text =  data_handler.flatKeyValue(config.loginFailed_text, '\6', "LOGIN Failed", "Access Rejected");
    config.loginSuccess_text = data_handler.flatKeyValue(config.loginSuccess_text, '\6', "LOGIN Success", "Access Granted");
    config.logoutFailed_text = data_handler.flatKeyValue(config.logoutFailed_text, '\6', "LOGOUT Failed", "Access Rejected");
    config.logoutSuccess_text = data_handler.flatKeyValue(config.logoutSuccess_text, '\6', "LOGOUT Success", "Good Bye :)");
    config.emptyCredPassed = data_handler.flatKeyValue(config.emptyCredPassed, '\6', "LOGIN Failed", "Empty Credentials Passed");
    config.incorrectCred = data_handler.flatKeyValue(config.incorrectCred, '\6', "LOGIN Failed", "Incorrect Credentials");
};


void load_default_CMD(){
    // CMD Varables; // Specifically for Debian
    // Variable Identifier that is to be replaced [Format : $[<variable>]$]
    //                                            [ Requires 4 variable for position identification $,[,],$
    config.uuidCMD = data_handler.cpArray(config.uuidCMD, "getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f3 | tr -s '\n' ' '");
    config.usrGroupCMD = data_handler.cpArray(config.usrGroupCMD, "getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f4 | tr -s '\n' ' '");
    config.getUserFullnameCMD = data_handler.cpArray(config.getUserFullnameCMD, "getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f5 | tr -s '\n' ' '");
    config.usrHomeDirCMD = data_handler.cpArray(config.usrHomeDirCMD, "getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f6 | tr -s '\n' '/'");
    config.usrShellCMD = data_handler.cpArray(config.usrShellCMD, "getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f7 | tr -s '\n' ' '");
    config.getSystemUnameCMD = data_handler.cpArray(config.getSystemUnameCMD, "uname -n -o");
    config.currentUserDesktopEnvCMD = data_handler.cpArray(config.currentUserDesktopEnvCMD, "sudo cat /var/lib/AccountsService/users/$[USER]$ 2>/dev/null | grep 'XSe*' | cut -d '=' -f 2");
    config.availableUserDesktopEnvCMD = data_handler.cpArray(config.availableUserDesktopEnvCMD, "ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'");
    // config.availableUserDesktopEnvCMD = data_handler.cpArray(config.availableUserDesktopEnvCMD, "ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev");
    config.setUserDesktopEnvCMD = data_handler.cpArray(config.setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@; 1s@^@exec @' > /home/$[USER]$/.xsessionrc");
    config.getSystemBasicInfoCMD = data_handler.cpArray(config.getSystemBasicInfoCMD, "cat /etc/os-release | grep -w -E 'NAME=|VERSION=' | cut -d '=' -f 2 | cut -d '\"' -f 2 | tr -s '\n' ' '");
    config.shutdownCMD = data_handler.cpArray(config.shutdownCMD, "sudo /usr/sbin/shutdown -h now");
    config.sleepCMD = data_handler.cpArray(config.sleepCMD, "sudo systemctl suspend");
    config.restartCMD = data_handler.cpArray(config.restartCMD, "sudo shutdown -r now");
    config.dateTimeCMD = data_handler.cpArray(config.dateTimeCMD, "/usr/bin/date | tr -s '\n' ' '");
    config.calenderCMD = data_handler.cpArray(config.calenderCMD, "/usr/bin/cal");
    config.cpuStatusCMD = data_handler.cpArray(config.cpuStatusCMD, "/usr/bin/mpstat -P ALL");
    config.networkStatusCMD = data_handler.cpArray(config.networkStatusCMD, "/usr/sbin/tcpdump --list-interfaces");

    config.refresh = 0;
    config.exit = 0;
};
