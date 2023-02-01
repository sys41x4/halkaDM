#ifndef HALKADM_AUTH_MANAGEMENT_H
#define HALKADM_AUTH_MANAGEMENT_H

#include <locale.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>

class AUTH_MANAGEMENT{
public:
    int chkCharAllowence(char character);

    int authCheck(char* usrHomeDir, char* username, char* userpass);
    int usernameCheck(char* username);
//    struct pam_response *reply;

//    int PAMfuncConversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
//    int chkPAMAuthStatus(char* username, char* password);
};
int PAMfuncConversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
int chkPAMAuthStatus(char* username, char* password);
// AUTH_MANAGEMENT auth_management;

#endif
