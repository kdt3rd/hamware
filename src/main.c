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

    /* 1. Load / initialize the configuration */

    /* 2. Load plugins
     *    a) audio modes
     *         - voice
     *         - digital
     *            * teletype / rtty
     *            * fsk
     *            * ft8
     *            * morse code
     *            * custom
     *    b) frequency control - hamlib
     *    c) mode switching - hamlib
     *    d) tuner
     *    e) noise reduction control
     */

    /* 3. Connect to radio (error if fail) */

    /* 4. Create jsonrpc server */
    /* 5. Create http server */

    /* 6. main loop */
    
    /*
    if ( daemonize( -1, 1 ) != 0 )
        exit( -1 );

    while ( 1 )
    {
        printf( "running...\n" );
        sleep( 1 );
    }
    */
    return 0;
}
