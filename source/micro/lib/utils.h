#ifndef HALKADM_UTILS_H
#define HALKADM_UTILS_H

//#include "draw.h"
#include "inputs.h"
#include "../config/config.h"

void desktop_load(struct desktop* target);
//void hostname(char** out);
//void free_hostname();
void switch_tty();
void switch_tty(struct term_buf* buf);
void save(struct desktop* desktop, struct text* login);
void load(struct desktop* desktop, struct text* login);

#endif
