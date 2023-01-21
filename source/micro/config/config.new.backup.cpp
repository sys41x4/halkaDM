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

void load_default_alertText(){
    // Alert Text
    config.loginFailed_text = {"LOGIN Failed", "Access Rejected"};
    config.loginSuccess_text = {"LOGIN Success", "Access Granted"};
    config.logoutFailed_text = {"LOGOUT Failed", "Access Rejected"};
    config.logoutSuccess_text = {"LOGOUT Success", "Good Bye :)"};
    config.emptyCredPassed = {"LOGIN Failed", "Empty Credentials Passed"};
    config.incorrectCred = {"LOGIN Failed", "Incorrect Credentials"};
};

void load_default_CMD(){
    // CMD Varables; // Specifically for Debian
    // Variable Identifier that is to be replaced [Format : $[<variable>]$]
    //                                            [ Requires 4 variable for position identification $,[,],$
    config.uuidCMD = "getent passwd $[USER]$ | grep -v '/nologin' | cut -d: -f3 | tr -s '\n' ' '";
    config.usrGroupCMD = "getent passwd $[USER]$ | grep -v '/nologin' | cut -d: -f4 | tr -s '\n' ' '";
    config.getUserFullnameCMD = "getent passwd $[USER]$ | grep -v '/nologin' | cut -d: -f5 | tr -s '\n' ' '";
    config.usrHomeDirCMD = "getent passwd $[USER]$ | grep -v '/nologin' | cut -d: -f6 | tr -s '\n' '/'";
    config.usrShellCMD = "getent passwd $[USER]$ | grep -v '/nologin' | cut -d: -f7 | tr -s '\n' ' '";
    config.getSystemUnameCMD = "uname -n -o";
    config.currentUserDesktopEnvCMD = "sudo cat /var/lib/AccountsService/users/$[USER]$ 2>/dev/null | grep 'XSe*' | cut -d '=' -f 2";
    config.availableUserDesktopEnvCMD = "ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'";
    config.setUserDesktopEnvCMD = "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@; 1s@^@exec @' > /home/$[USER]$/.xsessionrc";
    config.getSystemBasicInfoCMD = "cat /etc/os-release | grep -w -E 'NAME=|VERSION=' | cut -d '=' -f 2 | cut -d '\"' -f 2 | tr -s '\n' ' '";
    config.shutdownCMD = "sudo /usr/sbin/shutdown -h now";
    config.sleepCMD = "sudo systemctl suspend";
    config.restartCMD = "sudo shutdown -r now";
    config.dateTimeCMD = "/usr/bin/date | tr -s '\n' ' '";
    config.calenderCMD = "/usr/bin/cal";
    config.cpuStatusCMD = "/usr/bin/mpstat -P ALL";
    config.networkStatusCMD = "/usr/sbin/tcpdump --list-interfaces";

    config.refresh = 0;
    config.exit = 0;
};
