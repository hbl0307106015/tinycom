#include "utils.h"
#include "tinycom.h"

int main(int argc, char *argv[])
{
    int ret;
    if (argc < TINYCOM_MIN_ARGC) {
        usage();
        goto out;
    }

    ret = tinycom_init(argc, argv);
    if (ret)
        goto out;

    tinycom_main();

out:
    tinycom_quit();
    return 0;
}
