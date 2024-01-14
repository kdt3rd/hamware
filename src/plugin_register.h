/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#include "hamware_config.h"

typedef struct signal_processor_plugin_v1
{
    
} signal_processor_plugin;

typedef struct radio_provider_plugin_v1
{
    void (*init_library)( );
    void (*shutdown_library)( );

    /* TODO: config args? */
    void *(*create_radio)( );
    void (*destroy_radio)( void *context );

    int (*set_configuration)( void *context, hamware_config_entry_t config, const char *val );

    /* TODO: config args? */
    int (*open_radio)( void *context );
    void (*close_radio)( void *context );

    /* TODO: args */
    int (*set_frequency)( void *context, int frequency );
    /* TODO: args */
    int (*set_mode)( void *context, int mode );
} radio_provider_plugin;

typedef struct plugin_registry_v1
{
    void (*register_signal_processor)( void *context, const char *name, signal_processor_plugin *p );
    void (*register_radio_provider)( void *context, const char *name, radio_provider_plugin *p );
} plugin_registry;

#define HAMWARE_PLUGIN_REGISTER \
extern void hamware_plugin_v1_init( void *context, struct plugin_registry_v1 *r ) __attribute__((visibility("default"))); \
void hamware_plugin_v1_init( void *context, struct plugin_registry_v1 *r )
