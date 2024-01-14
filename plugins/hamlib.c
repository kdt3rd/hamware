/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include <plugin_register.h>
#include <hamlib/rig.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef struct hamlib_context
{
    /* TODO: what else */
    RIG *rig;
    rig_model_t rigmodelid;
    char *serialport;
} hamlibcontext;

static void init_hamlib()
{
//    rig_set_debug_level( RIG_DEBUG_NONE );
    rig_set_debug_level( RIG_DEBUG_VERBOSE );

    rig_load_all_backends();
}

static void destroy_hamlib()
{
}

static void *create_hamlib_radio()
{
    hamlibcontext *ctxt = malloc( sizeof(hamlibcontext) );
    if ( ctxt )
    {
        ctxt->rig = NULL;
        ctxt->serialport = strdup( "/dev/ttyUSB0" );
        ctxt->rigmodelid = RIG_MODEL_IC7200;
    }
    return ctxt;
}

static void destroy_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        if (ctxt->serialport)
            free( ctxt->serialport );
        free( ctxt );
    }
}

static int config_hamlib_radio( void *context, hamware_config_entry_t cf, const char *val )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        switch ( cf )
        {
            case HAMWARE_MODEL_NAME:
                ctxt->rigmodelid = (rig_model_t)atoi( val );
                return 0;
            case HAMWARE_SERIAL_PORT:
                if ( ctxt->serialport )
                    free( ctxt->serialport );
                ctxt->serialport = strdup( val );
                return 0;
        }
    }
    printf( "invalid configuration entry or context" );
    return 1;
}

static int open_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt )
    {
        int retcode;
        freq_t freq;
        rmode_t rmode;
        pbwidth_t width;

        ctxt->rig = rig_init( ctxt->rigmodelid );
        if ( ! ctxt->rig )
        {
            printf( "rig_open: error initializing\n" );
            return 1;
        }

        strncpy( ctxt->rig->state.rigport.pathname, ctxt->serialport, HAMLIB_FILPATHLEN - 1 );

        retcode = rig_open( ctxt->rig );
        if ( retcode != RIG_OK )
        {
            printf( "rig_open: error %s\n", rigerror(retcode) );
            rig_cleanup( ctxt->rig );
            ctxt->rig = NULL;
            return 2;
        }

        rig_get_freq( ctxt->rig, RIG_VFO_CURR, &freq );
        rig_get_mode( ctxt->rig, RIG_VFO_CURR, &rmode, &width );

        printf( "Opened radio:\n      freq: %.6f MHz\n      mode: %s\n  passband: %.3f kHz\n\n",
                freq / 1000000.f,
                rig_strrmode( rmode ),
                width / 1000.f );

        return 0;
    }
    // no context, error
    return 1;
}

static void close_hamlib_radio( void *context )
{
    hamlibcontext *ctxt = context;
    if ( ctxt && ctxt->rig )
    {
        rig_close( ctxt->rig );
        rig_cleanup( ctxt->rig );
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
    .set_configuration = config_hamlib_radio,
    .open_radio = open_hamlib_radio,
    .close_radio = close_hamlib_radio,
    .set_frequency = hamlib_set_frequency,
    .set_mode = hamlib_set_mode
};

HAMWARE_PLUGIN_REGISTER
{
    r->register_radio_provider( context, "hamlib", &hamlibradio );
}
