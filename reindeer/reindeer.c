/*
    This file is part of Reindeer.

    Copyright (C) 2008, 2009, 2010 - Patrik Olsson

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

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include "reindeer.h"

#include <ren/ren.h>
#include <glib.h>
#include <string.h>
#include "filevercmp.h"

#define lt_dlclose(libhandle)\
    ((lt_dlclose (libhandle) != 0) ?\
        (g_message ("LTDL close error: %s", lt_dlerror ()), FALSE) : TRUE)

static ren_bool inited = FALSE;
static ren_bool exited = FALSE;
static gchar **search_paths = NULL;
static GHashTable *backend_table = NULL;

static RenBackend*
backend_get (const char* name, RenError **error);
static ren_bool
backend_use (RenBackend *backend, RenError **error);
static void
backend_unuse (RenBackend *backend);
static void
backend_destroy (RenBackend *backend);

ren_bool
ren_library_init (RenError **error)
{
    g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
    if (inited)
    {
        g_warning ("Reindeer library already initialized");
        return TRUE;
    }
    if (exited)
    {
        g_warning ("Reindeer library should never be reinitialized");
        return FALSE;
    }

    if (lt_dlinit () != 0)
    {
        g_set_error ((GError **) error, REN_ERROR, REN_ERROR_FAILED,
            "LTDL init error: %s", lt_dlerror ());
        return FALSE;
    }

    const gchar *reindeer_path = g_getenv ("REINDEER_PATH");
    if (reindeer_path != NULL)
    {
        static const gchar separator[] = {LT_PATHSEP_CHAR, '\0'};
        search_paths = g_strsplit (reindeer_path, separator, 0);
    }
    if (search_paths == NULL)
    {
        search_paths = g_new (gchar*, 2);
        search_paths[0] = g_strdup (AC_renlibdir);
        search_paths[1] = NULL;
    }

    backend_table = g_hash_table_new_full (
        g_str_hash, g_str_equal, NULL, (GDestroyNotify) backend_destroy);

    _ren_back_data_table_init ();

    inited = TRUE;
    return TRUE;
}

void
ren_library_exit (void)
{
    if (!inited)
        return;

    g_strfreev (search_paths);

    g_hash_table_destroy (backend_table);

    _ren_back_data_table_exit ();

    if (lt_dlexit () != 0)
    {
        g_message ("LTDL exit error on Reindeer library exit: %s",
            lt_dlerror ());
    }

    inited = FALSE;
    exited = TRUE;
}

ren_bool
ren_library_is_inited (void)
{
    return inited;
}

RenReindeer*
ren_reindeer_new (RenBackend *backend, RenError **error)
{
    g_return_val_if_fail (backend != NULL, NULL);
    g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
    if (exited)
    {
        g_critical ("Reindeer library already exited");
        return NULL;
    }
    if (!inited)
    {
        g_critical ("Reindeer library not initialized");
        return NULL;
    }
    if (!backend_use (backend, error))
    {
        g_prefix_error ((GError **) error,
            "Creating Reindeer context failed: ");
        return NULL;
    }

    _RenBackendData *backend_data = backend->data;
    RenReindeer *r = g_new (RenReindeer, 1);
    r->ref_count = 1;
    r->backend = backend;
    memcpy (&(r->backend_data), backend_data, sizeof (_RenBackendData));
    r->back_data = g_malloc0 (backend_data->reindeer_back_data_size);

    RenReindeerInitFunc reindeer_init =
        backend_data->reindeer_init;
    if (reindeer_init != NULL)
        reindeer_init (r, r->back_data);

    return r;
}

RenReindeer*
ren_reindeer_ref (RenReindeer *r)
{
    ++(r->ref_count);
    return r;
}

void
ren_reindeer_unref (RenReindeer *r)
{
    if (!inited || r == NULL)
        return;

    if (--(r->ref_count) > 0)
        return;

    RenReindeerFiniFunc reindeer_fini =
        r->backend_data.reindeer_fini;
    if (reindeer_fini != NULL)
        reindeer_fini (r, r->back_data);
    g_free (r->back_data);

    backend_unuse (r->backend);
    g_free (r);
}

RenBackend*
ren_reindeer_backend (RenReindeer *r)
{
    return r->backend;
}

RenReindeerBackData*
ren_reindeer_back_data (RenReindeer *r)
{
    return r->back_data;
}

RenBackend*
ren_backend_lookup (const char *name, RenError **error)
{
    g_return_val_if_fail (error == NULL || *error == NULL, NULL);
    if (exited)
    {
        g_critical ("Reindeer library already exited");
        return NULL;
    }
    if (!inited)
    {
        g_critical ("Reindeer library not initialized");
        return NULL;
    }

    const char *i = name;
    while (*(++i) != '\0')
    {
        char c = *i;
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '-')
        {
            g_set_error ((GError **) error, REN_ERROR, REN_ERROR_FAILED,
                "Not a valid backend name: `%s'", name);
            return NULL;
        }
    }

    return backend_get (name, error);
}

ren_bool
ren_backend_use (RenBackend *backend, RenError **error)
{
    g_return_val_if_fail (backend != NULL, FALSE);
    g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
    return backend_use (backend, error);
}

void
ren_backend_unuse (RenBackend *backend)
{
    g_return_if_fail (backend != NULL);
    backend_unuse (backend);
}

void
ren_error_free (RenError *error)
{
    g_error_free ((GError *) error);
}

GQuark
ren_error_quark_ (void)
{
  return g_quark_from_static_string ("ren-error-quark");
}

const ren_uint32 ren_major_version = REN_MAJOR_VERSION;
const ren_uint32 ren_minor_version = REN_MINOR_VERSION;
const ren_uint32 ren_micro_version = REN_MICRO_VERSION;
const char *ren_compat_version = REN_COMPAT_VERSION;

#define _REN_FUNC(F)\
    _REN_RET(F) _REN_SYM(F) _REN_PRM(F) {\
        return r->backend_data.ren._REN_FNM(F) _REN_ARG(F);\
    }
#include <ren/funcs.h>
#undef _REN_FUNC

static inline void*
backend_symbol (lt_dlhandle libhandle,
    const char *backend_name, const char* name)
{
    char *symbol_name = g_strjoin ("__", "ren", backend_name, name, NULL);
    void *symbol = lt_dlsym (libhandle, symbol_name);
    g_free (symbol_name);
    return symbol;
}

static ren_bool
backend_link (RenBackend *backend,
    lt_dlhandle libhandle, const gchar *filename)
{
    const gchar *name = backend->name;

    RenBackendInitFunc backend_init =
        backend_symbol (libhandle, name, "backend_init");
    if (backend_init != NULL && !backend_init (backend))
    {
        g_message ("Reindeer backend `%s' (%s) initialization failed",
            name, filename);
        return FALSE;
    }

    _RenBackendData *data = g_new (_RenBackendData, 1);
    ren_bool loading_error = FALSE;

    #define XSTR(s) STR(s)
    #define STR(s) #s
    #define LOAD_SYMBOL(func)\
        data->ren.func = backend_symbol (libhandle, name, XSTR(func));\
        if (data->ren.func == NULL)\
        {\
            g_message ("Function `%s' could not be found in `%s'.",\
                XSTR(func), filename);\
            loading_error = TRUE;\
        }
    #define _REN_FUNC(F)\
        LOAD_SYMBOL(_REN_FNM(F))
    #include <ren/funcs.h>
    #undef _REN_FUNC
    #undef LOAD_SYMBOL
    #undef STR
    #undef XSTR

    #define LOAD_SYMBOL(sym)\
        data->object.sym = backend_symbol (libhandle, name, "object_" #sym);
    LOAD_SYMBOL(change_material)
    #undef LOAD_SYMBOL

    #define LOAD_SYMBOL(sym)\
        data->sym = backend_symbol (libhandle, name, #sym);\
        if (data->sym == NULL)\
        {\
            g_message ("Symbol `%s' could not be found in `%s'.",\
                #sym, filename);\
            loading_error = TRUE;\
        }
    #define LOAD_SIZE(sym)\
        {\
        ren_size *sizep = NULL;\
        if ((sizep = backend_symbol (libhandle, name, #sym)) != NULL)\
            data->sym = *sizep;\
        else\
        {\
            g_message ("Symbol `%s' could not be found in `%s'.",\
                #sym, filename);\
            loading_error = TRUE;\
        }\
        }
    LOAD_SYMBOL(reindeer_init)
    LOAD_SYMBOL(reindeer_fini)
    LOAD_SIZE(reindeer_back_data_size)
    #undef LOAD_SIZE
    #undef LOAD_SYMBOL

    if (loading_error)
    {
        g_free (data);
        return FALSE;
    }

    backend->data = data;
    return TRUE;
}

static gint
lib_compare (gconstpointer a, gconstpointer b, gpointer user_data)
{
    const char *name_a = g_strrstr (a, "/");
    if (name_a == NULL)
        name_a = a;
    const char *name_b = g_strrstr (b, "/");
    if (name_b == NULL)
        name_b = b;
    return -filevercmp (name_a, name_b);
}

static ren_bool
backend_load (RenBackend *backend, ren_bool fully, RenError **error)
{
    char *libname = g_strconcat ("libreindeer", "-", backend->name, ".", NULL);
    GSequence *files = g_sequence_new (g_free);

    const gchar *path;
    gint i = 0;
    for (path = search_paths[i]; path != NULL; path = search_paths[++i])
    {
        GDir *dir = g_dir_open (path, 0, NULL);
        if (dir == NULL)
            continue;
        const gchar *filename;
        while ((filename = g_dir_read_name (dir)) != NULL)
        {
            if (g_str_has_prefix (filename, libname) &&
                g_str_has_suffix (filename, LT_MODULE_EXT))
            {
                gchar *file = g_strdup_printf ("%s/%s",
                    path, filename);
                g_sequence_insert_sorted (files, file, lib_compare, NULL);
            }
        }
        g_dir_close (dir);
    }

    if (g_sequence_get_length (files) == 0)
        goto FAIL;

    GSequenceIter *iter = g_sequence_get_begin_iter (files);
    while (!g_sequence_iter_is_end (iter))
    {
        const gchar *filename = g_sequence_get (iter);

        lt_dlhandle libhandle = lt_dlopenext (filename);
        if (libhandle == NULL)
        {
            g_message ("Could not load library `%s': %s",
                filename, lt_dlerror ());
            goto BAD_FILE;
        }

        const char **compat = backend_symbol (libhandle,
            backend->name, "compat_version");
        if (compat == NULL)
        {
            g_message ("Library `%s' is missing compatibility information",
                filename);
            goto BAD_FILE;
        }
        if (strcmp (*compat, ren_compat_version) != 0)
        {
            g_message ("Library `%s' is not compatible (%s instead of %s)",
                filename, *compat, ren_compat_version);
            goto BAD_FILE;
        }

        if (fully && !backend_link (backend, libhandle, filename))
            goto BAD_FILE;

        backend->libhandle = libhandle;
        backend->filename = g_strdup (filename);
        break;

        BAD_FILE:
        if (libhandle) lt_dlclose (libhandle);
        iter = g_sequence_iter_next (iter);
        continue;
    }
    if (g_sequence_iter_is_end (iter))
        goto FAIL;

    g_free (libname);
    g_sequence_free (files);
    return TRUE;

    FAIL:
    g_free (libname);
    g_sequence_free (files);
    g_set_error ((GError **) error, REN_ERROR, REN_ERROR_FAILED,
        "Could not load Reindeer backend `%s'", backend->name);
    return FALSE;
}

static RenBackend*
backend_get (const char* name, RenError **error)
{
    RenBackend *backend = g_hash_table_lookup (backend_table, name);
    if (backend != NULL)
    {
        if (backend_use (backend, error))
            return backend;
        else
            return NULL;
    }

    backend = g_new0 (RenBackend, 1);
    backend->name = g_strdup (name);

    if (!backend_use (backend, error))
        goto FAIL;

    g_hash_table_insert (backend_table, backend->name, backend);

    return backend;

    FAIL:
    if (backend)
    {
        g_free (backend->name);
        g_free (backend);
    }
    return NULL;
}

static ren_bool
backend_use (RenBackend *backend, RenError **error)
{
    if (++(backend->use_count) != 1)
        return TRUE;

    if (!backend_load (backend, TRUE, error))
    {
        --(backend->use_count);
        return FALSE;
    }

    return TRUE;
}

static void
backend_unuse (RenBackend *backend)
{
    if (--(backend->use_count) > 0)
        return;

    RenBackendFiniFunc backend_fini =
        backend_symbol (backend->libhandle, backend->name, "backend_fini");
    if (backend_fini != NULL && !backend_fini ())
        g_message ("Reindeer backend finalization failed");

    lt_dlclose (backend->libhandle);

    backend->libhandle = 0;
    g_free (backend->data);
    backend->data = NULL;
}

static void
backend_destroy (RenBackend *backend)
{
    if (backend->use_count > 0)
    {
        backend->use_count = 1;
        backend_unuse (backend);
    }
    g_free (backend->name);
    g_free (backend);
}
