#ifndef __TINYCOM_H__
#define __TINYCOM_H__ 1

#include "utils.h"

enum {
    ROLE_NONE,
    ROLE_SENDER,
    ROLE_RECEIVER,
    __ROLE_MAX,
};

enum {
    ECHO_MODE_STDIN,
    __ECHO_MODE_MAX,
};

struct tinycom {
    int fd;
    int role;
    union {
        struct _role_s {
            int echo_mode;
        } role_sender;
        struct _role_r {
            void *c;
        } role_receiver;
    };
};

#define TINYCOM_MIN_ARGC 4

static struct tinycom tym;

static inline int tinycom_open(const char *f)
{
    int fd = open(f, O_RDWR);
    if (fd < 0)
        goto out_err;

    tym.fd = fd;
    return 0;
out_err:
    return -1;
}

static inline int tinycom_quit(void)
{
    close(tym.fd);
    tym.fd = -1;
    return 0;
}

static inline struct tinycom *
tinycom_object(void)
{
    return &tym;
}

static inline int tinycom_main(void) { return 0; }

extern int tinycom_init(int, char *[]);

#endif /* __TINYCOM_H__ */
