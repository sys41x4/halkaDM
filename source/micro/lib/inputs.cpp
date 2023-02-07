#include "inputs.h"
#include "../config/config.h"

void USER::allocate(){

//    if(config.dm_display_visual==DM_REFRESH){reset();}


    XDG_SESSION_TYPE_NAME=strdup(config.default_text);
    XDG_SESSION_NAME=strdup(config.default_text);
    desktop_name = strdup("DESKTOP1"); // Must be parsed from config file later on;
    desktop_cmd = strdup("jwm");


    // Allocate Username Space
    username = static_cast<char*>(std::malloc(config.maxUsernameLen+1 * sizeof(char)));
    if(username!=nullptr){
        //memset(username, '\0', configmaxUsernameLen);
        halkadm_crypto.maskStr(config.maxUsernameLen, '\0', username);
        username[0] = '\0';username[config.maxUsernameLen]='\0';
    }

    visible_username = static_cast<char*>(std::malloc(config.visibleAuthStrLen+1 * sizeof(char)));
    if(visible_username!=nullptr){
        //memset(visible_username, '\0', config.visibleAuthStrLen);
        halkadm_crypto.maskStr(config.visibleAuthStrLen, '\0', visible_username);
        visible_username[0] = '\0';visible_username[config.visibleAuthStrLen]='\0';
    }
    // Allocate Userpass Space
    userpass = static_cast<char*>(std::malloc(config.maxUserpassLen+1 * sizeof(char)));
    if(userpass!=nullptr){
        halkadm_crypto.maskStr(config.maxUserpassLen, '\0', userpass);
        //memset(userpass, '\0', config.maxUserpassLen);
        userpass[0]='\0';userpass[config.maxUserpassLen]='\0';
    }
    visible_userpass = static_cast<char*>(std::malloc(config.visibleAuthStrLen+1 * sizeof(char)));
    if(visible_userpass!=nullptr){
        halkadm_crypto.maskStr(config.visibleAuthStrLen, '\0', visible_userpass);
//        memset(visible_userpass, '\0', config.visibleAuthStrLen);
        visible_userpass[0]='\0';visible_userpass[config.visibleAuthStrLen]='\0';
    }



    allocated=1;
}

void USER::deallocate(){
    std::free(authType);
    std::free(username);
    std::free(userpass);
    std::free(usernameHash);
    std::free(visible_username);
    std::free(visible_userpass);
    std::free(userFullName);
    std::free(XDG_SESSION_TYPE_NAME);
    std::free(XDG_SESSION_NAME);
    std::free(desktop_name);
    std::free(desktop_cmd);

    if(config.dm_display_visual==DM_REFRESH){reset();}
    deallocated=1;
}

void USER::reset(){
    authType=nullptr;
    username=nullptr;
    userpass=nullptr;
    usernameVerified=0;
    usernameHash=nullptr;
    visible_username=nullptr;
    visible_userpass=nullptr;
    userFullName=nullptr;
    XDG_SESSION_TYPE = DS_DEFAULT;
    XDG_SESSION_TYPE_NAME=nullptr;//strdup("Default");
    XDG_SESSION_NAME=nullptr;//strdup("Default");
    desktop_name = nullptr;//strdup("DESKTOP1"); // Must be parsed from config file later on;
    desktop_cmd = nullptr;//strdup("jwm");
    allocated=0;
    deallocated=0;
}
