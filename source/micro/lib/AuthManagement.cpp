#include "CMDexecutor.h"
#include "AuthManagement.h"
#include "dataHandling.h"
#include "../config/config.h"
#include "CMDexecutor.h"
#include <security/pam_appl.h>
#include <unistd.h>


int AUTH_MANAGEMENT::authCheck(char* usrHomeDir, char* username, char* userpass){
    // On Auth Success Create Session File and source file for sourcing
    // Create New Session Key

    if(strlen(username)>0 && strlen(userpass)>0){

        char* cmd=nullptr;
        cmd = data_handler.replaceStr(config.usrHomeDirCMD, "$[", "]$", "USER", username);

        config.usrHomeDir = cmd_executor.fetchExecOutput(usrHomeDir, cmd);

        free(cmd);
        if(config.usrHomeDir==nullptr){
            return 0; // Auth Failed
        }
        else{return 1;} // Auth Success
    }
    else{return 0;}
}



int AUTH_MANAGEMENT::usernameCheck(char* username){

    /*return 1 : If username matches
      return 0 : If username doesn't match*/
    char* availableUsername=nullptr;
    availableUsername = cmd_executor.fetchExecOutput(availableUsername, config.getAvailableUsernameCMD);
    int status = data_handler.getItemID('\7', availableUsername, username);
    free(availableUsername);//availableUsername=nullptr;delete availableUsername;
    if(status!=-1){return 1;}
    return 0;
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
int chkPAMAuthStatus(char* user, char* password)
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

  char* username = strdup(user);

  // local_auth_handle gets set based on the service
  retval = pam_start("common-auth", username, &local_conversation, &local_auth_handle);

  if (retval != PAM_SUCCESS)
  {
    //std::cout << "pam_start returned " << retval << std::endl;
    // exit(retval);
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
      return retval;
    }
    else
    {
     // std::cout << "pam_authenticate returned " << retval << std::endl;
     return retval;
    }
    // exit(retval);
    return retval;
  }

  // std::cout << "Authenticated." << std::endl;

  retval = pam_end(local_auth_handle, retval);

  if (retval != PAM_SUCCESS)
  {
//    std::cout << "pam_end returned " << retval << std::endl;
    return retval;
  }
  free(username);//username=nullptr;delete username;
  return retval;
}
