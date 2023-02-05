//#include "h"
#include "../lib/dataHandling.h"
#include "../lib/inputs.h"
#include "../lib/cpptoml.h"
#include <iostream>

#ifndef DEBUG
//	#define INI_LANG DATADIR "/lang/%s.ini"
        #define INI_LANG DATADIR "/lang/en_US.ini"
	#define INI_CONFIG "/etc/halkaDM/halkaDM.micro/ini"
        #define INI_CMD_CONFIG "/etc/halkaDM/halkaDM.micro/config_CMD.toml"
#else
//	#define INI_LANG "../res/lang/%s.ini"
        #define INI_LANG "../res/lang/en_US.ini"
	#define INI_CONFIG "../res/ini"
        #define INI_CMD_CONFIG "../res/config_CMD.toml"
#endif


void CONFIG::load_default_softwareInfo(){
    // Default Software Info
    package = strdup("halkaDM:nano|testing");
    service_name = strdup("halkaDM");
    distro = DEBIAN_DISTRO;
};

void CONFIG::allocate(){

    int tmp=0;
    try {
        auto configFile = cpptoml::parse_file(INI_LANG);

    // Language Variable Config


        loginBTN_text = strdup(configFile->get_as<std::string>("loginBTN_text")->c_str());
        logoutBTN_text = strdup(configFile->get_as<std::string>("logoutBTN_text")->c_str());
        powerBTN_text = strdup(configFile->get_as<std::string>("powerBTN_text")->c_str());
        utilitiesBTN_text = strdup(configFile->get_as<std::string>("utilitiesBTN_text")->c_str());
        default_text = strdup(configFile->get_as<std::string>("default_text")->c_str());
        usernameFieldID_text = strdup(configFile->get_as<std::string>("usernameFieldID_text")->c_str());
        userpassFieldID_text = strdup(configFile->get_as<std::string>("userpassFieldID_text")->c_str());
        currentDesktopENV_text = default_text;
        availableUserDesktopEnv =  default_text;


        tmp=configFile->get_as<std::string>("sleepBTN_text")->length();
        tmp+=configFile->get_as<std::string>("restartBTN_text")->length();
        tmp+=configFile->get_as<std::string>("shutdownBTN_text")->length();
        tmp+=5;
        powerList_text = static_cast<char*>(std::malloc(tmp * sizeof(char)));
        snprintf(powerList_text, tmp, "%s\7%s\7%s\7", configFile->get_as<std::string>("sleepBTN_text")->c_str(), 
        configFile->get_as<std::string>("restartBTN_text")->c_str(), configFile->get_as<std::string>("shutdownBTN_text")->c_str());

        tmp=configFile->get_as<std::string>("calenderBTN_text")->length();
        tmp+=configFile->get_as<std::string>("cpuStatusBTN_text")->length();
        tmp+=configFile->get_as<std::string>("networkStatusBTN_text")->length();
        tmp+=configFile->get_as<std::string>("refreshBTN_text")->length();
        tmp+=configFile->get_as<std::string>("exitBTN_text")->length();
        tmp+=7;
        utilitiesList_text = static_cast<char*>(std::malloc(tmp * sizeof(char)));
        snprintf(utilitiesList_text, tmp, "%s\7%s\7%s\7%s\7%s\7", configFile->get_as<std::string>("calenderBTN_text")->c_str(), 
        configFile->get_as<std::string>("cpuStatusBTN_text")->c_str(), configFile->get_as<std::string>("networkStatusBTN_text")->c_str(), 
        configFile->get_as<std::string>("refreshBTN_text")->c_str(), configFile->get_as<std::string>("exitBTN_text")->c_str());

        tmp=strlen(default_text)+30;
        currentUserDesktopEnvComProtocol = static_cast<char*>(std::malloc(tmp * sizeof(char)));
        snprintf(currentUserDesktopEnvComProtocol, tmp, "%s\7shell\7xinitrc\7Xorg\7wayland\7", configFile->get_as<std::string>("default_text")->c_str());

        tmp=0;
        configFile.reset();

    }
    catch (const cpptoml::parse_exception& e) {
       load_default_lang();
    }




  //  delete configFile;
}

void CONFIG::deallocate(){

    free(package);

//    free(capslock);
    free(err_alloc);
    free(err_bounds);
    free(err_chdir);
    free(err_console_dev);
    free(err_dgn_oob);
    free(err_domain);
    free(err_hostname);
    free(err_mlock);
    free(err_null);
    free(err_pam);
    free(err_pam_abort);
    free(err_pam_acct_expired);
    free(err_pam_auth);
    free(err_pam_authinfo_unavail);
    free(err_pam_authok_reqd);
    free(err_pam_buf);
    free(err_pam_cred_err);
    free(err_pam_cred_expired);
    free(err_pam_cred_insufficient);
    free(err_pam_cred_unavail);
    free(err_pam_maxtries);
    free(err_pam_perm_denied);
    free(err_pam_session);
    free(err_pam_sys);
    free(err_pam_user_unknown);
    free(err_path);
    free(err_perm_dir);
    free(err_perm_group);
    free(err_perm_user);
    free(err_pwnam);
    free(err_user_gid);
    free(err_user_init);
    free(err_user_uid);
    free(err_xsessions_dir);
    free(err_xsessions_open);

    free(service_name);
    free(loginBTN_text);
    free(logoutBTN_text);
    free(powerList_text);
    free(utilitiesList_text);
    free(powerBTN_text);
    free(utilitiesBTN_text);
    free(desktopENVBTN_text);
    free(currentDesktopENV_text);
    free(usernameFieldID_text);
    free(userpassFieldID_text);

    free(loginFailed_text);
    free(loginSuccess_text);
    free(logoutFailed_text);
    free(logoutSuccess_text);
    free(emptyCredPassed);
    free(incorrectCred);

    free(uuidCMD);
    free(usrGroupCMD);
    free(getUserFullnameCMD);
    free(usrHomeDirCMD);
    free(usrShellCMD);
    free(getSystemUnameCMD);
    free(currentUserDesktopEnvCMD);
    free(availableUserDesktopEnvCMD);
    free(setUserDesktopEnvCMD);
    free(getSystemBasicInfoCMD);
    free(shutdownCMD);
    free(sleepCMD);
    free(restartCMD);
    free(dateTimeCMD);
    free(calenderCMD);
    free(cpuStatusCMD);
    free(networkStatusCMD);
}


void CONFIG::error_default_lang(){
    capslock = strdup("capslock");
    err_alloc = strdup("failed memory allocation");
    err_bounds = strdup("out-of-bounds index");
    err_chdir = strdup("failed to open home folder");
    err_console_dev = strdup("failed to access console");
    err_dgn_oob = strdup("log message");
    err_domain = strdup("invalid domain");
    err_hostname = strdup("failed to get hostname");
    err_mlock = strdup("failed to lock password memory");
    err_null = strdup("null pointer");
    err_pam = strdup("pam transaction failed");
    err_pam_abort = strdup("pam transaction aborted");
    err_pam_acct_expired = strdup("account expired");
    err_pam_auth = strdup("authentication error");
    err_pam_authinfo_unavail = strdup("failed to get user info");
    err_pam_authok_reqd = strdup("token expired");
    err_pam_buf = strdup("memory buffer error");
    err_pam_cred_err = strdup("failed to set credentials");
    err_pam_cred_expired = strdup("credentials expired");
    err_pam_cred_insufficient = strdup("insufficient credentials");
    err_pam_cred_unavail = strdup("failed to get credentials");
    err_pam_maxtries = strdup("reached maximum tries limit");
    err_pam_perm_denied = strdup("permission denied");
    err_pam_session = strdup("session error");
    err_pam_sys = strdup("system error");
    err_pam_user_unknown = strdup("unknown user");
    err_path = strdup("failed to set path");
    err_perm_dir = strdup("failed to change current directory");
    err_perm_group = strdup("failed to downgrade group permissions");
    err_perm_user = strdup("failed to downgrade user permissions");
    err_pwnam = strdup("failed to get user info");
    err_user_gid = strdup("failed to set user GID");
    err_user_init = strdup("failed to initialize user");
    err_user_uid = strdup("failed to set user UID");
    err_xsessions_dir = strdup("failed to find sessions folder");
    err_xsessions_open = strdup("failed to open sessions folder");
}

void CONFIG::load_default_lang(){

    // Language Variable Config
    default_text = strdup("Default");
    loginBTN_text = strdup(" LOGIN ");
    logoutBTN_text = strdup(" LOGOUT ");
    powerList_text = strdup("Sleep\7Restart\7Shutdown\7");
    utilitiesList_text = strdup("Calender\7CPU Status\7Network Status\7Refresh\7Exit\7");
    powerBTN_text = strdup("Power");
    utilitiesBTN_text = strdup("Utilities");
    desktopENVBTN_text = strdup("ENV");
    availableUserDesktopEnv = default_text;
    currentDesktopENV_text = default_text;
    currentUserDesktopEnvComProtocol =  strdup("Default\7shell\7xinitrc\7Xorg\7wayland\7");
    usernameFieldID_text = strdup("USER");
    userpassFieldID_text = strdup("PASS");

};

void CONFIG::load_default_keyValues(){
    // Default KeyValues
    /*KEY_ESCAPE = '\x1b';
    asciiColors[] = {
        COLOR_BLACK,
        COLOR_GREEN,
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_WHITE,
        COLOR_MAGENTA,
        COLOR_BLUE,
        COLOR_CYAN
    };
    totalASCIIcolors = sizeof(asciiColors)/sizeof(asciiColors[0]);*/
    // availableUserDesktopEnvComProtocol = strdup("xsessions\7wayland-sessions\7shell\7");
    totalManualColors = 8;
    totalRandomizedColors = 10;
    tty = 2;
    path = strdup("/usr/local/sbin:/usr/local/bin:/usr/bin");
    term_reset_cmd = strdup("/usr/bin/tput reset");
    wayland_cmd = strdup(DATADIR "/wsetup.sh");
    waylandsessions = strdup("/usr/share/wayland-sessions");
    x_cmd = strdup("/usr/bin/X");
    xinitrc = strdup("~/.xinitrc");
    x_cmd_setup = strdup(DATADIR "/xsetup.sh");
    xauth_cmd = strdup("/usr/bin/xauth");
    xsessions = strdup("/usr/share/xsessions");
    mcookie_cmd = strdup("/usr/bin/mcookie");
    console_dev = strdup("/dev/console");

    // SESSION_KEY = nullptr;
    // SESSION_KEY_LENGTH = 32;
    maxTitleBarItemTreeDepth = 2;
    titleBarHoverableItemCount = 4;
    // char** titleBarSubItems;
    // char** titleBarSubItemsCMD;
    // usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    // userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    maxUsernameLen = 255;
    maxUserpassLen = 255;
    visibleAuthStrLen = 50;
};

void CONFIG::load_default_alertText(){
    // Alert Text
    data_handler.flatKeyValue(loginFailed_text, '\6', "LOGIN Failed", "Access Rejected");
    data_handler.flatKeyValue(loginSuccess_text, '\6', "LOGIN Success", "Access Granted");
    data_handler.flatKeyValue(logoutFailed_text, '\6', "LOGOUT Failed", "Access Rejected");
    data_handler.flatKeyValue(logoutSuccess_text, '\6', "LOGOUT Success", "Good Bye :)");
    data_handler.flatKeyValue(emptyCredPassed, '\6', "LOGIN Failed", "Empty Credentials Passed");
    data_handler.flatKeyValue(incorrectCred, '\6', "LOGIN Failed", "Incorrect Credentials");
};


void CONFIG::load_default_CMD(){
    // CMD Varables; // Specifically for Debian
    // Variable Identifier that is to be replaced [Format : $[<variable>]$]
    //                                            [ Requires 4 variable for position identification $,[,],$
    if(distro==DEBIAN_DISTRO){
        getAvailableUsernameCMD = strdup("awk -F: '$7 !~ /\\/nologin/ && $7 !~ /\\/bin\\/false/ {print $1}' /etc/passwd | tr -s '\n' '\7'");
        uuidCMD = strdup("getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f3 | tr -d '\n'");
        usrGroupCMD = strdup("getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f4 | tr -d '\n'");
        getUserFullnameCMD = strdup("getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f5 | tr -d '\n'");
        usrHomeDirCMD = strdup("getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f6 | tr -s '\n' '/'");
        usrShellCMD = strdup("getent passwd $[USER]$ | grep -v '/nologin' | cut -d ':' -f7 | tr -d '\n'");
        getSystemUnameCMD = strdup("uname -n -o");
        currentUserDesktopEnvCMD = strdup("sudo cat /var/lib/AccountsService/users/$[USER]$ 2>/dev/null | grep 'XSe*' | cut -d '=' -f 2");
    //availableUserDesktopEnvCMD = data_handler.cpArray(availableUserDesktopEnvCMD, "{ls /usr/share/xsessions & ls /usr/share/wayland-sessions} | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'");
    //availableUserDesktopEnvCMD = data_handler.cpArray(availableUserDesktopEnvCMD, "ls /usr/share/xsessions | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'");
    // availableUserDesktopEnvCMD = data_handler.cpArray(availableUserDesktopEnvCMD, "ls $[Xprotocol]$ 2>/dev/null | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'");
        availableUserDesktopEnvCMD = strdup("ls $[Xprotocol]$ 2>/dev/null | rev | cut -d '.' -f 2 | rev | tr -s '\n' '\7'");
    //setUserDesktopEnvCMD = data_handler.cpArray(setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@;' > /home/$[USER]$/.xsessionrc");
        setUserDesktopEnvCMD = strdup("cat $[Xprotocol]$/$[ENV]$.* 2>/dev/null | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@;' > /home/$[USER]$/.xsessionrc");
        getUserDesktopEnvCMD = strdup("head -n 1 /home/$[USER]$/.xsessionrc 2>/dev/null | tr -d '\n'");
        setUserDesktopEnvTypeCMD = strdup("echo $[xsessiontype]$ > /home/$[USER]$/.xsessiontype");
        getUserDesktopEnvTypeCMD = strdup("head -n 1 /home/$[USER]$/.xsessiontype 2>/dev/null | tr -d '\n'");
    // setUserDesktopEnvCMD = data_handler.cpArray(setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@;'");
    // saveUserDesktopEnvCMD = data_handler.cpArray(saveUserDesktopEnvCMD, "echo $[xsessionCMD]$ > /home/$[USER]$/.xsessionrc");
    // setUserDesktopEnvCMD = data_handler.cpArray(setUserDesktopEnvCMD, "cat /usr/share/xsessions/$[ENV]$.* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@; 1s@^@exec @' > /home/$[USER]$/.xsessionrc");
        getSystemBasicInfoCMD = strdup("cat /etc/os-release | grep -w -E 'NAME=|VERSION=' | cut -d '=' -f 2 | cut -d '\"' -f 2 | tr -d '\n'");
        shutdownCMD = strdup("echo Shutting Down System && sudo /usr/sbin/shutdown -h now");
        sleepCMD = strdup("echo System going to Sleep && sudo systemctl suspend");
        restartCMD = strdup("echo Restarting System && sudo shutdown -r now");
        dateTimeCMD = strdup("/usr/bin/date | tr -s '\n' ' '");
        calenderCMD = strdup("/usr/bin/cal");
        cpuStatusCMD = strdup("/usr/bin/mpstat -P ALL");
        networkStatusCMD = strdup("tcpdump --list-interfaces");

    }
    refresh = 0;
    exit = 0;
};
