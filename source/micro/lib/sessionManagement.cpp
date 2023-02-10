#include "CMDexecutor.h"
#include "sessionManagement.h"
#include "inputs.h"
#include "dataHandling.h"
#include "pam.h"
#include "draw.h"
#include <xcb/xcb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <security/pam_appl.h>
#include <utmp.h>
#include <unistd.h>

void SESSION_MANAGEMENT::createSessionKey(int len, char* session_key){
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        // arr[i] = '!'+(rand() % 94);
        session_key[i] = 'a'+(rand() % 25);
    }
}

static void stop_x_server() {
    if (x_server_pid != 0) {
        kill(x_server_pid, SIGKILL);
    }
}

static void sig_handler(int signo) {
    stop_x_server();
}
static void start_x_server(const char *display, const char *vt) {
    x_server_pid = fork();
    if (x_server_pid == 0) {
        char cmd[32];
        snprintf(cmd, sizeof(cmd), "/usr/bin/X %s %s", display, vt);
        execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
        printf("Failed to start X server");
        exit(1);
    } else {
        // TODO: Wait for X server to start
        sleep(1);
    }
}



/*bool startSession(const char* username, const char* password){
    bool login_status = dm_PAMAuth.login(username, password);
    return login_status;
}*/


void SESSION_MANAGEMENT::autoDetectSession(const char* username){
    /*
       First Detect XDG_SESSION_NAME
       Location Series:
           [1] /var/lib/AccountsService/users/$[USER]$ ->XSession->Value
           [2] /home/$[USER]$/.Xsession ->head -n 1->output
           [2] /usr/share/xsessions ->1st DE/WM File
           [3] /usr/share/wayland-sessions ->1st DE/WM File [Not Implemented currently]
           [4] FALLBACK SHELL
    */
    bool xsession_files_present=0, wayland_session_files_present=0;
    int XDG_NAME_id=-1;

    char* cmd=nullptr;
    // First it search the DE/WM XSession Name in /var/lib/AccountsService/users/$[USER]$
    if(user.XDG_SESSION_NAME!=nullptr){std::free(user.XDG_SESSION_NAME);}
    cmd = data_handler.replaceStr(config.currentUserDesktopEnvCMD, "$[USER]$", username);
    user.XDG_SESSION_NAME = cmd_executor.fetchExecOutput(cmd);
    std::free(cmd);cmd=nullptr;
    user.XDG_SESSION_TYPE=DS_DEFAULT;

    if(user.XDG_SESSION_NAME==nullptr){
    // Assuming that the user has minimum 1 Xorg DE/WM installed in the system
    // and the file located at /home/$[USER]$/.Xsession has it's name mentioned

        cmd = data_handler.replaceStr(config.getUserDesktopEnv, "$[USER]$", username);
        user.XDG_SESSION_NAME = cmd_executor.fetchExecOutput(cmd);
        std::free(cmd);cmd=nullptr;
        user.XDG_SESSION_TYPE=DS_DEFAULT;
    }




    // 1st Detect the file in /usr/share/xsessions
    cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[Xprotocol]$", config.xsessions);
    if(config.availableUserDesktopEnv!=nullptr){std::free(config.availableUserDesktopEnv);config.availableUserDesktopEnv=nullptr;}
    config.availableUserDesktopEnv = cmd_executor.fetchExecOutput(cmd);
    std::free(cmd);cmd=nullptr;
    if(config.availableUserDesktopEnv!=nullptr){
        xsession_files_present=1;
    }
    if(xsession_files_present==1 && user.XDG_SESSION_NAME!=nullptr){
        XDG_NAME_id = data_handler.getItemID('\7', config.availableUserDesktopEnv, user.XDG_SESSION_NAME);
        user.XDG_SESSION_TYPE=DS_XORG;
    }

    //if 1st causes any error then use /usr/share/wayland-sessions
    if(XDG_NAME_id==-1){
        cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[Xprotocol]$", config.waylandsessions);
        if(config.availableUserDesktopEnv!=nullptr){std::free(config.availableUserDesktopEnv);config.availableUserDesktopEnv=nullptr;}
        config.availableUserDesktopEnv = cmd_executor.fetchExecOutput(cmd);
        std::free(cmd);cmd=nullptr;
        if(config.availableUserDesktopEnv!=nullptr){
            wayland_session_files_present=1;
        }
        if(wayland_session_files_present==1 && user.XDG_SESSION_NAME!=nullptr){
           XDG_NAME_id = data_handler.getItemID('\7', config.availableUserDesktopEnv, user.XDG_SESSION_NAME);
           user.XDG_SESSION_TYPE=DS_WAYLAND;
        }
    }

    if(XDG_NAME_id==-1 && user.XDG_SESSION_NAME!=nullptr){
        user.XDG_SESSION_TYPE=DS_SHELL;
        std::free(user.XDG_SESSION_NAME);user.XDG_SESSION_NAME=nullptr;
    }


    if(user.XDG_SESSION_NAME==nullptr && xsession_files_present==1){
        // Assuming that the user has minimum 1 Xorg DE/WM installed in the system
        // and the first file will be selected from /usr/share/xsessions
        // Otherwise the session will be initiated with Shell config and will start the user using basic fallback shell

        cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[Xprotocol]$", config.xsessions);
        if(config.availableUserDesktopEnv!=nullptr){free(config.availableUserDesktopEnv);config.availableUserDesktopEnv=nullptr;}
        config.availableUserDesktopEnv = cmd_executor.fetchExecOutput(cmd);
        std::free(cmd);cmd=nullptr;
        user.XDG_SESSION_NAME = data_handler.getItemName('\7', config.availableUserDesktopEnv, 0);
        user.XDG_SESSION_TYPE=DS_XORG;
    }

    if(user.XDG_SESSION_NAME==nullptr && wayland_session_files_present==1){
        // Assuming that the user has minimum 1 Xorg DE/WM installed in the system
        // and the first file will be selected from /usr/share/wayland-sessions
        // Otherwise the session will be initiated with Shell config and will start the user using basic fallback shell
        cmd = data_handler.replaceStr(config.availableUserDesktopEnvCMD, "$[Xprotocol]$", config.waylandsessions);
        if(config.availableUserDesktopEnv!=nullptr){free(config.availableUserDesktopEnv);config.availableUserDesktopEnv=nullptr;}
        config.availableUserDesktopEnv = cmd_executor.fetchExecOutput(cmd);
        std::free(cmd);cmd=nullptr;
        user.XDG_SESSION_NAME = data_handler.getItemName('\7', config.availableUserDesktopEnv, 0);
        user.XDG_SESSION_TYPE=DS_WAYLAND;
    }

    if(user.XDG_SESSION_NAME==nullptr){
            user.XDG_SESSION_NAME=strdup(config.default_text);
            user.XDG_SESSION_TYPE=DS_SHELL;
    }

    // At This point XDG_SESSION_NAME & XDG_SESSION_TYPE will be automatically Detected. Now we are required to detect the XDG_SESSION_TYPE_NAME //

    if(user.XDG_SESSION_TYPE_NAME!=nullptr){std::free(user.XDG_SESSION_TYPE_NAME);user.XDG_SESSION_TYPE_NAME=nullptr;}
    user.XDG_SESSION_TYPE_NAME = data_handler.getItemName('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE);
    if(cmd!=nullptr){std::free(cmd);}
//    return true;
}



bool SESSION_MANAGEMENT::initiateSession(const char* username, const char* userpass){

    if(user.usernameVerified){
    char* cmd=nullptr;char* cmd2=nullptr;


    if(user.XDG_SESSION_TYPE!=DS_SHELL && strcmp(user.XDG_SESSION_NAME, config.default_text)==0){
    /* Assuming that user.XDG_SESSION_TYPE has been set but
       user.XDG_SESSION_NAME set to Default in some reason
       Then the default XDG_SESSION of user is auto detected and selected
    */
         autoDetectSession(username);
    }


    if(user.XDG_SESSION_TYPE!=DS_SHELL && strcmp(user.XDG_SESSION_NAME, config.default_text)){
        // If user.XDG_SESSION_TYPE!=DS_SHELL && user.XDG_SESSION_NAME!=Default then output the user.XDG_SESSION_NAME at /home/$[USER]$/.Xsession

        cmd = data_handler.replaceStr(config.setUserDesktopEnvCMD, "$[ENV]$", user.XDG_SESSION_NAME);
        cmd2 = data_handler.replaceStr(cmd, "$[USER]$", username);
        std::free(cmd);cmd=nullptr;
        cmd_executor.exec(cmd2);
        std::free(cmd2);cmd2=nullptr;
    }


    if(user.desktop_cmd!=nullptr){std::free(user.desktop_cmd);user.desktop_cmd=nullptr;}

    if(user.XDG_SESSION_TYPE!=DS_SHELL && user.XDG_SESSION_TYPE!=DS_DEFAULT){

        if(user.XDG_SESSION_TYPE==DS_XINITRC || user.XDG_SESSION_TYPE==DS_XORG){
            cmd = data_handler.replaceStr(config.getUserDesktopEnvCMD, "$[Xprotocol]$", config.xsessions);
        }
        else if(user.XDG_SESSION_TYPE==DS_WAYLAND){
            cmd = data_handler.replaceStr(config.getUserDesktopEnvCMD, "$[Xprotocol]$", config.waylandsessions);
        }

        cmd2 = data_handler.replaceStr(cmd, "$[ENV]$", user.XDG_SESSION_NAME);
        std::free(cmd);cmd=nullptr;
        user.desktop_cmd = cmd_executor.fetchExecOutput(cmd2);
        std::free(cmd2);cmd2=nullptr;
    }
    std::free(cmd);std::free(cmd2);

    // Required Variables during login
    // user.XDG_SESSION_TYPE
    // user.desktop_cmd
    // user.desktop_name
    // config.tty
    // config.wayland_cmd
    // config.xauth_cmd
    // config.mcookie_cmd
    // config.x_cmd
    // config.x_cmd_setup
    // config.service_name
    // config.path
    // config.term_reset_cmd
    // user.username
    // user.password

    // After Assigning all the required variables required to create session
    // free up memory spaces of user.* and config.* as well as other unnecessary items

    return 1;
    }
    return 0;
//    stop_x_server();
}
