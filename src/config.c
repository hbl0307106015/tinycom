#include "utils.h"
#include "tinycom.h"

static int int_baud_rates [] = {
    9600, 38400, 57600, 115200, -1
};

static speed_t baud_rates[] = {
    B9600, B38400, B57600, B115200
};

static int
tinycom_init_baudrate(int fd, int b)
{
    int i;
    struct termios t;
    speed_t p = B115200;

    if (tcgetattr(fd, &t) != 0)
        goto out_err;

    for (i = 0; int_baud_rates[i] > 0; i++)
    {
        if (int_baud_rates[i] == b)
           p = baud_rates[i] ;
    }

    tcflush(fd, TCIOFLUSH);
    cfsetispeed(&t, p);
    cfsetospeed(&t, p);
    if (tcsetattr(fd, TCSANOW, &t) != 0)
        goto out_err; 

    tcflush(fd, TCIOFLUSH);
    return 0;
out_err:
    perror("set baud rate error");
    return 1;
}

static int
tinycom_init_databit(int fd, const uint16_t d)
{
#define d2c(d,c) case d: (c) |= (CS##d)
    struct termios t;

    if (tcgetattr(fd, &t) != 0)
        goto out_err;

    t.c_cflag &= (~CSIZE); // clear character size mask
    // ignore modem control lines, enable receiver
    t.c_cflag |= (CLOCAL | CREAD);

    switch (d) {
        d2c(5, t.c_cflag); break;
        d2c(6, t.c_cflag); break;
        d2c(7, t.c_cflag); break;
        d2c(8, t.c_cflag); break;
        default:
            dbg("%s date bit invalid\n", __func__);
            return 1;
    }

    if (tcsetattr(fd, TCSANOW, &t) != 0)
        goto out_err; 
    return 0;

out_err:
    perror("set date bit error");
    return 1;
}

static int
tinycom_init_stopbit(int fd, const uint16_t s)
{
    struct termios t;

    if (tcgetattr(fd, &t) != 0)
        goto out_err;

    switch (s) {
        case 1: t.c_cflag &= (~CSTOPB); break; // 1 stop bit
        case 2: t.c_cflag |= CSTOPB; break;    // 2 stop bit
        default:
            dbg("%s stop bit error\n", __func__);
            return 1;
    }

    if (tcsetattr(fd, TCSANOW, &t) != 0)
        goto out_err; 
    return 0;

out_err:
    perror("set stop bit error");
    return 1;
}

static int
tinycom_init_parity(int fd, const char parity)
{
    char c;
    struct termios t;

    if (tcgetattr(fd, &t) != 0)
        goto out_err;

    c = toupper((int)parity);
    switch (c) {
        case 'N': // no parity
        t.c_cflag &= (~PARENB); // clear parity
        t.c_iflag &= (~INPCK);  // enable parity checking
        break;
        case 'O': // odd parity
        t.c_cflag |= (PARODD | PARENB); // odd parity
        t.c_iflag |= INPCK; // disable parity checking
        break;
        case 'E': // even parity
        t.c_cflag |= PARENB; // enable parity
        t.c_cflag &= (~PARODD); // even parity
        t.c_iflag |= INPCK;
        break;
        case 'S': // as no parity
        t.c_cflag &= (~PARENB);
        t.c_cflag &= (~CSTOPB);
        break;
        default:
            dbg("%s parity invalid\n", __func__);
            return 1;
    }
    if (c != 'N')
        t.c_iflag |= INPCK;

    if (tcsetattr(fd, TCSANOW, &t) != 0)
        goto out_err;
    return 0;

out_err:
    perror("set parity error");
    return 1;
}

static int
tinycom_init_misc(int fd)
{
    struct termios t;

    if (tcgetattr(fd, &t) != 0)
        goto out_err;

    tcflush(fd, TCIFLUSH);

    // work in raw mode
    t.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    t.c_oflag &= ~OPOST;
    
    t.c_cc[VTIME] = 150; // timeout as 15 seconds
    t.c_cc[VMIN] = 0; // update the option and do it now

    // flow control
#if defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
    t.c_cflag &= (~CRTSCTS);
#endif /* defined(_BSD_SOURCE) || defined(_SVID_SOURCE) */
    t.c_iflag &= ~(IXON | IXOFF);

    if (tcsetattr(fd, TCSANOW, &t) != 0)
        goto out_err;
    return 0;
out_err:
    perror("set misc attribution error");
    return 1;
}

int
tinycom_init(int argc, char *argv[])
{
    int c = -1;
    int err = 0;
    struct tinycom *t = tinycom_object();

    if ( argc < TINYCOM_MIN_ARGC ) {
        dbg("%s argument error\n", __func__);
        goto out_err;
    }

    memset(t, 0, sizeof(struct tinycom));

    t->fd = tinycom_open(argv[argc - 1]);
    if (t->fd < 0) {
        perror("failed to open serial file");
        goto out_err;
    }

    for (;;)
    {
        c = getopt(argc, argv, "b:d:hp:s:");
        if (c < 0)
            break;
        switch (c)
        {
            case 'h':
            usage();
            break;
            case 'b':
            if (tinycom_init_baudrate(t->fd, atoi(optarg)))
                err++;
            break;
            case 'd':
            if (tinycom_init_databit(t->fd, atoi(optarg)))
                err++;
            break;
            case 'p':
            if (tinycom_init_parity(t->fd, *optarg))
                err++;
            break;
            case 's':
            if (tinycom_init_stopbit(t->fd, atoi(optarg)))
                err++;
            break;
            default:
            usage();
            break;
        }
    }
    if (tinycom_init_misc(t->fd))
        err++;

    if (err == 0)
        return 0;
out_err:
    dbg("program quit with error\n");
    return 1;
}
