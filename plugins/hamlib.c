/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include <plugin_register.h>
#include <hamlib/rig.h>

#include <malloc.h>
#include <string.h>

typedef struct hamlib_context
{
    /* TODO: what else */
    RIG rig;
} hamlibcontext;

static void init_hamlib()
{
}

static void destroy_hamlib()
{
}

static void *create_hamlib_radio()
{
    hamlibcontext *ctxt = malloc( sizeof(hamlibcontext) );
    if ( ctxt )
    {
    }
    return ctxt;
}

static void destroy_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        free( ctxt );
    }
}

static int open_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        return 0;
    }
    // no context, error
    return 1;
}

static void close_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
    }
}


static int hamlib_set_frequency( void *context, int frequency )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        return 0;
    }
    return 1;
}

static int hamlib_set_mode( void *context, int mode )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        return 0;
    }
    return 1;
}

static radio_provider_plugin hamlibradio =
{
    .init_library = init_hamlib,
    .shutdown_library = destroy_hamlib,
    .create_radio = create_hamlib_radio,
    .destroy_radio = destroy_hamlib_radio,
    .open_radio = open_hamlib_radio,
    .close_radio = close_hamlib_radio,
    .set_frequency = hamlib_set_frequency,
    .set_mode = hamlib_set_mode
};

HAMWARE_PLUGIN_REGISTER
{
    r->register_radio_provider( context, "hamlib", &hamlibradio );
}
