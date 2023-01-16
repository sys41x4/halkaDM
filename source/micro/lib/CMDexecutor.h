#ifndef HALKADM_CMD_EXECUTOR_H
#define HALKADM_CMD_EXECUTOR_H

#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

class CMD_EXECUTOR{
public:
    void exec(const char* cmd);
    void exec(char* cmd);
    char* fetchExecOutput(char* arr, const char* cmd);
    char* fetchExecOutput(char* arr, char* cmd);
};

CMD_EXECUTOR cmd_executor;

#endif
