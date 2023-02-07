#ifndef HALKADM_INPUTS_H
#define HALKADM_INPUTS_H

#define DS_DEFAULT 0
#define DS_SHELL 1
#define DS_XINITRC 2
#define DS_XORG 3
#define DS_WAYLAND 4

#define DATADIR "/etc/halkaDM/halkaDM.micro"
#include "../config/config.h"
#include "dataHandling.h"
#include "CMDexecutor.h"
#include "cryptography.h"
#include "security.h"
#include "AuthManagement.h"
#include "sessionManagement.h"
#include "draw.h"


char KEY_ESCAPE = '\x1b';

class USER{
public:
    char* authType=nullptr;

    char* username=nullptr;
    char* userpass=nullptr;

    int usernameVerified=0;

    char* usernameHash=nullptr;

    char* visible_username=nullptr;
    char* visible_userpass=nullptr;

    char* userFullName=nullptr;

    int XDG_SESSION_TYPE = DS_DEFAULT;
    char* XDG_SESSION_TYPE_NAME=nullptr;//strdup("Default");

    char* XDG_SESSION_NAME=nullptr;//strdup("Default");

    char* desktop_name = nullptr;//strdup("DESKTOP1"); // Must be parsed from config file later on;
    char* desktop_cmd = nullptr;//strdup("jwm");

    bool allocated=0;
    bool deallocated=0;


    // Config Values allocation & Deallocation & nullification
    void reset();
    void allocate();
    void deallocate();

};
/*
USER user;

CONFIG config;
DATA data_handler;
CMD_EXECUTOR cmd_executor;
HALKADM_CRYPTO halkadm_crypto;
HALKADM_SECURITY halkadm_security;
DRAW draw;
SESSION_MANAGEMENT session_management;
AUTH_MANAGEMENT auth_management;
*/
#endif
