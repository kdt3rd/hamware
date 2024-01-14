/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#include "configuration.h"

typedef struct http_server
{
    int https_server;

    void *ssl_context;
} http_server;

extern int start_http_server( configuration *conf, http_server *srvr );
extern void shutdown_http_server( http_server *srvr );

