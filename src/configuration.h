/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#pragma once

////////////////////////////////////////

typedef struct configuration_v1
{
    char *application_dir;
    char *plugin_dir;

    char *radio_device_name;
    char *radio_config; /* file or string? */
    char *radio_plugname; /* identifies which plugin to use for radio device */
    char *radio_portname; /* identifies which plugin to use for radio device */

    char *jsonrpc_interface; /* if null, all network cards */
    int jsonrpc_port;
    int jsonrpcs_port;

    char *http_interface; /* if null, all network cards */
    int http_port;
    int https_port;

    char *http_server_rootdir;

    char *ssl_certfile;
    char *ssl_keyfile;
} configuration;

extern int init_config( configuration *conf, char *argv[], int argc );
extern void destroy_config( configuration *conf );



