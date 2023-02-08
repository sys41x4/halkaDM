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
/*
void SESSION_MANAGEMENT::createSession(char* currentDesktopENV, char* usrHomeDir, char* username){


    // std::replace(usrHomeDir, usrHomeDir + strlen(usrHomeDir), '\7', '/');

    // createSessionKey(SESSION_KEY_LENGTH-1, SESSION_KEY);
    char cmd[250] = {'\0'};

    //memset(cmd, '\0', sizeof(cmd)); // Clear cmd char* array
    //for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}
    // Create Session Directory
    char envSource[] = "env_source";
    // char cmd[200] = "mkdir -p ";
    strcpy(cmd, "mkdir -p ");
    strcat(cmd, usrHomeDir);
    strcat(cmd, ".halkaDM/");
    //strcat(cmd, SESSION_KEY);
    strcat(cmd, " && echo 'NEW_USER=");
    strcat(cmd, username);
    strcat(cmd, "&&SESSION=");
    strcat(cmd, currentDesktopENV);
    strcat(cmd, "' > ");
    strcat(cmd, usrHomeDir);
    strcat(cmd, ".halkaDM/");
    //strcat(cmd, SESSION_KEY);
    //strcat(cmd, "/");
    strcat(cmd, envSource);
    // drawCMDStr(mainScreenWin, winMaxY-5, winMaxX-(strlen(usrHomeDir)+2), 1, 0, 0, 13, cmd);
    //execCMD(cmd);
    cmd_executor.exec(cmd);
    //}
    for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}

    // Create .xsessionrc file entry
    strcpy(cmd, "cat /usr/share/xsessions/");
    strcat(cmd, currentDesktopENV);
    strcat(cmd, ".* | grep -E -m 1 '^Exec\\s*=' | sed '1s@^Exec\\s*=\\s*@@; 1s@^@exec @' > /home/");
    strcat(cmd, username);
    strcat(cmd, "/.Xsession");
    // cat /usr/share/xsessions/$SESSION.* | grep -E -m 1 '^Exec\s*=' | sed '1s/^Exec\s*=\s//; 1s/^/exec /' > /home/$NEWUSER/.xsession
    // execCMD(cmd);
    cmd_executor.exec(cmd);
    for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}

    // Create .xinitrc file entry


}
*/

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


bool initiateSession(const char* username, const char* userpass){

    if(user.usernameVerified){
    char* cmd=nullptr;char* cmd2=nullptr;
    int trackID=-1;
    if(user.XDG_SESSION_TYPE==DS_DEFAULT){

       //cmd = data_handler.replaceStr(config.getUserDesktopEnvTypeCMD, "$[USER]$", user.username);
       cmd = data_handler.replaceStr(config.getUserDesktopEnvTypeCMD, "$[USER]$", username);
       user.XDG_SESSION_TYPE_NAME = cmd_executor.fetchExecOutput(cmd);
       std::free(cmd);cmd=nullptr;

       user.XDG_SESSION_TYPE = data_handler.getItemID('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE_NAME);
       trackID=user.XDG_SESSION_TYPE;
       if(user.XDG_SESSION_TYPE==-1 || user.XDG_SESSION_TYPE==DS_DEFAULT){
           user.XDG_SESSION_TYPE=DS_XORG;
           user.XDG_SESSION_TYPE_NAME=data_handler.getItemName('\7', config.currentUserDesktopEnvComProtocol, DS_XORG, user.XDG_SESSION_TYPE_NAME);
           trackID=-1;
       }
    }

    if(user.XDG_SESSION_TYPE!=DS_DEFAULT && trackID==-1){
        cmd = data_handler.replaceStr(config.setUserDesktopEnvTypeCMD, "$[xsessiontype]$", user.XDG_SESSION_TYPE_NAME);
        cmd2=strdup(cmd);std::free(cmd);
//        cmd = data_handler.replaceStr(cmd2, "$[USER]$", user.username);
        cmd = data_handler.replaceStr(cmd2, "$[USER]$", username);
        std::free(cmd2);cmd2=nullptr;
        cmd_executor.exec(cmd);
        std::free(cmd);cmd=nullptr;
    }

    trackID=-1;

/*    if(user.XDG_SESSION_TYPE!=DS_SHELL && strcmp(user.XDG_SESSION_NAME, config.default_text)){


        if(user.XDG_SESSION_TYPE==DS_XINITRC || user.XDG_SESSION_TYPE==DS_XORG){
            //cmd = data_handler.replaceStr(config.setUserDesktopEnvCMD, "$[", "]$", "Xprotocol", config.xsessions);
            cmd = data_handler.replaceStr(config.setUserDesktopEnvCMD, "$[Xprotocol]$", config.xsessions);
        }
        else if(user.XDG_SESSION_TYPE==DS_WAYLAND){
            cmd = data_handler.replaceStr(config.setUserDesktopEnvCMD, "$[Xprotocol]$", config.waylandsessions);
        }
        cmd2=strdup(cmd);std::free(cmd);
        cmd = data_handler.replaceStr(cmd2, "$[ENV]$", user.XDG_SESSION_NAME);
        std::free(cmd2);
        cmd2 = strdup(cmd);std::free(cmd);
        //cmd = data_handler.replaceStr(cmd2, "$[USER]$", user.username);
        cmd = data_handler.replaceStr(cmd2, "$[USER]$", username);
        std::free(cmd2);cmd2=nullptr;
        cmd_executor.exec(cmd);
        std::free(cmd);cmd=nullptr;
    }
*/

    if(user.XDG_SESSION_TYPE!=DS_SHELL && strcmp(user.XDG_SESSION_NAME, config.default_text)){


        cmd = data_handler.replaceStr(config.setUserDesktopEnvCMD, "$[ENV]$", user.XDG_SESSION_NAME);
        cmd2 = data_handler.replaceStr(cmd, "$[USER]$", username);
        std::free(cmd);cmd=nullptr;
        cmd_executor.exec(cmd2);
        std::free(cmd2);cmd2=nullptr;
    }

    std::free(user.desktop_cmd);user.desktop_cmd=nullptr;
//    cmd = data_handler.replaceStr(config.getUserDesktopEnvCMD, "$[USER]$", user.username);
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
    // Testing
/*    std::free(user.XDG_SESSION_NAME);std::free(user.XDG_SESSION_TYPE_NAME);
    user.XDG_SESSION_NAME=nullptr;user.XDG_SESSION_TYPE_NAME=nullptr;
    user.XDG_SESSION_NAME=strdup(config.default_text);user.XDG_SESSION_TYPE_NAME=strdup(config.default_text);
    user.XDG_SESSION_TYPE=DS_DEFAULT;
    return 1;
*/

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

//    DM_PAMAuth dm_PAMAuth;
//    DM_PAMAuth *dm_PAMAuth = new DM_PAMAuth;
/*
    // Add Values to required fields at DM_PAMAuth
    dm_PAMAuth.allocate();
    dm_PAMAuth.XDG_SESSION_TYPE = user.XDG_SESSION_TYPE;
    dm_PAMAuth.tty = config.tty;
    dm_PAMAuth.desktop_cmd = strdup(user.desktop_cmd);
    dm_PAMAuth.desktop_name = strdup(user.desktop_name);
    dm_PAMAuth.wayland_cmd = strdup(config.wayland_cmd);
    dm_PAMAuth.xauth_cmd = strdup(config.xauth_cmd);
    dm_PAMAuth.mcookie_cmd = strdup(config.mcookie_cmd);
    dm_PAMAuth.x_cmd = strdup(config.x_cmd);
    dm_PAMAuth.x_cmd_setup = strdup(config.x_cmd_setup);
    dm_PAMAuth.service_name = strdup(config.service_name);
    dm_PAMAuth.path = strdup(config.path);
    dm_PAMAuth.term_reset_cmd = strdup(config.term_reset_cmd);
    dm_PAMAuth.username = strdup(username);
    dm_PAMAuth.password = strdup(userpass);

*/    
    // After Assigning all the required variables required to create session
    // free up spaces and memory to user.* and config.*

    return 1;
/*

    //bool login_status = login(username, userpass);
//    bool login_status = dm_PAMAuth.login(dm_PAMAuth.username, dm_PAMAuth.password);

  //  delete dm_PAMAuth;
    if (login_status) {
    // Wait for child process to finish (wait for logout)

        std::free(user.XDG_SESSION_NAME);std::free(user.XDG_SESSION_TYPE_NAME);
        //user.XDG_SESSION_NAME=nullptr;user.XDG_SESSION_TYPE_NAME=nullptr;
        user.XDG_SESSION_NAME=strdup(config.default_text);user.XDG_SESSION_TYPE_NAME=strdup(config.default_text);
        user.XDG_SESSION_TYPE=DS_DEFAULT;

        return 1;
    } else { return 0;}
*/
    }
    return 0;
//    stop_x_server();
}
