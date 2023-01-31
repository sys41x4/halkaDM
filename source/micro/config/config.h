#ifndef HALKADM_CONFIG_H
#define HALKADM_CONFIG_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define DISPLAY      ":1"
#define VT           "vt01"
static bool testing = false;
static pthread_t login_thread;
static pid_t x_server_pid;


char KEY_ESCAPE = '\x1b';

class CONFIG{
public:
char* capslock;
char* err_alloc;
char* err_bounds;
char* err_chdir;
char* err_console_dev;
char* err_dgn_oob;
char* err_domain;
char* err_hostname;
char* err_mlock;
char* err_null;
char* err_pam;
char* err_pam_abort;
char* err_pam_acct_expired;
char* err_pam_auth;
char* err_pam_authinfo_unavail;
char* err_pam_authok_reqd;
char* err_pam_buf;
char* err_pam_cred_err;
char* err_pam_cred_expired;
char* err_pam_cred_insufficient;
char* err_pam_cred_unavail;
char* err_pam_maxtries;
char* err_pam_perm_denied;
char* err_pam_session;
char* err_pam_sys;
char* err_pam_user_unknown;
char* err_path;
char* err_perm_dir;
char* err_perm_group;
char* err_perm_user;
char* err_pwnam;
char* err_user_gid;
char* err_user_init;
char* err_user_uid;
char* err_xsessions_dir;
char* err_xsessions_open;

    char* service_name;
    int tty;
    char* path;
    char* term_reset_cmd;
    char* wayland_cmd;
    char* x_cmd_setup;
    char* xinitrc;
    char* x_cmd;
    char* xauth_cmd;
    char* xsessions;
    char* waylandsessions;
    char* mcookie_cmd;

    // Language Variable Config
    char* loginBTN_text;
    char* logoutBTN_text;
    char* shutdownBTN_text;
    char* restartBTN_text;
    char* sleepBTN_text;
    char* powerList_text;
    char* utilitiesList_text;
    char* calenderBTN_text;
    char* cpuStatusBTN_text;
    char* networkStatusBTN_text;
    char* refreshBTN_text;
    char* exitBTN_text;
    char* powerBTN_text;
    char* utilitiesBTN_text;
    char* desktopENVBTN_text;
    char* currentDesktopENV_text;
    char* usernameFieldID_text;
    char* userpassFieldID_text;


    // Software Info
    char* package;

    // Default KeyValues
    char KEY_ESCAPE = '\x1b';
    int asciiColors[8] = {
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
    // char* newSelectedDesktopEnv = nullptr;
    char* SESSION_KEY = nullptr;
    int SESSION_KEY_LENGTH = 33;
    int maxTitleBarItemTreeDepth = 2;
    int titleBarHoverableItemCount = 3;
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
    char* currentUserDesktopEnvComProtocol;
    char* availableUserDesktopEnvComProtocol;
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
    char* loginFailed_text;
    char* loginSuccess_text;
    char* logoutFailed_text;
    char* logoutSuccess_text;
    char* emptyCredPassed;
    char* incorrectCred;

    // CMD Varables;
    char* uuidCMD;
    char* usrGroupCMD;
    char* getUserFullnameCMD;
    char* usrHomeDirCMD = nullptr;
    char* usrShellCMD;
    char* getSystemUnameCMD;
    char* setUserDesktopEnvCMD;
    char* getDefaultUserDesktopEnvCMD;
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

    void allocate();
    void deallocate();
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


/*

// COLORS MAPOUT //

0: [1-8] : size:8 | Pre Setup Colours
1: [9-16] | [COLOR_BLACK, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_WHITE, COLOR_MAGENTA, COLOR_BLUE, COLOR_CYAN] : size:8 | Exclude the following for directly calling them in any case
2: [<totalRandomizedColors>] | <randomize <totalRandomizedColors> colors> : size <totalRandomizedColors> | Randomization will be done only once on initial loading of the executable, to reduce memory consumpti>2: [totalRandomizedColors>+3] | [username visibility colourmap] : size:3
3: [<<totalRandomizedColors>+3>+3] | [userpass visibility colourmap] : size:3
4: [Profile Pic] : size:(Relative)maxX*maxY | 1D-array | available colour : 8+<static_Colors><totalRandomizedColors>
5: [Login Randomization colourMap] : size:(Relative)maxX*maxY | 1D-array | available colour : 8+<static_Colors>+<totalRandomizedColors>


*/

#endif
