// SPDX-License-Identifier: MIT

#include "config.h"
#include "daemonize.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void init_config( struct configuration *conf );
extern int load_config_toml( struct configuration *conf );
extern void destroy_config( struct configuration *conf );

extern int load_plugins( struct configuration *conf, struct plugin_store *plugs );

extern int connect_radio( struct configuration *conf, struct radio_connection *radio );
extern void disconnect_radio( struct radio_connection *radio );

extern int start_jsonrpc_server( struct configuration *conf, struct jsonrpc_server *srvr );
extern void shutdown_jsonrpc_server( struct jsonrpc_server *srvr );

extern int start_http_server( struct configuration *conf, struct http_server *srvr );
extern void shutdown_http_server( struct http_server *srvr );

extern int handle_socket_data( struct jsonrpc_server *jrpc, struct http_server *hserver, struct client_connections *connections );
extern int handle_radio_data( struct radio_connection *radio, struct client_connections *connections );

int main( int argc, char *argv[] )
{
    struct configuration conf;
    struct plugin_store plugs = {0};
    struct client_connections connections = {0};
    struct radio_connection radio;
    struct jsonrpc_server jrpc;
    struct http_server hserver;
    int rv;

    /* 0. initialize interrupt / signal handling */

    /* 1. Load / initialize the configuration */
    printf( "\nStarting Hamware version " VERSION_STRING "\n" );
    init_config( &conf );
    rv = load_config_toml( &conf );
    if ( rv != 0 )
    {
        printf( "\nError loading configuration\n" );
        destroy_config( &conf );
        return rv;
    }

    /* 2. Load plugins
     *    a) audio modes
     *         - voice
     *         - digital
     *            * teletype / rtty
     *            * fsk
     *            * ft8
     *            * morse code
     *            * custom
     *    b) radio control
     *       frequency, mode, tuner, noise reduction
     *         - hamlib
     */
    rv = load_plugins( &conf, &plugs );
    if ( rv != 0 )
    {
        printf( "\nError loading plugins\n" );
        destroy_config( &conf );
        return rv;
    }

    /* 3. Connect to radio (error if fail) */
    rv = connect_radio( &conf, &plugs, &radio );
    if ( rv != 0 )
    {
        printf( "\nError connecting to the radio\n" );
        destroy_config( &conf );
        return rv;
    }

    /* 4. Create jsonrpc server
     *
     * add rpc functiosn for freq. control, mode switch, tuner controls, NR controls
     */
    rv = create_jsonrpc_server( &conf, &jrpc );
    if ( rv != 0 )
    {
        printf( "\nError creating JSON/RPC server\n" );
        disconnect_radio( &radio );
        destroy_config( &conf );
        return rv;
    }
    
    /* 5. Create http server
     *
     * websocket handler for streams, audio / data
     */
    rv = create_http_server( &conf, &hserver );
    if ( rv != 0 )
    {
        printf( "\nError creating HTTP server\n" );
        shutdown_jsonrpc_server( &jrpc );
        disconnect_radio( &radio );
        destroy_config( &conf );
        return rv;
    }

    /* 6. main loop */
    while ( 1 )
    {
        handle_socket_data( &jrpc, &hserver, &connections );
        handle_radio_data( &radio, &connections );
    }

    /* 7. cleanup, destroy */
    shutdown_http_server( &hserver );
    shutdown_jsonrpc_server( &jrpc );
    disconnect_radio( &radio );
    destroy_config( &conf );
    
    /* 
    if ( daemonize( -1, 1 ) != 0 )
        exit( -1 );

    */
    return 0;
}
