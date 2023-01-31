#ifndef HALKADM_INPUTS_H
#define HALKADM_INPUTS_H

#define DS_SHELL 0
#define DS_XINITRC 1
#define DS_XORG 2
#define DS_WAYLAND 3

#define DATADIR "/etc/halkaDM/halkaDM.micro"

class USER{
public:
    char* authType;
    char* username=nullptr;
    char* userpass=nullptr;
    char* visible_username=nullptr;
    char* visible_userpass=nullptr;
    char* userFullName=nullptr;

    int XDG_SESSION_TYPE = 2;
    char* XDG_SESSION_TYPE_NAME=strdup("Default");

    char* XDG_SESSION_NAME=strdup("Default");

    char* desktop_name = strdup("DESKTOP1"); // Must be parsed from config file later on;
    char* desktop_cmd = strdup("jwm");


};

USER user;

#endif
