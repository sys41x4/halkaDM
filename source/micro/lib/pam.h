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
    int get_free_display();
    void add_utmp_entry(
        struct utmp *entry,
        const char* username,
        pid_t display_pid);
    void remove_utmp_entry(struct utmp *entry);
    void err(const char* name, struct pam_handle* handle, int result);
    void shell(struct passwd* pwd);
    void xorg(
        struct passwd* pwd,
        const char* vt,
        const char* desktop_cmd);
    void wayland(struct passwd* pwd,const char* desktop_cmd);
    void xauth(const char* display_name, const char* shell, char* pwd);
    void init_env(struct passwd *pw, const char* path);
    void set_env(char *name, char *value, struct pam_handle* handle);
    void env_xdg_session(int display_server);
    void env_xdg(const char* tty_id, const char* desktop_name);
    int end(int last_result, struct pam_handle* handle);

    void allocate();
    void deallocate();

    bool login(const char *username, const char *password);
    bool logout(struct pam_handle* handle);

};

int conv(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr);
// DM_PAMAuth dm_PAMAuth

#endif /* _PAM_H_ */
