#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <xcb/xcb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <grp.h>
#include <errno.h>
#include <pwd.h>
#include <paths.h>
#include "inputs.h"
#include "pam.h"

//#define SERVICE_NAME "display-manager"

void DM_PAMAuth::err(const char* name, struct pam_handle* handle, int result){
    do {
        fprintf(stderr, "%s: %s\n", name, pam_strerror(handle, result));
        end(result, handle);
        //return false;
    } while (1);
}
/*static void init_env(struct passwd *pw);
static void set_env(char *name, char *value);
static int end(int last_result);

static int conv(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr);
*/

//static pam_handle_t *pam_handle;

int DM_PAMAuth::get_free_display()
{
	char xlock[1024];
	uint8_t i;

	for (i = 0; i < 200; ++i)
	{
		snprintf(xlock, 1024, "/tmp/.X%d-lock", i);

		if (access(xlock, F_OK) == -1)
		{
			break;
		}
	}

	return i;
}

void DM_PAMAuth::reset_terminal(struct passwd* pw)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        execl(pw->pw_shell, pw->pw_shell, "-c", term_reset_cmd, NULL);
        exit(EXIT_SUCCESS);
    }

    int status;
    waitpid(pid, &status, 0);
}

void DM_PAMAuth::env_xdg_session(int display_server)
{
    switch (display_server)
    {
        case DS_SHELL:
        {
            setenv("XDG_SESSION_TYPE", "tty", 0);
            break;
        }
        case DS_XINITRC:
        case DS_XORG:
        {
            setenv("XDG_SESSION_TYPE", "x11", 0);
            break;
        }
        case DS_WAYLAND:
        {
            setenv("XDG_SESSION_TYPE", "wayland", 1);
            break;
        }
    }
}

void DM_PAMAuth::env_xdg(const char* tty_id, const char* desktop_name)
{
    char user[20];
    snprintf(user, 20, "/run/user/%d", getuid());
    setenv("XDG_RUNTIME_DIR", user, 0);
    setenv("XDG_SESSION_CLASS", "user", 0);
    setenv("XDG_SESSION_ID", "1", 0);
    setenv("XDG_SESSION_DESKTOP", desktop_name, 0);
    setenv("XDG_SEAT", "seat0", 0);
    setenv("XDG_VTNR", tty_id, 0);
}

void DM_PAMAuth::add_utmp_entry(
	struct utmp *entry,
	const char* username,
	pid_t display_pid
) {
    entry->ut_type = USER_PROCESS;
    entry->ut_pid = display_pid;
    strcpy(entry->ut_line, ttyname(STDIN_FILENO) + strlen("/dev/"));

    /* only correct for ptys named /dev/tty[pqr][0-9a-z] */
    strcpy(entry->ut_id, ttyname(STDIN_FILENO) + strlen("/dev/tty"));

    time((long int *) &entry->ut_time);

    strncpy(entry->ut_user, username, UT_NAMESIZE);
    memset(entry->ut_host, 0, UT_HOSTSIZE);
    entry->ut_addr = 0;
    setutent();

    pututline(entry);
}

void DM_PAMAuth::remove_utmp_entry(struct utmp *entry) {
    entry->ut_type = DEAD_PROCESS;
    memset(entry->ut_line, 0, UT_LINESIZE);
    entry->ut_time = 0;
    memset(entry->ut_user, 0, UT_NAMESIZE);
    setutent();
    pututline(entry);
    endutent();
}


void DM_PAMAuth::xauth(const char* display_name, const char* shell, char* pwd)
{
	const char* xauth_file = "halkaDMxauth";
	char* xauth_dir = getenv("XDG_RUNTIME_DIR");
	if ((xauth_dir == NULL) || (*xauth_dir == '\0'))
	{
		xauth_dir = getenv("XDG_CONFIG_HOME");
		struct stat sb;
		if ((xauth_dir == NULL) || (*xauth_dir == '\0'))
		{
			xauth_dir = strdup(pwd);
			strcat(xauth_dir, "/.config");
			stat(xauth_dir, &sb);
			if (S_ISDIR(sb.st_mode))
			{
				strcat(xauth_dir, "/halkaDM");
			}
			else
			{
				xauth_dir = pwd;
				xauth_file = ".halkaDMxauth";
			}
		}
		else
		{
			strcat(xauth_dir, "/halkaDM");
		}

		// If .config/halkaDM/ or XDG_CONFIG_HOME/halkaDM/ doesn't exist and can't create the directory, use pwd
		// Passing pwd beforehand is safe since stat will always evaluate false
		stat(xauth_dir, &sb);
		if (!S_ISDIR(sb.st_mode) && mkdir(xauth_dir, 0777) == -1)
		{
			xauth_dir = pwd;
			xauth_file = ".halkaDMxauth";
		}
	}

	// trim trailing slashes
	int i = strlen(xauth_dir) - 1;
	while (xauth_dir[i] == '/') i--;
	xauth_dir[i + 1] = '\0';

	char xauthority[256];
	snprintf(xauthority, 256, "%s/%s", xauth_dir, xauth_file);
	setenv("XAUTHORITY", xauthority, 1);
	setenv("DISPLAY", display_name, 1); // Review

	FILE* fp = fopen(xauthority, "ab+");

	if (fp != NULL)
	{
		fclose(fp);
	}

	pid_t pid = fork();

	if (pid == 0)
	{
		char cmd[1024];
		snprintf(
			cmd,
			1024,
			"%s add %s . `%s`",
			xauth_cmd,
			display_name,
			mcookie_cmd);
		execl(shell, shell, "-c", cmd, NULL);
		exit(EXIT_SUCCESS);
	}

	int status;
	waitpid(pid, &status, 0);
}

void DM_PAMAuth::xorg(
	struct passwd* pwd,
	const char* vt,
	const char* desktop_cmd)
{
    char display_name[4];

    snprintf(display_name, 3, ":%d", get_free_display());
    xauth(display_name, pwd->pw_shell, pwd->pw_dir);

	// start xorg
    pid_t pid = fork();

    if (pid == 0)
    {
        char x_CMD[1024];
        snprintf(
            x_CMD,
            1024,
            "%s %s %s",
            x_cmd,
            display_name,
            vt);
        execl(pwd->pw_shell, pwd->pw_shell, "-c", x_CMD, NULL);
        exit(EXIT_SUCCESS);
    }

    int ok;
    xcb_connection_t* xcb;

    do
    {
        xcb = xcb_connect(NULL, NULL);
        ok = xcb_connection_has_error(xcb);
        kill(pid, 0);
    }
    while((ok != 0) && (errno != ESRCH));

    if (ok != 0)
    {
        return;
    }

    pid_t xorg_pid = fork();

    if (xorg_pid == 0)
    {
        char de_cmd[1024];
        snprintf(
            de_cmd,
            1024,
            "%s %s",
            x_cmd_setup,
            desktop_cmd);
        execl(pwd->pw_shell, pwd->pw_shell, "-c", de_cmd, NULL);
        exit(EXIT_SUCCESS);
    }

    int status;
    waitpid(xorg_pid, &status, 0);
    xcb_disconnect(xcb);
    kill(pid, 0);

    if (errno != ESRCH)
    {
        kill(pid, SIGTERM);
        waitpid(pid, &status, 0);
    }
}

void DM_PAMAuth::wayland(
	struct passwd* pwd,
	const char* desktop_cmd)
{
//        setenv("WAYLAND_DISPLAY", user.desktop_name, 1);
	char cmd[1024];
	snprintf(cmd, 1024, "%s %s", wayland_cmd, desktop_cmd);
	execl(pwd->pw_shell, pwd->pw_shell, "-c", cmd, NULL);
}

void DM_PAMAuth::shell(struct passwd* pwd)
{
    const char* pos = strrchr(pwd->pw_shell, '/');
    char args[1024];
    args[0] = '-';

    if (pos != NULL)
    {
        pos = pos + 1;
    }
    else
    {
        pos = pwd->pw_shell;
    }

    strncpy(args + 1, pos, 1023);
    execl(pwd->pw_shell, args, NULL);
}

void DM_PAMAuth::allocate(){
    XDG_SESSION_TYPE=DS_DEFAULT;
    tty=2;
    desktop_cmd=nullptr;
    desktop_name=nullptr;
    wayland_cmd=nullptr;
    xauth_cmd=nullptr;
    mcookie_cmd=nullptr;
    x_cmd=nullptr;
    x_cmd_setup=nullptr;
    service_name=nullptr;
    path=nullptr;
    term_reset_cmd=nullptr;
    username=nullptr;
    password=nullptr;
}

void DM_PAMAuth::deallocate(){
    XDG_SESSION_TYPE=DS_DEFAULT;
    tty=2;
    std::free(desktop_name);
    std::free(term_reset_cmd);
    std::free(service_name);
    std::free(path);
    std::free(username);
    std::free(password);

    std::free(desktop_cmd);
    std::free(wayland_cmd);
    std::free(xauth_cmd);
    std::free(mcookie_cmd);
    std::free(x_cmd);
    std::free(x_cmd_setup);

}

bool DM_PAMAuth::login(const char *user, const char *pass){

    if(user==nullptr || pass==nullptr){return 0;}

    int result;
    char tty_id [3];

    snprintf(tty_id, 3, "%d", tty);


    // Add XDG environment variables
    env_xdg_session(XDG_SESSION_TYPE);
    env_xdg(tty_id, desktop_name);

    // open pam session
    const char *data[2] = {user, pass};
    struct pam_conv pam_conv = { conv, data };
    struct pam_handle* handle;

    result = pam_start(service_name, NULL, &pam_conv, &handle);

    if (result != PAM_SUCCESS) {
        err("pam_start", handle, result);
        pam_end(handle, result);
        return 0;
    }

    result = pam_set_item(handle, PAM_USER, user);

    if (result != PAM_SUCCESS) {
        err("pam_set_item", handle, result);
        pam_end(handle, result);
        return 0;
    }

    result = pam_authenticate(handle, 0);
    if (result != PAM_SUCCESS) {
        err("pam_authenticate", handle, result);
        pam_end(handle, result);
        return 0;
    }

    result = pam_acct_mgmt(handle, 0);
    if (result != PAM_SUCCESS) {
        err("pam_acct_mgmt", handle, result);
        pam_end(handle, result);
        return 0;
    }

    result = pam_setcred(handle, PAM_ESTABLISH_CRED);
    if (result != PAM_SUCCESS) {
        err("pam_setcred", handle, result);
        pam_end(handle, result);
        return 0;
    }

    result = pam_open_session(handle, 0); // This func is Causing Memory Leak :[definitely lost: 8,192 bytes in 2 blocks] only after using this function
    // Reched with Manual Returns and breakpoints in source codes
    if (result != PAM_SUCCESS) {
        pam_setcred(handle, PAM_DELETE_CRED);
        err("pam_open_session", handle, result);
        pam_end(handle, result);
        return 0;
    }


////////////////////////////////////////////////////////
    // get passwd structure
    struct passwd* pw = getpwnam(user);
    endpwent();

    if (pw == NULL)
    {
        //dgn_throw(DGN_PWNAM);
        pam_end(handle, result);
        return 0;
    }

    // set user shell
    if (pw->pw_shell[0] == '\0')
    {
        setusershell();

        char* shell = getusershell();

        if (shell != NULL)
        {
            strcpy(pw->pw_shell, shell);
        }

        endusershell();

        std::free(shell);

    }

    // init_env(pw);


    // start desktop environment
    //int pid = fork();
    pid_t pid = fork();


    if (pid == 0){
        result = initgroups(pw->pw_name, pw->pw_gid);
        //if (result != 0){err("");}
        if (result != 0){exit(EXIT_FAILURE);}

        result = setgid(pw->pw_gid);
        //if (result != 0){err("");}
        if (result != 0){exit(EXIT_FAILURE);}

        result = setuid(pw->pw_uid);
        //if (result != 0){err("");}
        if (result != 0){exit(EXIT_FAILURE);}

        // get a display
        char vt[5];
        snprintf(vt, 5, "vt%d", tty);

        // set env (this clears the environment)
        init_env(pw, path);
        // Re-add XDG environment variables
        env_xdg_session(XDG_SESSION_TYPE);
        env_xdg(tty_id, desktop_name);

        // add pam variable
        char** env = pam_getenvlist(handle);

        for (uint16_t i = 0; env && env[i]; ++i){putenv(env[i]);}

        if (env != NULL) {
            for (int i = 0; env[i] != NULL; i++) {
                std::free(env[i]);
            }

            std::free(env);
        }
        // execute
        int result = chdir(pw->pw_dir);
        //if (result != 0){err("");}
        if (result != 0){exit(EXIT_FAILURE);}

        reset_terminal(pw);


        switch(XDG_SESSION_TYPE){ // Must be fixed
            case DS_WAYLAND:
            {
                // Currently wayland protocol support is in development
                //wayland(pw, desktop_cmd);
                xorg(pw, vt, desktop_cmd);
                break;
            }
            case DS_SHELL:
            {
                shell(pw);
                break;
            }
            case DS_XINITRC:
            case DS_XORG:
            {
                xorg(pw, vt, desktop_cmd);
                break;
            }
        }

        exit(EXIT_SUCCESS);


    }

    // add utmp audit
    struct utmp entry;
    add_utmp_entry(&entry, pw->pw_name, pid);

    // wait for the session to stop
    int status;
    waitpid(pid, &status, 0);
    remove_utmp_entry(&entry);
    reset_terminal(pw);

    // reinit login Screen for halkaDM
    // reload the desktop environment list on logout

    // On Ending Session Logout
    logout(handle);
/*
    result = pam_close_session(handle, 0);
    if (result != PAM_SUCCESS) {
        //pam_setcred(handle, PAM_DELETE_CRED);
        err("pam_close_session", handle, result);
        return 0;
    }

    result = pam_setcred(handle, PAM_DELETE_CRED);
    if (result != PAM_SUCCESS) {
        err("pam_setcred", handle, result);
        return 0;
    }

//    end(result);
    result = pam_end(handle, 0);
*/
/*    if (result != PAM_SUCCESS)
    {
        pam_diagnose(ok, buf);
    }
*/

    return true;
}

bool DM_PAMAuth::logout(struct pam_handle* handle) {
    int result = pam_close_session(handle, 0);
    if (result != PAM_SUCCESS) {
        pam_setcred(handle, PAM_DELETE_CRED);
        err("pam_close_session", handle, result);
    }

    result = pam_setcred(handle, PAM_DELETE_CRED);
    if (result != PAM_SUCCESS) {
        err("pam_setcred", handle, result);
    }

    end(result, handle);
    return true;
}

void DM_PAMAuth::set_env(char *name, char *value, struct pam_handle* handle) {
    //char *name_value = (char*)malloc(strlen(name) + strlen(value) + 2);
    char* name_value = static_cast<char*>(std::malloc((strlen(name) + strlen(value) + 2) * sizeof(char)));
    strcpy(name_value, name);
    strcat(name_value, "=");
    strcat(name_value, value);
    pam_putenv(handle, name_value); // TODO: Handle errors
    std::free(name_value);
}

void DM_PAMAuth::init_env(struct passwd *pw, const char* path) {
    extern char** environ;
    char* term = getenv("TERM");
    char* lang = getenv("LANG");
    // clean env
    environ[0] = NULL;

    setenv("TERM", term ? term : "linux", 1);
    setenv("HOME", pw->pw_dir, 1);
    setenv("PWD", pw->pw_dir, 1);
    setenv("SHELL", pw->pw_shell, 1);
    setenv("USER", pw->pw_name, 1);
    setenv("LOGNAME", pw->pw_name, 1);
    setenv("LANG", lang ? lang : "C", 1);
    if (strlen(path))
    {
        int ok = setenv("PATH", strdup(path), 1);

        if (ok != 0)
        {
            //dgn_throw(DGN_PATH);
        }
    }
    else{setenv("PATH", strdup("/usr/local/sbin:/usr/local/bin:/usr/bin"), 1);}

    //set_env("DISPLAY", DISPLAY);
    setenv("MAIL", _PATH_MAILDIR, 1);
    //char* xauthority = (char*)malloc(strlen(pw->pw_dir) + strlen("/.Xauthority") + 1);
    char* xauthority = static_cast<char*>(std::malloc((strlen(pw->pw_dir) + strlen("/.Xauthority") + 1) * sizeof(char)));
    strcpy(xauthority, pw->pw_dir);
    strcat(xauthority, "/.Xauthority");
    setenv("XAUTHORITY", xauthority, 1);
    std::free(xauthority);
}

int DM_PAMAuth::end(int last_result, struct pam_handle* handle) {
    int result = pam_end(handle, last_result);
    handle = 0;
    return result;
}

int conv(int num_msg, const struct pam_message** msg,
                 struct pam_response** resp, void* appdata_ptr) {
    int i;

    *resp = (struct pam_response*)calloc(num_msg, sizeof(struct pam_response));
    if (*resp == NULL) {
        return PAM_BUF_ERR;
    }

    char* user=nullptr;
    char* pass=nullptr;
    int result = PAM_SUCCESS;
    for (i = 0; i < num_msg; ++i) {
        switch (msg[i]->msg_style)
       {
           case PAM_PROMPT_ECHO_ON:
           {
               user = ((char **) appdata_ptr)[0];
               (*resp)[i].resp = strdup(user);
               //std::free(user);
               break;
           }
           case PAM_PROMPT_ECHO_OFF:
           {
               pass = ((char **) appdata_ptr)[1];
               (*resp)[i].resp = strdup(pass);
               //std::free(pass);
               break;
           }
           case PAM_ERROR_MSG:
           {
               //fprintf(stderr, "%s\n", msg[i]->msg);
               result = PAM_CONV_ERR;
               break;
           }
           case PAM_TEXT_INFO:
           {
               //printf("%s\n", msg[i]->msg);
               break;
            }
       }
        if (result != PAM_SUCCESS) {
            break;
        }
    }

    if (result != PAM_SUCCESS) {
        //free(*resp);
        //*resp = 0;
        for (i = 0; i < num_msg; ++i)
        {
            if ((*resp)[i].resp == NULL)
            {
                continue;
            }

            free((*resp)[i].resp);
            (*resp)[i].resp = NULL;
        }

        free(*resp);
        *resp = NULL;

    }

    return result;
}

