#include "CMDexecutor.h"
#include "AuthManagement.h"
#include "../lib/dataHandling.h"
#include "../config/config.h"
#include <security/pam_appl.h>
#include <unistd.h>

int AUTH_MANAGEMENT::authCheck(char* usrHomeDir, char* username, char* userpass){
    // On Auth Success Create Session File and source file for sourcing
    // Create New Session Key

    if(strlen(username)>0 && strlen(userpass)>0){
        //char* cmd = nullptr;
        //char cmd[250] = "getent passwd ";
        //strcat(cmd, username);
        //strcat(cmd, " | grep -v '/nologin' | cut -d: -f6 | tr -s '\n' '/'");

        char* cmd;
        cmd = data_handler.replaceStr(cmd, config.usrHomeDirCMD, "$[", "]$", "USER", username);

        // usrHomeDir = storeExecCMD(usrHomeDir, cmd);
        //cmd = data_handler.replaceStr(cmd, config.usrHomeDirCMD, "$[", "]$", "USER", username);
        config.usrHomeDir = cmd_executor.fetchExecOutput(usrHomeDir, cmd);

        //for(int i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++){cmd[i]='\0';}
        free(cmd);
        // Check if any home directory exist for the user & if it is valid
        if(config.usrHomeDir==nullptr){
            //drawCMDStr(mainScreenWin, 20, 0, 1, 0, 0, 13, "NULL");
//            free(config.usrHomeDir);
            return 0; // Auth Failed
        }
        else{return 1;} // Auth Success
    }
    else{return 0;}
}


int AUTH_MANAGEMENT::chkCharAllowence(char character){
    if(character>=32 && character<=126){
        return 1; // If character is allowed then it will return 1
    }
    return 0; // If character is allowed then it will return 0
}


/*int AUTH_MANAGEMENT::PAMfuncConversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
  *resp = reply;
  return PAM_SUCCESS;
}*/
//struct *reply;
struct pam_response *reply;

int PAMfuncConversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
  *resp = reply;
  return PAM_SUCCESS;
}

//int AUTH_MANAGEMENT::chkPAMAuthStatus(char* username, char* password)
int chkPAMAuthStatus(char* username, char* password)
{
  /*if(argc != 2) {
      fprintf(stderr, "Usage: check_user <username>\n");
      exit(1);
  }
  const char *username;
  username = argv[1];*/

  const struct pam_conv local_conversation = { PAMfuncConversation, NULL };
  pam_handle_t *local_auth_handle = NULL; // this gets set by pam_start

  int retval;

  // local_auth_handle gets set based on the service
  retval = pam_start("common-auth", username, &local_conversation, &local_auth_handle);

  if (retval != PAM_SUCCESS)
  {
    //std::cout << "pam_start returned " << retval << std::endl;
    // exit(retval);
    // draw_charArr(mainScreenWin, winMaxY-2, 0, 13, "Error Starting PAM Service");
    // wrefresh(mainScreenWin);
    return retval;
  }

  reply = (struct pam_response *)malloc(sizeof(struct pam_response));

  // *** Get the password by any method, or maybe it was passed into this function.
  // reply[0].resp = getpass("Password: ");
  reply[0].resp = strdup(password);
  reply[0].resp_retcode = 0;

  retval = pam_authenticate(local_auth_handle, 0);

  if (retval != PAM_SUCCESS)
  {
    if (retval == PAM_AUTH_ERR)
    {
//      std::cout << "Authentication failure." << std::endl;
      // draw_charArr(mainScreenWin, winMaxY-2, 0, 13, "Authentication Failure");
      // wrefresh(mainScreenWin);
      return retval;
    }
    else
    {
     // std::cout << "pam_authenticate returned " << retval << std::endl;
     // draw_charArr(mainScreenWin, winMaxY-2, 0, 13, "pam_authenticate returned");
     // wrefresh(mainScreenWin);
     return retval;
    }
    // exit(retval);
    return retval;
  }

  // std::cout << "Authenticated." << std::endl;
  //draw_charArr(mainScreenWin, winMaxY-2, 0, 13, "Authenticated");
  //wrefresh(mainScreenWin);

  retval = pam_end(local_auth_handle, retval);

  if (retval != PAM_SUCCESS)
  {
//    std::cout << "pam_end returned " << retval << std::endl;
    // draw_charArr(mainScreenWin, winMaxY-3, 0, 13, "pam_end returned");
    // wrefresh(mainScreenWin);
    //exit(retval);
    return retval;
  }

  return retval;
}
