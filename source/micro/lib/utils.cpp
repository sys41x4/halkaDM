//#include "configator.h"
//#include "dragonfail.h"
#include "inputs.h"
#include "../config/config.h"
#include "utils.h"

#include <dirent.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#if defined(__DragonFly__) || defined(__FreeBSD__)
	#include <sys/consio.h>
#else // linux
	#include <linux/vt.h>
#endif

void switch_tty()
//void switch_tty(struct term_buf* buf)
{
    FILE* console = fopen(config.console_dev, "w");

    if (console == NULL)
    {
//        buf->info_line = lang.err_console_dev;
        return;
    }

    int fd = fileno(console);

    ioctl(fd, VT_ACTIVATE, config.tty);
    ioctl(fd, VT_WAITACTIVE, config.tty);

    fclose(console);
}
