/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include "configuration.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int load_config_toml( configuration *conf )
{
    return 0;
}

int init_config( configuration *conf, char *argv[], int argc )
{
    char *envoverride;
    int rv;

    char progfullbuf[PATH_MAX+1];
    char *progfull = realpath(argv[0], progfullbuf);

    if (progfull)
    {
        char *progdir = strrchr(progfull, '/');
        strcpy(progdir + 1, "plugins");
    }
    else
    {
        strcpy(progfullbuf, "./plugins");
        progfull = progfullbuf;
    }

    memset( conf, 0, sizeof(configuration) );

    conf->application_dir = strdup( "." );
    conf->plugin_dir = strdup( progfull );
    conf->radio_plugname = strdup( "hamlib" );

    /* load the config file */
    rv = load_config_toml( conf );

    /*
     * name of the called program is always argv[0], so only care if we have
     * additional args which should override the config file...
     */
    for ( int arg = 1; arg < argc; ++arg )
    {
        /* TODO: parse command line arguments */
    }

    /* allow env var to override arguments */
    envoverride = getenv( "HAMWARE_CONFIG_VALUES" );
    if ( envoverride )
    {
        /* TODO: parse the string and set values... */
    }

    return rv;
}

void destroy_config( configuration *conf )
{
    if ( conf )
    {
        if ( conf->application_dir )
            free( conf->application_dir );
        if ( conf->plugin_dir )
            free( conf->plugin_dir );

        if ( conf->radio_device_name )
            free( conf->radio_device_name );
        if ( conf->radio_config )
            free( conf->radio_config );
        if ( conf->radio_plugname )
            free( conf->radio_plugname );

        if ( conf->jsonrpc_interface )
            free( conf->jsonrpc_interface );

        if ( conf->http_interface )
            free( conf->http_interface );
        if ( conf->http_server_rootdir )
            free( conf->http_server_rootdir );
        
        if ( conf->ssl_certfile )
            free( conf->ssl_certfile );
        if ( conf->ssl_keyfile )
            free( conf->ssl_keyfile );
    }
}
