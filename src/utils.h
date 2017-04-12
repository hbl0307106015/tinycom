#ifndef __UTILS_H__
#define __UTILS_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define dbg(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

static const char * prog_name = "tinycom";

static inline void usage(void)
{
    fprintf(stdout, "\nUsage: %s [options] file...\n"
            "\nOptions:\n"
            "\tfile\tthe path of serial device, "
                "example: /dev/ttyS0\n"
            "\t-b  \tbaud rate, "
                "example: 9600, 38400, 57600, 115200\n"
            "\t-d  \tdata bit, "
                "example: 5, 6, 7, 8\n"
            "\t-p  \tparity, "
                "example: 0 (no parity), 1 odd, 2 even\n"
            "\t-s  \tstop bit, "
                "example: 0 (no stop bit), 1, 2\n"
            "\n\t%s -b 115200 -d 8 -p 0 -s 1 /dev/ttyS0\n"
            "\n", \
            prog_name, prog_name);
#if 0
    fprintf(stdout, "\nUsage: %s [options] file...\n"
            "\nOptions:\n"
            "\tfile\tThe path of serial device, "
                "default: /dev/ttyS0\n"
            "\t-c  \tfile path of configuration, "
                "default: /etc/tinycom.conf\n"
            "\t-p  \tfile path of pid, "
                "default: /var/run/tinycom.pid\n\n", \
            prog_name);
#endif
}

#endif /* __UTILS_H__ */
