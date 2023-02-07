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

    // if(dm_display_visual==DM_REFRESH){null_values();}

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
        currentDesktopENV_text = strdup(default_text);
        availableUserDesktopEnv =  strdup(default_text);


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
        snprintf(currentUserDesktopEnvComProtocol, tmp, "%s\7shell\7xinitrc\7Xorg\7wayland\7", default_text);

        tmp=0;
        configFile.reset();

    }
    catch (const cpptoml::parse_exception& e) {
       load_default_lang();
    }

    // Allocate Keys


    load_default_CMD();
    load_default_keyValues();
    load_default_softwareInfo();
    error_default_lang();
    load_default_alertText();


  //  delete configFile;
}


void CONFIG::deallocate(){

    std::free(capslock);
    std::free(err_alloc);
    std::free(err_bounds);
    std::free(err_chdir);
    std::free(err_console_dev);
    std::free(err_dgn_oob);
    std::free(err_domain);
    std::free(err_hostname);
    std::free(err_mlock);
    std::free(err_null);
    std::free(err_pam);
    std::free(err_pam_abort);
    std::free(err_pam_acct_expired);
    std::free(err_pam_auth);
    std::free(err_pam_authinfo_unavail);
    std::free(err_pam_authok_reqd);
    std::free(err_pam_buf);
    std::free(err_pam_cred_err);
    std::free(err_pam_cred_expired);
    std::free(err_pam_cred_insufficient);
    std::free(err_pam_cred_unavail);
    std::free(err_pam_maxtries);
    std::free(err_pam_perm_denied);
    std::free(err_pam_session);
    std::free(err_pam_sys);
    std::free(err_pam_user_unknown);
    std::free(err_path);
    std::free(err_perm_dir);
    std::free(err_perm_group);
    std::free(err_perm_user);
    std::free(err_pwnam);
    std::free(err_user_gid);
    std::free(err_user_init);
    std::free(err_user_uid);
    std::free(err_xsessions_dir);
    std::free(err_xsessions_open);


    std::free(service_name);
    std::free(path);
    std::free(term_reset_cmd);
    std::free(wayland_cmd);
    std::free(x_cmd_setup);
    std::free(xinitrc);
    std::free(x_cmd);
    std::free(xauth_cmd);
    std::free(xsessions);
    std::free(waylandsessions);
    std::free(mcookie_cmd);
    std::free(console_dev);

    // Language Variable Config

    std::free(loginBTN_text);
    std::free(logoutBTN_text); //
    std::free(shutdownBTN_text);
    std::free(restartBTN_text);//
    std::free(sleepBTN_text);
    std::free(powerList_text);
    std::free(utilitiesList_text);//
    std::free(calenderBTN_text);
    std::free(cpuStatusBTN_text);
    std::free(networkStatusBTN_text);//
    std::free(refreshBTN_text);
    std::free(exitBTN_text);
    std::free(powerBTN_text);
    std::free(utilitiesBTN_text);
    std::free(desktopENVBTN_text);
    std::free(currentDesktopENV_text);
    std::free(usernameFieldID_text);
    std::free(userpassFieldID_text);
    std::free(default_text);

    // Software Info
    std::free(package);

    // Default KeyValues
    std::free(titleBarItemTree);
    std::free(loginColourMatrixConf);
    std::free(SESSION_KEY);
    std::free(uuid);
    std::free(usrGroup);
    std::free(getUserFullname);
    std::free(usrHomeDir);
    std::free(usrShell);
    std::free(getSystemUname);
    std::free(setUserDesktopEnv);
    std::free(currentUserDesktopEnvComProtocol);
    std::free(availableUserDesktopEnvComProtocol);
    std::free(currentUserDesktopEnv);
    std::free(availableUserDesktopEnv);
    std::free(getSystemBasicInfo);
    std::free(shutdown);
    std::free(sleep);
    std::free(restart);
    std::free(dateTime);
    std::free(calender);
    std::free(cpuStatus);
    std::free(networkStatus);

    // Alert Text
    std::free(loginFailed_text);
    std::free(loginSuccess_text);
    std::free(logoutFailed_text);
    std::free(logoutSuccess_text);
    std::free(emptyCredPassed);
    std::free(incorrectCred);

    // CMD Varables;
    std::free(getAvailableUsernameCMD);
    std::free(uuidCMD);
    std::free(usrGroupCMD);
    std::free(getUserFullnameCMD);
    std::free(usrHomeDirCMD);
    std::free(usrShellCMD);
    std::free(getSystemUnameCMD);
    std::free(setUserDesktopEnvCMD);
    std::free(getUserDesktopEnvCMD);
    std::free(setUserDesktopEnvTypeCMD);
    std::free(getUserDesktopEnvTypeCMD);
    std::free(currentUserDesktopEnvCMD);
    std::free(availableUserDesktopEnvCMD);
    std::free(getSystemBasicInfoCMD);
    std::free(shutdownCMD);
    std::free(sleepCMD);
    std::free(restartCMD);
    std::free(dateTimeCMD);
    std::free(calenderCMD);
    std::free(cpuStatusCMD);
    std::free(networkStatusCMD);
    if(dm_display_visual==DM_REFRESH){null_values();}

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
    loginBTN_text = strdup("LOGIN");
    logoutBTN_text = strdup("LOGOUT");
    powerList_text = strdup("Sleep\7Restart\7Shutdown\7");
    utilitiesList_text = strdup("Calender\7CPU Status\7Network Status\7Refresh\7Exit\7");
    powerBTN_text = strdup("Power");
    utilitiesBTN_text = strdup("Utilities");
    desktopENVBTN_text = strdup("ENV");
    availableUserDesktopEnv = strdup(default_text);
    currentDesktopENV_text = strdup(default_text);
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

    // TitleBar Item Tree Depth | Storage Space Allocation
    titleBarItemTree = static_cast<int*>(std::malloc(maxTitleBarItemTreeDepth * sizeof(int)));

    // Allocate space for config list for loginColourMatrix
    loginColourMatrixConf = static_cast<int*>(std::malloc(4 * sizeof(int)));
    // char** titleBarSubItems;
    // char** titleBarSubItemsCMD;
    // usernameVisibilityConf[4] = {0, 0, 0}; // {[-2..1], [0..1], [0..1]}Default username_visibility_config
    // userpassVisibilityConf[4] = {1, 0, 0}; // Default userpass_visibility_config
    //maxUsernameLen = 255;
     maxUsernameLen = 50;
    maxUserpassLen = 255;
    visibleAuthStrLen = 50;
};

void CONFIG::load_default_alertText(){
    // Alert Text

    loginFailed_text = data_handler.flatKeyValue('\6', "LOGIN Failed", "Access Rejected");
    loginSuccess_text = data_handler.flatKeyValue('\6', "LOGIN Success", "Access Granted");
    logoutFailed_text = data_handler.flatKeyValue('\6', "LOGOUT Failed", "Access Rejected");
    logoutSuccess_text = data_handler.flatKeyValue('\6', "LOGOUT Success", "Good Bye :)");
    emptyCredPassed = data_handler.flatKeyValue('\6', "LOGIN Failed", "Empty Credentials Passed");
    incorrectCred = data_handler.flatKeyValue('\6', "LOGIN Failed", "Incorrect Credentials");
/*
    loginFailed_text = strdup("LOGIN Failed\6Access Rejected");
    loginSuccess_text = strdup("LOGIN Success\6Access Granted");
    logoutFailed_text = strdup("LOGOUT Failed\6Access Rejected");
    logoutSuccess_text = strdup("LOGOUT Success\6Good Bye :)");
    emptyCredPassed = strdup("LOGIN Failed\6Empty Credentials Passed");
    incorrectCred = strdup("LOGIN Failed\6Incorrect Credentials");
*/
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
        currentUserDesktopEnvCMD = strdup("sudo cat /var/lib/AccountsService/users/$[USER]$ 2>/dev/null | grep 'XSe*' | cut -d '=' -f 2 | tr -d '\n'");
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


void CONFIG::null_values(){

        capslock=nullptr;
	err_alloc=nullptr;
	err_bounds=nullptr;
	err_chdir=nullptr;
	err_console_dev=nullptr;
	err_dgn_oob=nullptr;
	err_domain=nullptr;
	err_hostname=nullptr;
	err_mlock=nullptr;
	err_null=nullptr;
	err_pam=nullptr;
	err_pam_abort=nullptr;
	err_pam_acct_expired=nullptr;
	err_pam_auth=nullptr;
	err_pam_authinfo_unavail=nullptr;
	err_pam_authok_reqd=nullptr;
	err_pam_buf=nullptr;
	err_pam_cred_err=nullptr;
	err_pam_cred_expired=nullptr;
	err_pam_cred_insufficient=nullptr;
	err_pam_cred_unavail=nullptr;
	err_pam_maxtries=nullptr;
	err_pam_perm_denied=nullptr;
	err_pam_session=nullptr;
	err_pam_sys=nullptr;
	err_pam_user_unknown=nullptr;
	err_path=nullptr;
	err_perm_dir=nullptr;
	err_perm_group=nullptr;
	err_perm_user=nullptr;
	err_pwnam=nullptr;
	err_user_gid=nullptr;
	err_user_init=nullptr;
	err_user_uid=nullptr;
	err_xsessions_dir=nullptr;
	err_xsessions_open=nullptr;

	service_name=nullptr;
	tty = 1;
	path=nullptr;
	term_reset_cmd=nullptr;
	wayland_cmd=nullptr;
	x_cmd_setup=nullptr;
	xinitrc=nullptr;
	x_cmd=nullptr;
	xauth_cmd=nullptr;
	xsessions=nullptr;
	waylandsessions=nullptr;
	mcookie_cmd=nullptr;
	console_dev=nullptr;
	dm_display_visual=DM_START;

	// Language Variable Config

	default_text=nullptr;
	loginBTN_text=nullptr;
	logoutBTN_text=nullptr;
	shutdownBTN_text=nullptr;
	restartBTN_text=nullptr;
	sleepBTN_text=nullptr;
	powerList_text=nullptr;
	utilitiesList_text=nullptr;
	calenderBTN_text=nullptr;
	cpuStatusBTN_text=nullptr;
	networkStatusBTN_text=nullptr;
	refreshBTN_text=nullptr;
	exitBTN_text=nullptr;
	powerBTN_text=nullptr;
	utilitiesBTN_text=nullptr;
	desktopENVBTN_text=nullptr;
	currentDesktopENV_text=nullptr;
	usernameFieldID_text=nullptr;
	userpassFieldID_text=nullptr;

	// Software Info
	package=nullptr;
	SESSION_KEY = nullptr;
        titleBarItemTree=nullptr;
        loginColourMatrixConf=nullptr;

	uuid=nullptr;
	usrGroup=nullptr;
	getUserFullname=nullptr;
	usrHomeDir = nullptr;
	usrShell=nullptr;
	getSystemUname=nullptr;
	setUserDesktopEnv=nullptr;
	currentUserDesktopEnvComProtocol=nullptr;
	availableUserDesktopEnvComProtocol=nullptr;
	currentUserDesktopEnv=nullptr;
	availableUserDesktopEnv=nullptr;
	getSystemBasicInfo=nullptr;
	shutdown=nullptr;
	sleep=nullptr;
	restart=nullptr;
	dateTime=nullptr;
	calender=nullptr;
	cpuStatus=nullptr;
	networkStatus=nullptr;


	// Alert Text
	loginFailed_text=nullptr;
	loginSuccess_text=nullptr;
	logoutFailed_text=nullptr;
	logoutSuccess_text=nullptr;
	emptyCredPassed=nullptr;
	incorrectCred=nullptr;

	// CMD Varables;
	getAvailableUsernameCMD=nullptr;
	uuidCMD=nullptr;
	usrGroupCMD=nullptr;
	getUserFullnameCMD=nullptr;
	usrHomeDirCMD = nullptr;
	usrShellCMD=nullptr;
	getSystemUnameCMD=nullptr;
	setUserDesktopEnvCMD=nullptr;
	getUserDesktopEnvCMD=nullptr;
	//    getDefaultUserDesktopEnvCMD;
	setUserDesktopEnvTypeCMD=nullptr;
	getUserDesktopEnvTypeCMD=nullptr;
	currentUserDesktopEnvCMD=nullptr;
	availableUserDesktopEnvCMD=nullptr;
	getSystemBasicInfoCMD=nullptr;
	shutdownCMD=nullptr;
	sleepCMD=nullptr;
	restartCMD=nullptr;
	dateTimeCMD=nullptr;
	calenderCMD=nullptr;
	cpuStatusCMD=nullptr;
	networkStatusCMD=nullptr;

}
