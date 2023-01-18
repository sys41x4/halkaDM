#include "CMDexecutor.h"
#include "sessionManagement.h"

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
