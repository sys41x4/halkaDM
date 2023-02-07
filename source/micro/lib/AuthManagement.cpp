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

        //char* cmd=nullptr;
        char* cmd = data_handler.replaceStr(config.usrHomeDirCMD, "$[USER]$", username);

        config.usrHomeDir = cmd_executor.fetchExecOutput(config.usrHomeDir, cmd);

        std::free(cmd);
        if(config.usrHomeDir==nullptr){
            return 0; // Auth Failed
        }
        else{return 1;} // Auth Success
    }
    else{return 0;}
}



int AUTH_MANAGEMENT::usernameCheck(const char* username){

    /*return 1 : If username matches
      return 0 : If username doesn't match*/
    if(username==nullptr){return 0;}
    char* availableUsername=cmd_executor.fetchExecOutput(config.getAvailableUsernameCMD);
    //availableUsername = strdup("root\7sync\7sys41x4\7");
    int status = data_handler.getItemID('\7', availableUsername, username);
    std::free(availableUsername);//availableUsername=nullptr;delete availableUsername;
//    return 1;
    if(status!=-1){return 1;}
    return 0;
}

bool AUTH_MANAGEMENT::chkCharAllowence(char character){
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

int PAMfuncConversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
  if (num_msg != 1) {
    return PAM_CONV_ERR;
  }

  struct pam_response *reply = (struct pam_response *)calloc(1, sizeof(struct pam_response));
  if (!reply) {
    return PAM_BUF_ERR;
  }

  reply[0].resp = strdup(static_cast<const char *>(appdata_ptr));
  reply[0].resp_retcode = 0;

  *resp = reply;
  return PAM_SUCCESS;
}

int chkPAMAuthStatus(const char* username, const char* password)
{
  const struct pam_conv local_conversation = { PAMfuncConversation, const_cast<char *>(password) };
  pam_handle_t *local_auth_handle = NULL;

  int retval;

  retval = pam_start("common-auth", username, &local_conversation, &local_auth_handle);
  if (retval != PAM_SUCCESS) {
    return retval;
  }

  retval = pam_authenticate(local_auth_handle, 0);
  if (retval != PAM_SUCCESS) {
    pam_end(local_auth_handle, retval);
    return retval;
  }

  retval = pam_end(local_auth_handle, retval);
  return retval;
}
