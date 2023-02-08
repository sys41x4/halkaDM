#ifndef _PAM_H_
#define _PAM_H_

#include <stdbool.h>
#include "inputs.h"
class DM_PAMAuth{

public:
    int XDG_SESSION_TYPE=DS_DEFAULT;
    int tty=2;
    char* desktop_name=nullptr;
    char* desktop_cmd=nullptr;
    char* wayland_cmd=nullptr;
    char* xauth_cmd=nullptr;
    char* mcookie_cmd=nullptr;
    char* x_cmd=nullptr;
    char* x_cmd_setup=nullptr;
    char* service_name=nullptr;
    char* path=nullptr;
    char* term_reset_cmd=nullptr;
    char* username=nullptr;
    char* password=nullptr;


    void reset_terminal(struct passwd* pw);
//    static void init_env(struct passwd *pw);
    void xorg(
        struct passwd* pwd,
        const char* vt,
        const char* desktop_cmd);
    void wayland(struct passwd* pwd,const char* desktop_cmd);
    void xauth(const char* display_name, const char* shell, char* pwd);


    void allocate();
    void deallocate();

    bool login(const char *username, const char *password);
    bool logout(void);

};

static void init_env(struct passwd *pw, const char* path);

// DM_PAMAuth dm_PAMAuth

#endif /* _PAM_H_ */
