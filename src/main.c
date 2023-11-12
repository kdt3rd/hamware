// SPDX-License-Identifier: MIT

#include "config.h"
#include "daemonize.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
    printf( "\nStarting Hamware version " VERSION_STRING "\n" );
    if ( daemonize( -1, 1 ) != 0 )
        exit( -1 );

    while ( 1 )
    {
        printf( "running...\n" );
        sleep( 1 );
    }

    return 0;
}
