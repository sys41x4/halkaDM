#include "inputs.h"

void USER::allocate(){

    // Allocate Username Space
    username = static_cast<char*>(std::malloc(config.maxUsernameLen * sizeof(char)));
    visible_username = static_cast<char*>(std::malloc(config.visibleAuthStrLen * sizeof(char)));
    username[0] = '\0';
    visible_username[0] = '\0';

    // Allocate Userpass Space
    userpass = static_cast<char*>(std::malloc(config.maxUserpassLen * sizeof(char)));
    visible_userpass = static_cast<char*>(std::malloc(config.visibleAuthStrLen * sizeof(char)));
    userpass[0] = '\0';
    visible_userpass[0] = '\0';



    allocated=1;
}

void USER::deallocate(){
    free(authType);
    free(username);
    free(userpass);
    free(usernameHash);
    free(visible_username);
    free(visible_userpass);
    free(userFullName);
    free(XDG_SESSION_TYPE_NAME);
    free(XDG_SESSION_NAME);
    free(desktop_name);
    free(desktop_cmd);


    deallocated=1;
}
