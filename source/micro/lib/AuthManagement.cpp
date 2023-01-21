#include "CMDexecutor.h"
#include "AuthManagement.h"
#include "../lib/dataHandling.h"
#include "../config/config.new.h"

int AUTH_MANAGEMENT::authCheck(char* usrHomeDir, char* username, char* userpass){
    // On Auth Success Create Session File and source file for sourcing
    // Create New Session Key

    if(strlen(username)>0 && strlen(userpass)>0){
        //char* cmd = nullptr;
        char cmd[250] = "getent passwd ";
        strcat(cmd, username);
        strcat(cmd, " | grep -v '/nologin' | cut -d: -f6 | tr -s '\n' '/'");
        // usrHomeDir = storeExecCMD(usrHomeDir, cmd);
        //cmd = data_handler.replaceStr(cmd, config.usrHomeDirCMD, "$[", "]$", "USER", username);
        config.usrHomeDir = cmd_executor.fetchExecOutput(usrHomeDir, cmd);

        for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}
        // Check if any home directory exist for the user & if it is valid
        if(usrHomeDir==nullptr){
            //drawCMDStr(mainScreenWin, 20, 0, 1, 0, 0, 13, "NULL");
            free(usrHomeDir);
            return 0; // Auth Failed
        }
        else{return 1;} // Auth Success
    }
    else{return 0;}
}
