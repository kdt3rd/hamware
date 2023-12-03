/*
 * Copyright (c) 2023 Authors of hamware
 * SPDX-License-Identifier: MIT
 */

#include "plugins.h"
#include "plugin_register.h"

#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct register_context
{
    plugin_store *plugs;
    int num_registered;
};

static void add_signal_proc( void *context, const char *name, struct signal_processor_plugin_v1 *sp )
{
    struct register_context *ctxt = context;

    if ( ctxt && name && sp )
    {
        plugin_store *plugs = ctxt->plugs;
        signal_processor *nlist = malloc( sizeof(signal_processor) * ( plugs->num_signal_processors + 1 ) );
        if ( nlist )
        {
            signal_processor *newent;
            if ( plugs->signal_processors )
            {
                memcpy( nlist, plugs->signal_processors,
                        sizeof(signal_processor) * plugs->num_signal_processors );
                free( plugs->signal_processors );
            }

            newent = nlist + plugs->num_signal_processors;
            memset( newent, 0, sizeof(signal_processor) );

            newent->info.name = strdup( name );

            /* TODO */
            newent->info.init_plug_library = NULL;
            newent->info.shutdown_plug_library = NULL;
            newent->process_audio_data = NULL;

            printf( "\nRegistered signal processing plugin '%s'\n", name );

            plugs->signal_processors = nlist;
            plugs->num_signal_processors = plugs->num_signal_processors + 1;

            ctxt->num_registered++;
        }
    }
}

static void add_radio_prov( void *context, const char *name, struct radio_provider_plugin_v1 *rp )
{
    struct register_context *ctxt = context;

    if ( ctxt && name && rp )
    {
        plugin_store *plugs = ctxt->plugs;
        radio_provider *nlist = malloc( sizeof(radio_provider) * ( plugs->num_radio_providers + 1 ) );
        if ( nlist )
        {
            radio_provider *newent;
            if ( plugs->radio_providers )
            {
                memcpy( nlist, plugs->radio_providers,
                        sizeof(radio_provider) * plugs->num_radio_providers );
                free( plugs->radio_providers );
            }

            newent = nlist + plugs->num_radio_providers;
            memset( newent, 0, sizeof(radio_provider) );

            newent->info.name = strdup( name );

            newent->info.init_plug_library = rp->init_library;
            newent->info.shutdown_plug_library = rp->shutdown_library;
            newent->create = rp->create_radio;
            newent->destroy = rp->destroy_radio;
            newent->open_radio = rp->open_radio;
            newent->close_radio = rp->close_radio;
            newent->set_frequency = rp->set_frequency;
            newent->set_mode = rp->set_mode;

            printf( "\nRegistered radio provider plugin '%s'\n", name );

            plugs->radio_providers = nlist;
            plugs->num_radio_providers = plugs->num_radio_providers + 1;

            ctxt->num_registered++;
        }
    }
}

static int run_plugin_register_v1( plugin_store *plugs, void *reg )
{
    struct plugin_registry_v1 preg =
        {
            .register_signal_processor = add_signal_proc,
            .register_radio_provider = add_radio_prov,
        };
    struct register_context ctxt = { plugs, 0 };
    void (*regfunc)( void *, struct plugin_registry_v1 * ) = reg;

    (*regfunc)( &ctxt, &preg );

    return ctxt.num_registered;
}

static plugin_provider *add_provider( plugin_store *plugs, void *plug )
{
    plugin_provider *ret = NULL;
    void *reg = dlsym( plug, "hamware_plugin_v1_init" );
    if ( reg && run_plugin_register_v1( plugs, reg ) > 0 )
    {
        plugin_provider *nlist = malloc( sizeof(plugin_provider) * ( plugs->num_plugins + 1 ) );
        if ( nlist )
        {
            if ( plugs->providers )
            {
                memcpy( nlist, plugs->providers, sizeof(plugin_provider) * plugs->num_plugins );
                free( plugs->providers );
            }

            ret = nlist + plugs->num_plugins;

            plugs->providers = nlist;
            plugs->num_plugins = plugs->num_plugins + 1;
        }
    }

    return ret;
}

int load_plugins( configuration *conf, plugin_store *plugs )
{
    if ( conf && plugs )
    {
        DIR *pdir;
        struct dirent *ent;

        memset( plugs, 0, sizeof(plugin_store) );

        pdir = opendir( conf->plugin_dir );
        if ( ! pdir )
        {
            printf( "Unable to open plugin directory '%s'\n", conf->plugin_dir );
            return 1;
        }

        ent = readdir( pdir );
        while ( ent )
        {
            size_t nlen = strlen( ent->d_name );
            if ( nlen > 3 )
            {
                if ( ent->d_name[nlen - 3] == '.' &&
                     ent->d_name[nlen - 2] == 's' &&
                     ent->d_name[nlen - 1] == 'o' )
                {
                    char path[PATH_MAX];
                    void *plug;
                    /* should use snpritnf technically but meh */
                    sprintf( path, "%s/%s", conf->plugin_dir, ent->d_name );

                    plug = dlopen( path, RTLD_LOCAL | RTLD_NOW );
                    if ( plug )
                    {
                        plugin_provider *newprov = add_provider( plugs, plug );
                        if ( newprov )
                        {
                            newprov->dso_path = strdup( path );
                            newprov->dso = plug;
                        }
                        else
                        {
                            dlclose( plug );
                        }
                    }
                    else
                    {
                        printf( "Unable to open plugin '%s': %s\n", path, dlerror() );
                    }
                }
            }
            ent = readdir( pdir );
        }

        closedir( pdir );

        for ( int p = 0; p < plugs->num_signal_processors; ++p )
        {
            if ( plugs->signal_processors[p].info.init_plug_library )
                plugs->signal_processors[p].info.init_plug_library();
        }
        for ( int p = 0; p < plugs->num_radio_providers; ++p )
        {
            if ( plugs->radio_providers[p].info.init_plug_library )
                plugs->radio_providers[p].info.init_plug_library();
        }
        return 0;
    }
    return 1;
}

void destroy_plugins( plugin_store *plugs )
{
    if ( plugs )
    {
        for ( int p = 0; p < plugs->num_signal_processors; ++p )
        {
            plugs->signal_processors[p].info.shutdown_plug_library();
            free( plugs->signal_processors[p].info.name );
        }

        if ( plugs->signal_processors )
            free( plugs->signal_processors );

        for ( int p = 0; p < plugs->num_radio_providers; ++p )
        {
            plugs->radio_providers[p].info.shutdown_plug_library();
            free( plugs->radio_providers[p].info.name );
        }

        if ( plugs->radio_providers )
            free( plugs->radio_providers );

        for ( int p = 0; p < plugs->num_plugins; ++p )
        {
            dlclose( plugs->providers[p].dso );
        }

        if ( plugs->providers )
            free( plugs->providers );
    }
}


