/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#include "configuration.h"
#include "hamware_config.h"

////////////////////////////////////////

typedef struct hamware_plugin_base
{
    char *name;

    void (*init_plug_library)();
    void (*shutdown_plug_library)();
} plugin_entry_base;

typedef struct hamware_signal_processor
{
    plugin_entry_base info;

    /* TODO: this needs to change to provide a send-to-the-next processor / receiver */
    int (*process_audio_data)( void *context, uint8_t *sampledata, uint64_t samplebytes );
} signal_processor;

typedef struct hamware_radio_provider
{
    plugin_entry_base info;

    /* TODO: revisit arguments */

    /* need a config? */
    void *(*create)();
    void (*destroy)( void *context );

    int (*set_configuration)( void *context, hamware_config_entry_t config, const char *val );

    int (*open_radio)( void *context );
    void (*close_radio)( void *context );

    int (*set_frequency)( void *context, int freq );
    int (*set_mode)( void *context, int mode );
    /* TODO: more control functions */
} radio_provider;

typedef struct plugin_provider_context_v1
{
    char *dso_path;
    void *dso;
} plugin_provider;

typedef struct hamware_plugin_list_v1
{
    int num_signal_processors;
    signal_processor *signal_processors;

    int num_radio_providers;
    radio_provider *radio_providers;

    int num_plugins;
    plugin_provider *providers;
} plugin_store;

extern int load_plugins( configuration *conf, plugin_store *plugs );
extern void destroy_plugins( plugin_store *plugs );

