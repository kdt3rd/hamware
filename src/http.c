/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include "http.h"

#include <unistd.h>

int start_http_server( configuration *conf, http_server *srvr )
{
//    srvr->http_server = -1;
    srvr->https_server = -1;
    srvr->ssl_context = NULL;

    return 0;
}

void shutdown_http_server( http_server *srvr )
{
    if ( srvr )
    {
//        if ( srvr->http_server >= 0 )
//            close( srvr->http_server );
        if ( srvr->https_server >= 0 )
            close( srvr->https_server );
        /* todo: cleanup ssl */
    }
}

