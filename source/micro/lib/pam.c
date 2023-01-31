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

#define SERVICE_NAME "display-manager"

#define err(name)                                   \
    do {                                            \
        fprintf(stderr, "%s: %s\n", name,           \
                pam_strerror(pam_handle, result));  \
        end(result);                                \
        return false;                               \
    } while (1);                                    \

static void init_env(struct passwd *pw);
static void set_env(char *name, char *value);
static int end(int last_result);

static int conv(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr);

static pam_handle_t *pam_handle;

int get_free_display()
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

void reset_terminal(struct passwd* pw)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		execl(pw->pw_shell, pw->pw_shell, "-c", config.term_reset_cmd, NULL);
		exit(EXIT_SUCCESS);
	}

	int status;
	waitpid(pid, &status, 0);
}

//void env_xdg_session(const enum display_server display_server)
void env_xdg_session(int display_server)
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

void env_xdg(const char* tty_id, const char* desktop_name)
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

void add_utmp_entry(
	struct utmp *entry,
	char *username,
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

void remove_utmp_entry(struct utmp *entry) {
	entry->ut_type = DEAD_PROCESS;
	memset(entry->ut_line, 0, UT_LINESIZE);
	entry->ut_time = 0;
	memset(entry->ut_user, 0, UT_NAMESIZE);
	setutent();
	pututline(entry);
	endutent();
}


void xauth(const char* display_name, const char* shell, char* pwd)
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
			config.xauth_cmd,
			display_name,
			config.mcookie_cmd);
		execl(shell, shell, "-c", cmd, NULL);
		exit(EXIT_SUCCESS);
	}

	int status;
	waitpid(pid, &status, 0);
}

void xorg(
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
		char x_cmd[1024];
		snprintf(
			x_cmd,
			1024,
			"%s %s %s",
			config.x_cmd,
			display_name,
			vt);
		execl(pwd->pw_shell, pwd->pw_shell, "-c", x_cmd, NULL);
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
			config.x_cmd_setup,
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

void wayland(
	struct passwd* pwd,
	const char* desktop_cmd)
{

	char cmd[1024];
	snprintf(cmd, 1024, "%s %s", config.wayland_cmd, desktop_cmd);
	execl(pwd->pw_shell, pwd->pw_shell, "-c", cmd, NULL);
}

void shell(struct passwd* pwd)
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

//bool login(const char *username, const char *password, pid_t *pid){
bool login(const char *username, const char *password){

    int result;
    char tty_id [3];

    snprintf(tty_id, 3, "%d", config.tty);


    // Add XDG environment variables
    env_xdg_session(user.XDG_SESSION_TYPE);
    env_xdg(tty_id, user.desktop_name);


    const char *data[2] = { username, password };
    struct pam_conv pam_conv = { conv, data };

    result = pam_start(config.service_name, NULL, &pam_conv, &pam_handle);


    if (result != PAM_SUCCESS) {
        err("pam_start");
    }

    result = pam_set_item(pam_handle, PAM_USER, username);

    if (result != PAM_SUCCESS) {
        err("pam_set_item");
    }

    result = pam_authenticate(pam_handle, 0);
    if (result != PAM_SUCCESS) {
        err("pam_authenticate");
    }

    result = pam_acct_mgmt(pam_handle, 0);
    if (result != PAM_SUCCESS) {
        err("pam_acct_mgmt");
    }

    result = pam_setcred(pam_handle, PAM_ESTABLISH_CRED);
    if (result != PAM_SUCCESS) {
        err("pam_setcred");
    }

    result = pam_open_session(pam_handle, 0);
    if (result != PAM_SUCCESS) {
        pam_setcred(pam_handle, PAM_DELETE_CRED);
        err("pam_open_session");
    }

    // Clear the credentials buffer

    // get passwd structure
    struct passwd *pw = getpwnam(username);
    endpwent();

    if (pw == NULL)
    {
        //dgn_throw(DGN_PWNAM);
        pam_end(pam_handle, result);
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
    }

    // init_env(pw);


    // start desktop environment
    int pid = fork();


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
        snprintf(vt, 5, "vt%d", config.tty);

        // set env (this clears the environment)
        init_env(pw);
        // Re-add XDG environment variables
        env_xdg_session(user.XDG_SESSION_TYPE);
        env_xdg(tty_id, user.desktop_name);

        // add pam variable
        char** env = pam_getenvlist(pam_handle);

        for (uint16_t i = 0; env && env[i]; ++i){putenv(env[i]);}
        // execute
        int result = chdir(pw->pw_dir);
        //if (result != 0){err("");}
        if (result != 0){exit(EXIT_FAILURE);}

        reset_terminal(pw);

        //char *cmd = strdup("exec /bin/bash --login .xinitrc");
//        char* cmd = strdup("startx");
        //execl(pw->pw_shell, pw->pw_shell, "-c", cmd, NULL);

        switch(user.XDG_SESSION_TYPE){ // Must be fixed
            case DS_WAYLAND:
            {
                wayland(pw, user.desktop_cmd);
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
                xorg(pw, vt, user.desktop_cmd);
                break;
            }
        }

        exit(EXIT_SUCCESS);
//        exit(1);


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
    logout();




/*    if (*child_pid == 0) {
        result = initgroups(pw->pw_name, pw->pw_gid);
        result = setgid(pw->pw_gid);
        result = setuid(pw->pw_uid);
        setenv("XDG_SESSION_TYPE", "x11", 0);
//        setenv("XDG_SESSION_TYPE", "x11", 0);

        char** env = pam_getenvlist(pam_handle);


        chdir(pw->pw_dir);
        // We don't use ~/.xinitrc because we should already be in the users home directory
        char *cmd = strdup("exec /bin/bash --login .xinitrc");
//        char* cmd = strdup("startx");
        execl(pw->pw_shell, pw->pw_shell, "-c", cmd, NULL);
        printf("Failed to start window manager");
        exit(1);
    }*/

    return true;
}

bool logout(void) {
    int result = pam_close_session(pam_handle, 0);
    if (result != PAM_SUCCESS) {
        pam_setcred(pam_handle, PAM_DELETE_CRED);
        err("pam_close_session");
    }

    result = pam_setcred(pam_handle, PAM_DELETE_CRED);
    if (result != PAM_SUCCESS) {
        err("pam_setcred");
    }

    end(result);
    return true;
}

static void set_env(char *name, char *value) {
    char *name_value = (char*)malloc(strlen(name) + strlen(value) + 2);
//    char* name_value = static_cast<char*>(std::malloc((strlen(name) + strlen(value) + 2) * sizeof(char)));
    strcpy(name_value, name);
    strcat(name_value, "=");
    strcat(name_value, value);
    pam_putenv(pam_handle, name_value); // TODO: Handle errors
    free(name_value);
}

static void init_env(struct passwd *pw) {
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
    if (strlen(config.path))
    {
        setenv("PATH", strdup(config.path), 1);
    }
    else{setenv("PATH", strdup("/usr/local/sbin:/usr/local/bin:/usr/bin"), 1);}

    //set_env("DISPLAY", DISPLAY);
    setenv("MAIL", _PATH_MAILDIR, 1);
    char *xauthority = (char*)malloc(strlen(pw->pw_dir) + strlen("/.Xauthority") + 1);
    strcpy(xauthority, pw->pw_dir);
    strcat(xauthority, "/.Xauthority");
    setenv("XAUTHORITY", xauthority, 1);
    free(xauthority);
}

static int end(int last_result) {
    int result = pam_end(pam_handle, last_result);
    pam_handle = 0;
    return result;
}

static int conv(int num_msg, const struct pam_message **msg,
                 struct pam_response **resp, void *appdata_ptr) {
    int i;

    *resp = (struct pam_response*)calloc(num_msg, sizeof(struct pam_response));
    if (*resp == NULL) {
        return PAM_BUF_ERR;
    }

    int result = PAM_SUCCESS;
    for (i = 0; i < num_msg; i++) {
        char *username, *password;
        switch (msg[i]->msg_style) {
        case PAM_PROMPT_ECHO_ON:
            username = ((char **) appdata_ptr)[0];
            (*resp)[i].resp = strdup(username);
            break;
        case PAM_PROMPT_ECHO_OFF:
            password = ((char **) appdata_ptr)[1];
            (*resp)[i].resp = strdup(password);
            break;
        case PAM_ERROR_MSG:
            fprintf(stderr, "%s\n", msg[i]->msg);
            result = PAM_CONV_ERR;
            break;
        case PAM_TEXT_INFO:
            printf("%s\n", msg[i]->msg);
            break;
        }
        if (result != PAM_SUCCESS) {
            break;
        }
    }

    if (result != PAM_SUCCESS) {
        free(*resp);
        *resp = 0;
    }

    return result;
}

