#include "config.h"
#include "../lib/dataHandling.h"
#include "../lib/inputs.h"

#ifndef DEBUG
	#define INI_LANG DATADIR "/lang/%s.ini"
	#define INI_CONFIG "/etc/halkaDM/halkaDM.micro/config.ini"
#else
	#define INI_LANG "../res/lang/%s.ini"
	#define INI_CONFIG "../res/config.ini"
#endif

void CONFIG::deallocate(){

    free(config.package);

    free(config.capslock);
    free(config.err_alloc);
    free(config.err_bounds);
    free(config.err_chdir);
    free(config.err_console_dev);
    free(config.err_dgn_oob);
    free(config.err_domain);
    free(config.err_hostname);
    free(config.err_mlock);
    free(config.err_null);
    free(config.err_pam);
    free(config.err_pam_abort);
    free(config.err_pam_acct_expired);
    free(config.err_pam_auth);
    free(config.err_pam_authinfo_unavail);
    free(config.err_pam_authok_reqd);
    free(config.err_pam_buf);
    free(config.err_pam_cred_err);
    free(config.err_pam_cred_expired);
    free(config.err_pam_cred_insufficient);
    free(config.err_pam_cred_unavail);
    free(config.err_pam_maxtries);
    free(config.err_pam_perm_denied);
    free(config.err_pam_session);
    free(config.err_pam_sys);
    free(config.err_pam_user_unknown);
    free(config.err_path);
    free(config.err_perm_dir);
    free(config.err_perm_group);
    free(config.err_perm_user);
    free(config.err_pwnam);
    free(config.err_user_gid);
    free(config.err_user_init);
    free(config.err_user_uid);
    free(config.err_xsessions_dir);
    free(config.err_xsessions_open);

    free(config.service_name);
    free(config.loginBTN_text);
    free(config.logoutBTN_text);
    free(config.powerList_text);
    free(config.utilitiesList_text);
    free(config.powerBTN_text);
    free(config.utilitiesBTN_text);
    free(config.desktopENVBTN_text);
    free(config.currentDesktopENV_text);
    free(config.usernameFieldID_text);
    free(config.userpassFieldID_text);

    free(config.loginFailed_text);
    free(config.loginSuccess_text);
    free(config.logoutFailed_text);
    free(config.logoutSuccess_text);
    free(config.emptyCredPassed);
    free(config.incorrectCred);

    free(config.uuidCMD);
    free(config.usrGroupCMD);
    free(config.getUserFullnameCMD);
    free(config.usrHomeDirCMD);
    free(config.usrShellCMD);
    free(config.getSystemUnameCMD);
    free(config.currentUserDesktopEnvCMD);
    free(config.availableUserDesktopEnvCMD);
    free(config.setUserDesktopEnvCMD);
    free(config.getSystemBasicInfoCMD);
    free(config.shutdownCMD);
    free(config.sleepCMD);
    free(config.restartCMD);
    free(config.dateTimeCMD);
    free(config.calenderCMD);
    free(config.cpuStatusCMD);
    free(config.networkStatusCMD);
}

void load_default_softwareInfo(){
    // Default Software Info
    config.package = data_handler.cpArray(config.package, "halkaDM:nano|testing");

};

void load_default_lang(){
config.capslock = strdup("capslock");
config.err_alloc = strdup("failed memory allocation");
config.err_bounds = strdup("out-of-bounds index");
config.err_chdir = strdup("failed to open home folder");
config.err_console_dev = strdup("failed to access console");
config.err_dgn_oob = strdup("log message");
config.err_domain = strdup("invalid domain");
config.err_hostname = strdup("failed to get hostname");
config.err_mlock = strdup("failed to lock password memory");
config.err_null = strdup("null pointer");
config.err_pam = strdup("pam transaction failed");
config.err_pam_abort = strdup("pam transaction aborted");
config.err_pam_acct_expired = strdup("account expired");
config.err_pam_auth = strdup("authentication error");
config.err_pam_authinfo_unavail = strdup("failed to get user info");
config.err_pam_authok_reqd = strdup("token expired");
config.err_pam_buf = strdup("memory buffer error");
config.err_pam_cred_err = strdup("failed to set credentials");
config.err_pam_cred_expired = strdup("credentials expired");
config.err_pam_cred_insufficient = strdup("insufficient credentials");
config.err_pam_cred_unavail = strdup("failed to get credentials");
config.err_pam_maxtries = strdup("reached maximum tries limit");
config.err_pam_perm_denied = strdup("permission denied");
config.err_pam_session = strdup("session error");
config.err_pam_sys = strdup("system error");
config.err_pam_user_unknown = strdup("unknown user");
config.err_path = strdup("failed to set path");
config.err_perm_dir = strdup("failed to change current directory");
config.err_perm_group = strdup("failed to downgrade group permissions");
config.err_perm_user = strdup("failed to downgrade user permissions");
config.err_pwnam = strdup("failed to get user info");
config.err_user_gid = strdup("failed to set user GID");
config.err_user_init = strdup("failed to initialize user");
config.err_user_uid = strdup("failed to set user UID");
config.err_xsessions_dir = strdup("failed to find sessions folder");
config.err_xsessions_open = strdup("failed to open sessions folder");

    // Language Variable Config
    config.service_name = strdup("halkaDM");
    config.loginBTN_text = data_handler.cpArray(config.loginBTN_text, " LOGIN ");
    config.logoutBTN_text = data_handler.cpArray(config.logoutBTN_text, " LOGOUT ");
    config.powerList_text = data_handler.cpArray(config.powerList_text, "Sleep\7Restart\7Shutdown\7");
    config.utilitiesList_text = data_handler.cpArray(config.utilitiesList_text, "Calender\7CPU Status\7Network Status\7Refresh\7Exit\7");
    config.powerBTN_text = data_handler.cpArray(config.powerBTN_text, "Power");
    config.utilitiesBTN_text = data_handler.cpArray(config.utilitiesBTN_text, "Utilities");
    config.desktopENVBTN_text = data_handler.cpArray(config.desktopENVBTN_text, "ENV");
    config.currentDesktopENV_text = data_handler.cpArray(config.currentDesktopENV_text, "Default");
    config.currentUserDesktopEnvComProtocol =  data_handler.cpArray(config.currentUserDesktopEnvComProtocol, "shell\7xinitrc\7Xorg\7wayland\7Default\7");
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
    config.availableUserDesktopEnvComProtocol = strdup("xsessions\7wayland-sessions\7shell\7");
    config.totalManualColors = 8;
    config.totalRandomizedColors = 10;
    config.tty = 2;
    config.path = strdup("/usr/local/sbin:/usr/local/bin:/usr/bin");
    config.term_reset_cmd = strdup("/usr/bin/tput reset");
    config.wayland_cmd = strdup(DATADIR "/wsetup.sh");
    config.waylandsessions = strdup("/usr/share/wayland-sessions");
    config.x_cmd = strdup("/usr/bin/X");
    config.xinitrc = strdup("~/.xinitrc");
    config.x_cmd_setup = strdup(DATADIR "/xsetup.sh");
    config.xauth_cmd = strdup("/usr/bin/xauth");
    config.xsessions = strdup("/usr/share/xsessions");
    config.mcookie_cmd = strdup("/usr/bin/mcookie");


    // config.SESSION_KEY = nullptr;
    // config.SESSION_KEY_LENGTH = 32;
    config.maxTitleBarItemTreeDepth = 2;
    config.titleBarHoverableItemCount = 4;
    // char** titleBarSubItems;
    // char** titleBarSubItemsCMD;
    // config.usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    // config.userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    config.maxUsernameLen = 255;
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
    config.setUserDesktopEnvCMD = data_handler.cpArray(config.setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@;' > /home/$[USER]$/.xsessionrc");
    config.getDefaultUserDesktopEnvCMD = data_handler.cpArray(config.getDefaultUserDesktopEnvCMD, "head -n 1 /home/$[USER]$/.xsessionrc | tr -s '\n' ' '");
    // config.setUserDesktopEnvCMD = data_handler.cpArray(config.setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@;'");
    // config.saveUserDesktopEnvCMD = data_handler.cpArray(config.saveUserDesktopEnvCMD, "echo $[xsessionCMD]$ > /home/$[USER]$/.xsessionrc");
    // config.setUserDesktopEnvCMD = data_handler.cpArray(config.setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@; 1s@^@exec @' > /home/$[USER]$/.xsessionrc");
    config.getSystemBasicInfoCMD = data_handler.cpArray(config.getSystemBasicInfoCMD, "cat /etc/os-release | grep -w -E 'NAME=|VERSION=' | cut -d '=' -f 2 | cut -d '\"' -f 2 | tr -s '\n' ' '");
    config.shutdownCMD = data_handler.cpArray(config.shutdownCMD, "echo Shutting Down System && sudo /usr/sbin/shutdown -h now");
    config.sleepCMD = data_handler.cpArray(config.sleepCMD, "echo System going to Sleep && sudo systemctl suspend");
    config.restartCMD = data_handler.cpArray(config.restartCMD, "echo Restarting System && sudo shutdown -r now");
    config.dateTimeCMD = data_handler.cpArray(config.dateTimeCMD, "/usr/bin/date | tr -s '\n' ' '");
    config.calenderCMD = data_handler.cpArray(config.calenderCMD, "/usr/bin/cal");
    config.cpuStatusCMD = data_handler.cpArray(config.cpuStatusCMD, "/usr/bin/mpstat -P ALL");
    config.networkStatusCMD = data_handler.cpArray(config.networkStatusCMD, "tcpdump --list-interfaces");

    config.refresh = 0;
    config.exit = 0;
};
