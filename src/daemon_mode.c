#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include "include/csapp.h"
#include "include/daemon_mode.h"

void daemonize()
{
    int i, fd0, fd1, fd2;
    pid_t pid;

    /*
     * Clear file creation mask.
     */
    umask(0);

    /*
     * Become a session leader to lose controlling TTY.
     */
    if ((pid = fork()) < 0)
        unix_error("can't fork");
    else if (pid != 0) /* parent */
        exit(0);
    setsid();



    /*
     * Change the current working directory to the root so
     * we won't prevent file systems from being unmounted.
     */
    /*if (chdir("/") < 0)
        unix_error("can't change directory to /");*/

    /*
     * Close all open file descriptors.
     */
    for (i = 0; i < 3; i++)
        close(i);

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file.
     */
    openlog("monitord", LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",fd0, fd1, fd2);
        exit(1);
    }
}
