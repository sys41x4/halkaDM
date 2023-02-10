#ifndef HALKADM_SESSION_MANAGEMENT_H
#define HALKADM_SESSION_MANAGEMENT_H

#include <locale.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>

class SESSION_MANAGEMENT{
public:

    void createSessionKey(int len, char* session_key);
    //void createSession(char* currentDesktopENV, char* usrHomeDir, char* username);
    //void startSession();
    void autoDetectSession(const char* username);
    bool initiateSession(const char* username, const char* userpass);
};

#endif
