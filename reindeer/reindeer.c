/*
    This file is part of Reindeer.

    Copyright (C) 2008, 2009 - Patrik Olsson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ren/ren.h>
#include <glib.h>
#include <string.h>

#include "reindeer.h"

#define GET_CONTEXT_DATA(r) ((_RenContextData*)((r) + sizeof (RenReindeer)))

static GHashTable *backend_table = NULL;
static ren_bool initialized = FALSE;

static RenBackend*
backend_get (const char* name);
static ren_bool
backend_ref (RenBackend *backend);
static void
backend_unref (RenBackend *backend);
static void
backend_destroy (RenBackend *backend);

ren_bool
ren_library_init (void)
{
    if (initialized)
        return TRUE;

    if (lt_dlinit () != 0)
    {
        g_warning ("%s", lt_dlerror ());
        return FALSE;
    }

    const char *renlibdir_path = AC_libdir "/reindeer";
    const char *reindeer_path = g_getenv ("REINDEER_PATH");
    char *search_paths;
    if (reindeer_path)
    {
        search_paths = g_strjoin ((gchar[]){LT_PATHSEP_CHAR, '\0'},
            reindeer_path, renlibdir_path, NULL);
    }
    else
    {
        search_paths = g_strdup (renlibdir_path);
    }
    if (lt_dlsetsearchpath (search_paths) != 0)
    {
        g_free (search_paths);
        g_warning ("LTDL error on setting search path: %s", lt_dlerror ());
        return FALSE;
    }
    g_free (search_paths);

    backend_table = g_hash_table_new_full (
        g_str_hash, g_str_equal, NULL, (GDestroyNotify) backend_destroy);

    initialized = TRUE;
    return TRUE;
}

void
ren_library_exit (void)
{
    if (!initialized)
        return;

    g_hash_table_destroy (backend_table);

    if (lt_dlexit () != 0)
    {
        g_warning ("LTDL error on exit: %s", lt_dlerror ());
    }
}

RenReindeer*
ren_load (RenBackend *backend)
{
    if (!initialized || backend == NULL || !backend_ref (backend))
        return NULL;

    _RenBackendData *backend_data = backend->data;
    RenReindeer *r = g_malloc (
        sizeof (RenReindeer) + backend_data->context_data_size);

    r->backend = backend;
    r->updater_table = g_hash_table_new (g_direct_hash, g_direct_equal);
    memcpy (&(r->backend_data), backend_data, sizeof (_RenBackendData));
    _RenContextDataInitFunc context_data_init =
        backend_data->context_data_init;
    if (context_data_init != NULL)
        context_data_init (GET_CONTEXT_DATA (r));

    return r;
}

void
ren_unload (RenReindeer *r)
{
    if (!initialized || r == NULL)
        return;

    GHashTableIter iter;
    gpointer key;
    _RenContextUpdater *value;
    g_hash_table_iter_init (&iter, r->updater_table);
    while (g_hash_table_iter_next (&iter, &key, (gpointer *) &value)) 
    {
        value->remove_updater (key, value->updater_item);
        g_hash_table_iter_remove (&iter);
    }

    _RenContextDataFiniFunc context_data_fini =
        r->backend_data.context_data_fini;
    if (context_data_fini != NULL)
        context_data_fini (GET_CONTEXT_DATA (r));

    g_hash_table_destroy (r->updater_table);
    backend_unref (r->backend);
    g_free (r);
}

RenBackend*
ren_backend (RenReindeer *r)
{
    return r->backend;
}

_RenContextData*
_ren_context_data (RenReindeer *r)
{
    return GET_CONTEXT_DATA (r);
}

RenBackend*
ren_lookup_backend (const char *name)
{
    if (!initialized)
        return NULL;

    const char *i = name;
    while (*(++i) != '\0')
    {
        if (*i < 'a' || *i > 'z')
        {
            g_warning ("Not a valid backend name: %s", name);
            return NULL;
        }
    }

    return backend_get (name);
}

#define _REN_FUNC(F)\
    _REN_RET(F) _REN_SYM(F) _REN_PRM(F) {\
        return r->backend_data.ren._REN_FNM(F) _REN_ARG(F);\
    }
#include <ren/funcs.h>
#undef _REN_FUNC

static lt_dlhandle
backend_load (const char *name)
{
    char *libname = g_strjoin ("-", "libreindeer", name, NULL);

    lt_dlerror ();
    lt_dlhandle libhandle = lt_dlopenext (libname);
    g_free (libname);
    if (libhandle == NULL)
    {
        g_warning ("Could not load backend `%s': %s",
            name, lt_dlerror ());
        return NULL;
    }
    /* FIXME: Make sure it checks the libtool version of the library?  */

    return libhandle;
}

static RenBackend*
backend_get (const char* name)
{
    RenBackend *backend = g_hash_table_lookup (backend_table, name);
    if (backend != NULL)
        return backend;

    lt_dlhandle libhandle = backend_load (name);
    if (libhandle == NULL)
        goto FAIL;

    backend = g_new0 (RenBackend, 1);
    backend->name = g_strdup (name);

    g_hash_table_insert (backend_table, backend->name, backend);

    if (lt_dlclose (libhandle) != 0)
    {
        g_warning ("LTDL error on closing: %s", lt_dlerror ());
    }
    return backend;

    FAIL:
    if (libhandle) lt_dlclose (libhandle);
    if (backend)
    {
        if (backend->name) g_free (backend->name);
        g_free (backend);
    }
    return NULL;
}

static inline void*
backend_symbol (lt_dlhandle libhandle,
    const char *backend_name, const char* func)
{
    char *symbolname = g_strjoin ("__", "ren", backend_name, func, NULL);
    void *symbol = lt_dlsym (libhandle, symbolname);
    g_free (symbolname);
    return symbol;
}

static ren_bool
backend_ref (RenBackend *backend)
{
    if (++(backend->ref_count) != 1)
        return TRUE;

    lt_dlhandle libhandle = NULL;
    _RenBackendData *data = NULL;

    const char *name = backend->name;

    libhandle = backend_load (name);
    if (libhandle == NULL)
        goto FAIL;

    _RenBackendInitFunc backend_init =
        backend_symbol (libhandle, name, "backend_init");
    if (backend_init != NULL && !backend_init (backend))
    {
        g_warning ("Backend initialization failed");
        goto FAIL;
    }

    data = g_new (_RenBackendData, 1);

    data->updater_table = g_hash_table_new (g_direct_hash, g_direct_equal);

    ren_bool loading_error = FALSE;
    #define XSTR(s) STR(s)
    #define STR(s) #s
    #define LOAD_SYMBOL(func)\
        data->ren.func = backend_symbol (libhandle, name, XSTR(func));\
        if (data->ren.func == NULL)\
        {\
            g_warning ("Function `%s' could not be found.", XSTR(func));\
            loading_error = TRUE;\
        }
    #define _REN_FUNC(F)\
        LOAD_SYMBOL(_REN_FNM(F))
    #include <ren/funcs.h>
    #undef _REN_FUNC
    #undef LOAD_SYMBOL
    #undef STR
    #undef XSTR
    if (loading_error)
        goto FAIL;

    ren_size *sizep = NULL;
    #define LOAD_SIZE(sym)\
        G_STMT_START {\
        if ((sizep = backend_symbol (libhandle, name, #sym)) != NULL)\
            data->sym = *sizep;\
        else\
            data->sym = 0;\
        } G_STMT_END
    #define LOAD_FUNC(sym)\
        data->sym = backend_symbol (libhandle, name, #sym)
    LOAD_SIZE(context_data_size);
    LOAD_FUNC(context_data_init);
    LOAD_FUNC(context_data_fini);
    #define LOAD_UPDATER_SYMS(type)\
        G_STMT_START {\
        LOAD_SIZE(type##_context_data_size);\
        LOAD_FUNC(type##_context_data_init);\
        LOAD_FUNC(type##_context_data_fini);\
        LOAD_FUNC(type##_context_data_update);\
        LOAD_SIZE(type##_backend_data_size);\
        LOAD_FUNC(type##_backend_data_init);\
        LOAD_FUNC(type##_backend_data_fini);\
        LOAD_FUNC(type##_backend_data_update);\
        } G_STMT_END
    LOAD_UPDATER_SYMS(matrix);
    LOAD_UPDATER_SYMS(color);
    #undef LOAD_UPDATER_SYMS
    #undef LOAD_FUNC
    #undef LOAD_SIZE

    backend->libhandle = libhandle;
    backend->data = data;

    return TRUE;

    FAIL:
    --(backend->ref_count);
    if (libhandle) lt_dlclose (libhandle);
    if (data) g_free (data);
    return FALSE;
}

static void
backend_unref (RenBackend *backend)
{
    if (--(backend->ref_count) > 0)
        return;

    GHashTableIter iter;
    gpointer key;
    _RenBackendUpdater *value;
    g_hash_table_iter_init (&iter, backend->data->updater_table);
    while (g_hash_table_iter_next (&iter, &key, (gpointer *) &value)) 
    {
        value->remove_updater (key, value->updater_item);
        g_hash_table_iter_remove (&iter);
    }

    _RenBackendFiniFunc backend_fini =
        backend_symbol (backend->libhandle, backend->name, "backend_fini");
    if (backend_fini != NULL && !backend_fini ())
        g_warning ("Backend finalization failed");

    if (lt_dlclose (backend->libhandle) != 0)
        g_warning ("LTDL error on closing: %s", lt_dlerror ());

    backend->libhandle = 0;
    g_hash_table_destroy (backend->data->updater_table);
    g_free (backend->data);
    backend->data = NULL;
}

static void
backend_destroy (RenBackend *backend)
{
    backend->ref_count = 1;
    backend_unref (backend);
    g_free (backend->name);
    g_free (backend);
}
