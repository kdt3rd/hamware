/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include "jsonrpc.h"

#include <unistd.h>

int start_jsonrpc_server( configuration *conf, jsonrpc_server *srvr )
{
    srvr->jsonrpc_server = -1;
    srvr->jsonrpcs_server = -1;
    srvr->ssl_context = NULL;

    return 0;
}

void shutdown_jsonrpc_server( jsonrpc_server *srvr )
{
    if ( srvr )
    {
        if ( srvr->jsonrpc_server >= 0 )
            close( srvr->jsonrpc_server );
        if ( srvr->jsonrpcs_server >= 0 )
            close( srvr->jsonrpcs_server );
        /* todo: cleanup ssl */
    }
}
