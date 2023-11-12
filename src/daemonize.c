// SPDX-License-Identifier: MIT

#include "daemonize.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/eventfd.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void
close_open_files( int startFD )
{
	struct rlimit rlim;
	int err = getrlimit( RLIMIT_NOFILE, &rlim );
	if ( err < 0 )
        return;

	for ( int i = startFD; i < ( int )rlim.rlim_cur; ++i )
		close( i );
}

int daemonize( int startFD, int background )
{
    int err;

    if ( background )
    {
        int child_wait_fd;
        pid_t pid, sid;
        uint64_t val;

        child_wait_fd = eventfd( 0, EFD_CLOEXEC );
        if (child_wait_fd == -1)
            return errno;

        pid = fork();
        if ( pid < 0 )
            return errno;

        if ( pid != 0 )
        {
            // do any setup of the child process here...

            // trigger the child to continue...
            val = 1;
            err = write( child_wait_fd, &val, sizeof(uint64_t) );
            // don't really care about the result status
            close( child_wait_fd );

            exit( 0 );
        }

        err = read( child_wait_fd, &val, sizeof(uint64_t) );
        close( child_wait_fd );

        // Become the session leader
        sid = setsid();
        if ( sid < 0 )
            return errno;

        // do the 'double fork' thing to truly detach from any terminal
        pid = fork();
        if ( pid < 0 )
            return errno;

        if ( pid != 0 )
        {
            exit( 0 );
        }

        // in the grandchild, off we go
    }

    // Change to root so we aren't in anyone's mount area...
	err = chdir( "/" );
	if ( err < 0 )
        return errno;

	// Clear umask so people have all the control they need...
	umask( 0 );

	// Close all open files to make sure we don't hold open any file descriptors
	// we inherited from our parent. Point stdin, stdout, stderr at /dev/null.
	if ( startFD >= 0 )
        close_open_files( startFD );

	if ( startFD == 0 )
	{
		int fd0 = open( "/dev/null", O_RDWR );
		int fd1 = dup( fd0 );
		int fd2 = dup( fd0 );

		if ( fd0 != STDIN_FILENO || fd1 != STDOUT_FILENO || fd2 != STDERR_FILENO )
			_exit( -1 );
    }


    return 0;
}

