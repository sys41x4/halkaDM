#ifndef HALKADM_AUTH_MANAGEMENT_H
#define HALKADM_AUTH_MANAGEMENT_H

#include <locale.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>

class AUTH_MANAGEMENT{
public:

    int authCheck(char* usrHomeDir, char* username, char* userpass);
};

#endif
