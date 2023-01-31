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
    //unsigned char result[MD5_DIGEST_LENGTH];
    //MD5((unsigned char*)arr, strlen(arr), result);

    //for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    //    sprintf(&arr[i*2], "%02x", (unsigned int)result[i]);
    //}
}

void SESSION_MANAGEMENT::createSession(char* currentDesktopENV, char* usrHomeDir, char* username){
        // drawCMDStr(mainScreenWin, 15, 0, 1, 0, 0, 13, usrHomeDir);
    //}


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
    strcat(cmd, "/.xsessionrc");
    // cat /usr/share/xsessions/$SESSION.* | grep -E -m 1 '^Exec\s*=' | sed '1s/^Exec\s*=\s*//; 1s/^/exec /' > /home/$NEWUSER/.xsession
    // execCMD(cmd);
    cmd_executor.exec(cmd);
    for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}

    // Create .xinitrc file entry


}


/*#define DISPLAY      ":1"
#define VT           "vt01"
static bool testing = false;
static pthread_t login_thread;
static pid_t x_server_pid;
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


int initiateSession(char* username, char* userpass){
/*    pid_t child_pid;
    const char *display = DISPLAY;
    const char *vt = VT;
    if (!testing) {
        signal(SIGSEGV, sig_handler);
        signal(SIGTRAP, sig_handler);
        start_x_server(display, vt);
    }
    setenv(strdup("DISPLAY"), display, true);
*/
    char* cmd;

    int trackID=-1;
    if(user.XDG_SESSION_TYPE==DS_DEFAULT){
//       trackID=user.XDG_SESSION_TYPE;
//       cmd = data_handler.replaceStr(cmd, config.setUserDesktopEnvTypeCMD, "$[", "]$", "xsessiontype", user.XDG_SESSION_TYPE_NAME);
       cmd = data_handler.replaceStr(cmd, config.getUserDesktopEnvTypeCMD, "$[", "]$", "USER", user.username);
       user.XDG_SESSION_TYPE_NAME = cmd_executor.fetchExecOutput(user.XDG_SESSION_TYPE_NAME, cmd);
       free(cmd);

/*        cmd = data_handler.replaceStr(cmd, config.setUserDesktopEnvTypeCMD, "$[", "]$", "xsessiontype", user.XDG_SESSION_TYPE_NAME);
        cmd = data_handler.replaceStr(cmd, cmd, "$[", "]$", "USER", user.username);
        cmd_executor.exec(cmd);
        free(cmd);
*/
       user.XDG_SESSION_TYPE = data_handler.getItemID('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE_NAME);
       trackID=user.XDG_SESSION_TYPE;
       if(user.XDG_SESSION_TYPE==-1 || user.XDG_SESSION_TYPE==DS_DEFAULT){
           user.XDG_SESSION_TYPE=DS_XORG;
           user.XDG_SESSION_TYPE_NAME=data_handler.getItemName('\7', config.currentUserDesktopEnvComProtocol, DS_XORG, user.XDG_SESSION_TYPE_NAME);
           trackID=-1;
       }
//       user.XDG_SESSION_TYPE_NAME=data_handler.getItemName('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE, user.XDG_SESSION_TYPE_NAME);
//       trackID=user.XDG_SESSION_TYPE;
    }

    if(user.XDG_SESSION_TYPE!=DS_DEFAULT && trackID==-1){
/*        if(trackID==-1){
           cmd = data_handler.replaceStr(cmd, config.getUserDesktopEnvTypeCMD, "$[", "]$", "USER", user.username);
           user.XDG_SESSION_TYPE_NAME = cmd_executor.fetchExecOutput(user.XDG_SESSION_TYPE_NAME, cmd);
           free(cmd);
        }

        else if(trackID==-2){*/
            cmd = data_handler.replaceStr(cmd, config.setUserDesktopEnvTypeCMD, "$[", "]$", "xsessiontype", user.XDG_SESSION_TYPE_NAME);
            cmd = data_handler.replaceStr(cmd, cmd, "$[", "]$", "USER", user.username);
            cmd_executor.exec(cmd);
            free(cmd);
//        }
    }

    trackID=-1;

    if(strcmp(user.XDG_SESSION_NAME, "Default")){

/*
        // Read From File If it is present in the user's home directory

        cmd = data_handler.replaceStr(cmd, config.getUserDesktopEnvCMD, "$[", "]$", "USER", user.username);
        user.desktop_cmd = cmd_executor.fetchExecOutput(user.desktop_cmd, cmd);
        free(cmd);

        if(strcmp(user.XDG_SESSION_NAME, "Default")==0){
            // There must be a default WM installed which means there must be a failsafe WM installed to handle any issues with primary WM installed, whose name should be fetched from /etc/halkaDM/.xsessionrc.default
            free(user.XDG_SESSION_NAME);
            user.XDG_SESSION_NAME=nullptr;
            user.XDG_SESSION_NAME = strdup("/usr/bin/jwm");

        }

    }
    else{*/
        cmd = data_handler.replaceStr(cmd, config.setUserDesktopEnvCMD, "$[", "]$", "ENV", user.XDG_SESSION_NAME);
        cmd = data_handler.replaceStr(cmd, cmd, "$[", "]$", "USER", user.username);
        cmd_executor.exec(cmd);
        free(cmd);
    }

//    user.XDG_SESSION_TYPE = data_handler.getItemID('\7', config.currentUserDesktopEnvComProtocol, user.XDG_SESSION_TYPE_NAME);
    // If user.XDG_SESSION_TYPE_NAME doesn't match from the file or is set to default then the default XDG_SESSION_TYPE whill be selected which is DS_XORG
//    if(user.XDG_SESSION_TYPE==-1 || user.XDG_SESSION_TYPE==DS_DEFAULT){user.XDG_SESSION_TYPE=DS_XORG;}

    free(user.desktop_cmd);user.desktop_cmd=nullptr;
    cmd = data_handler.replaceStr(cmd, config.getUserDesktopEnvCMD, "$[", "]$", "USER", user.username);
    user.desktop_cmd = cmd_executor.fetchExecOutput(user.desktop_cmd, cmd);
    free(cmd);
//    user.XDG_SESSION_NAME = strdup("Default");
//    if (login(strdup(username), strdup(userpass), &child_pid)) {
    if (login(username, userpass)) {
    // Wait for child process to finish (wait for logout)
//    int status;
//    waitpid(child_pid, &status, 0); // TODO: Handle errors
        // add utmp audit

/*        struct utmp entry;
        add_utmp_entry(&entry, pw->pw_name, pid);

    // wait for the session to stop
        int status;
        waitpid(pid, &status, 0);
        remove_utmp_entry(&entry);
        reset_terminal(pw);
*/
//    logout();
    // stop_x_server();

        free(user.XDG_SESSION_NAME);free(user.XDG_SESSION_TYPE_NAME);
        user.XDG_SESSION_NAME=nullptr;user.XDG_SESSION_TYPE_NAME=nullptr;
        user.XDG_SESSION_NAME=strdup("Default");user.XDG_SESSION_TYPE_NAME=strdup("Default");

        return 1;
    } else { return 0;}
//    stop_x_server();
}
