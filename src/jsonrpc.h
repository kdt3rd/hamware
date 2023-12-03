/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#include "configuration.h"

typedef struct jsonrpc_server
{
    int jsonrpc_server;
    int jsonrpcs_server;

    void *ssl_context;
} jsonrpc_server;

extern int start_jsonrpc_server( configuration *conf, jsonrpc_server *srvr );
extern void shutdown_jsonrpc_server( jsonrpc_server *srvr );

