// SPDX-License-Identifier: MIT

#include "config.h"

#include "daemonize.h"

#include "configuration.h"
#include "plugins.h"
#include "http.h"
#include "jsonrpc.h"

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _r_conn
{
    radio_provider *plug;
    void *context;
} radio_connection;

typedef struct _ham_server
{
    configuration config;
    plugin_store plugins;

    radio_connection r;

    jsonrpc_server jrpc;
    http_server http;
} ham_server;

static int keep_running = 1;
static void interrupt_handler( int sig )
{
    keep_running = 0;
}

static int connect_radio( ham_server *server )
{
    radio_provider *prov = NULL;
    if ( server->plugins.radio_providers )
    {
        prov = server->plugins.radio_providers;
        if ( server->config.radio_plugname )
        {
            prov = NULL;
            for ( int p = 0; p < server->plugins.num_radio_providers; ++p )
            {
                if ( 0 == strcmp( server->plugins.radio_providers[p].info.name,
                                  server->config.radio_plugname ) )
                {
                    prov = server->plugins.radio_providers + p;
                    printf( "\nUsing radio provider '%s'\n", prov->info.name );
                    break;
                }
            }
            if ( ! prov )
            {
                printf( "\nUnable to find radio provider '%s'\n", server->config.radio_plugname );
                return 1;
            }
        }
        else
        {
            printf( "\nUsing default radio provider '%s'\n", prov->info.name );
        }
        server->r.plug = prov;
        server->r.context = ( prov->create ) ? prov->create() : NULL;

        return ( prov->open_radio ) ? prov->open_radio( server->r.context ) : 0;
    }

    printf( "\nNo radio providers found\n" );
    return 1;
}

static void disconnect_radio( ham_server *server )
{
    if ( server->r.plug )
    {
        if ( server->r.plug->close_radio )
            server->r.plug->close_radio( server->r.context );
        if ( server->r.plug->destroy )
            server->r.plug->destroy( server->r.context );

        server->r.plug = NULL;
        server->r.context = NULL;
    }
}

static int handle_socket_data( ham_server *server )
{
    return 0;
}

static int handle_radio_data( ham_server *server )
{
    /* TODO: change this if hamlib can do a file descriptor so we can wait */
    if ( server->r.plug )
    {
    }
    return 1;
}

int main( int argc, char *argv[] )
{
    ham_server server;
    int rv;

    /* 0. initialize interrupt / signal handling */
    signal( SIGPIPE, SIG_IGN );
    signal( SIGINT, interrupt_handler );

    /* 1. Load / initialize the configuration */
    printf( "\nStarting Hamware version " VERSION_STRING "\n" );
    memset( &server, 0, sizeof(ham_server) );

    rv = init_config( &(server.config), argv, argc );
    if ( rv != 0 )
    {
        printf( "\nError loading configuration\n" );
        destroy_config( &(server.config) );
        return rv;
    }

    /* TODO: maybe bring this back eventually
    if ( daemonize( -1, 1 ) != 0 )
        exit( -1 );
    */

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
    rv = load_plugins( &(server.config), &(server.plugins) );
    if ( rv != 0 )
    {
        printf( "\nError loading plugins\n" );
        destroy_config( &(server.config) );
        return rv;
    }

    /* 3. Connect to radio (error if fail) */
    rv = connect_radio( &(server) );
    if ( rv != 0 )
    {
        printf( "\nError connecting to the radio\n" );
        destroy_plugins( &(server.plugins) );
        destroy_config( &(server.config) );
        return rv;
    }

    /* 4. Create jsonrpc server
     *
     * add rpc functiosn for freq. control, mode switch, tuner controls, NR controls
     */
    rv = start_jsonrpc_server( &(server.config), &(server.jrpc) );
    if ( rv != 0 )
    {
        printf( "\nError creating JSON/RPC server\n" );
        disconnect_radio( &(server) );
        destroy_plugins( &(server.plugins) );
        destroy_config( &(server.config) );
        return rv;
    }

    /* 5. Create http server
     *
     * websocket handler for streams, audio / data
     */
    rv = start_http_server( &(server.config), &(server.http) );
    if ( rv != 0 )
    {
        printf( "\nError creating HTTP server\n" );
        shutdown_jsonrpc_server( &(server.jrpc) );
        disconnect_radio( &(server) );
        destroy_plugins( &(server.plugins) );
        destroy_config( &(server.config) );
        return rv;
    }

    /* 6. main loop */
    while ( keep_running )
    {
        rv = handle_socket_data( &server );
        if ( rv != 0 )
        {
            printf( "\nError handling server, exiting...\n" );
            break;
        }

        rv = handle_radio_data( &server );
        if ( rv != 0 )
        {
            printf( "\nError handling radio data, exiting...\n" );
            break;
        }
    }

    /* 7. cleanup, destroy */
    printf( "\nShutting down...\n" );
    shutdown_http_server( &(server.http) );
    shutdown_jsonrpc_server( &(server.jrpc) );
    disconnect_radio( &(server) );
    destroy_plugins( &(server.plugins) );
    destroy_config( &(server.config) );

    return 0;
}
