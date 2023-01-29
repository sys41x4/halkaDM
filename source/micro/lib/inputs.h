#ifndef HALKADM_INPUTS_H
#define HALKADM_INPUTS_H

#define DS_SHELL 0
#define DS_XINITRC 1
#define DS_XORG 2
#define DS_WAYLAND 3

int XDG_SESSION_TYPE = 2; // Must be parsed from config file later on
char* desktop_name = strdup("DESKTOP1"); // Must be parsed from config file later on
char* desktop_cmd = strdup("icewm-session");
#define DATADIR "/etc/halkaDM/halkaDM.micro"


#endif
