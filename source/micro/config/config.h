#ifndef HALKADM_CONFIG_H
#define HALKADM_CONFIG_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

/*#define DISPLAY      ":1"
#define VT           "vt01"
*/static bool testing = false;
static pthread_t login_thread;
static pid_t x_server_pid;


#define DEBIAN_DISTRO 0

#define DM_REFRESH '\x2b'
#define DM_CLEAR '\x3e'
#define DM_STABLE '\x02'
#define DM_START '\x01'
#define DM_EXIT '\xfe'

//char KEY_ESCAPE = '\x1b';

class CONFIG{
public:
    int distro;




char* capslock=nullptr;
char* err_alloc=nullptr;
char* err_bounds=nullptr;
char* err_chdir=nullptr;
char* err_console_dev=nullptr;
char* err_dgn_oob=nullptr;
char* err_domain=nullptr;
char* err_hostname=nullptr;
char* err_mlock=nullptr;
char* err_null=nullptr;
char* err_pam=nullptr;
char* err_pam_abort=nullptr;
char* err_pam_acct_expired=nullptr;
char* err_pam_auth=nullptr;
char* err_pam_authinfo_unavail=nullptr;
char* err_pam_authok_reqd=nullptr;
char* err_pam_buf=nullptr;
char* err_pam_cred_err=nullptr;
char* err_pam_cred_expired=nullptr;
char* err_pam_cred_insufficient=nullptr;
char* err_pam_cred_unavail=nullptr;
char* err_pam_maxtries=nullptr;
char* err_pam_perm_denied=nullptr;
char* err_pam_session=nullptr;
char* err_pam_sys=nullptr;
char* err_pam_user_unknown=nullptr;
char* err_path=nullptr;
char* err_perm_dir=nullptr;
char* err_perm_group=nullptr;
char* err_perm_user=nullptr;
char* err_pwnam=nullptr;
char* err_user_gid=nullptr;
char* err_user_init=nullptr;
char* err_user_uid=nullptr;
char* err_xsessions_dir=nullptr;
char* err_xsessions_open=nullptr;

    char* service_name=nullptr;
    int tty = 1;
    char* path=nullptr;
    char* term_reset_cmd=nullptr;
    char* wayland_cmd=nullptr;
    char* x_cmd_setup=nullptr;
    char* xinitrc=nullptr;
    char* x_cmd=nullptr;
    char* xauth_cmd=nullptr;
    char* xsessions=nullptr;
    char* waylandsessions=nullptr;
    char* mcookie_cmd=nullptr;
    char* console_dev=nullptr;
    char dm_display_visual=DM_START;


    // Language Variable Config



    char* default_text=nullptr;
    char* loginBTN_text=nullptr;
    char* logoutBTN_text=nullptr;
    char* shutdownBTN_text=nullptr;
    char* restartBTN_text=nullptr;
    char* sleepBTN_text=nullptr;
    char* powerList_text=nullptr;
    char* utilitiesList_text=nullptr;
    char* calenderBTN_text=nullptr;
    char* cpuStatusBTN_text=nullptr;
    char* networkStatusBTN_text=nullptr;
    char* refreshBTN_text=nullptr;
    char* exitBTN_text=nullptr;
    char* powerBTN_text=nullptr;
    char* utilitiesBTN_text=nullptr;
    char* desktopENVBTN_text=nullptr;
    char* currentDesktopENV_text=nullptr;
    char* usernameFieldID_text=nullptr;
    char* userpassFieldID_text=nullptr;


    // Software Info
    char* package=nullptr;

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
    int totalRandomizedColors=0;
    // char* newSelectedDesktopEnv = nullptr;
    char* SESSION_KEY = nullptr;
    int SESSION_KEY_LENGTH = 33;
    int maxTitleBarItemTreeDepth = 2;
    int* titleBarItemTree=nullptr;
    int* loginColourMatrixConf=nullptr;
    int titleBarHoverableItemCount = 3;
    int usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    int userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    int maxUsernameLen = 32;
    int maxUserpassLen = 255;
    int visibleAuthStrLen = 50;


    char* uuid=nullptr;
    char* usrGroup=nullptr;
    char* getUserFullname=nullptr;
    char* usrHomeDir = nullptr;
    char* usrShell=nullptr;
    char* getSystemUname=nullptr;
    char* setUserDesktopEnv=nullptr;
    char* currentUserDesktopEnvComProtocol=nullptr;
    char* availableUserDesktopEnvComProtocol=nullptr;
    char* currentUserDesktopEnv=nullptr;
    char* availableUserDesktopEnv=nullptr;
    char* getSystemBasicInfo=nullptr;
    char* shutdown=nullptr;
    char* sleep=nullptr;
    char* restart=nullptr;
    char* dateTime=nullptr;
    char* calender=nullptr;
    char* cpuStatus=nullptr;
    char* networkStatus=nullptr;


    // Alert Text
    char* loginFailed_text=nullptr;
    char* loginSuccess_text=nullptr;
    char* logoutFailed_text=nullptr;
    char* logoutSuccess_text=nullptr;
    char* emptyCredPassed=nullptr;
    char* incorrectCred=nullptr;

    // CMD Varables;
    char* getAvailableUsernameCMD=nullptr;
    char* uuidCMD=nullptr;
    char* usrGroupCMD=nullptr;
    char* getUserFullnameCMD=nullptr;
    char* usrHomeDirCMD = nullptr;
    char* usrShellCMD=nullptr;
    char* getSystemUnameCMD=nullptr;
    char* setUserDesktopEnvCMD=nullptr;
    char* getUserDesktopEnvCMD=nullptr;
//    char* getDefaultUserDesktopEnvCMD;
    char* setUserDesktopEnvTypeCMD=nullptr;
    char* getUserDesktopEnvTypeCMD=nullptr;
    char* currentUserDesktopEnvCMD=nullptr;
    char* availableUserDesktopEnvCMD=nullptr;
    char* getSystemBasicInfoCMD=nullptr;
    char* shutdownCMD=nullptr;
    char* sleepCMD=nullptr;
    char* restartCMD=nullptr;
    char* dateTimeCMD=nullptr;
    char* calenderCMD=nullptr;
    char* cpuStatusCMD=nullptr;
    char* networkStatusCMD=nullptr;

    int refresh=0;
    int exit=0;

    // Default Value Loading
    void load_default_CMD();
    void load_default_keyValues();
    void load_default_softwareInfo();
    void load_default_lang();
    void load_default_alertText();
    void error_default_lang();

    // Config Values allocation & Deallocation & nullification
    void null_values();
    void allocate();
    void deallocate();
};


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
