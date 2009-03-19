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

#ifndef _REN_BACKEND_H
#define _REN_BACKEND_H

struct _RenBackend
{
    char *name;
    char *nicename;
    char *desc;
    char *version;

    ren_uint ref;
    lt_dlhandle libhandle;
    struct _RenFuncTable *ft;
};

static _RenSList *backend_list = NULL;

static lt_dlhandle
backend_load (const char *name)
{
    char *libname = NULL;
    asprintf (&libname, "libreindeer-%s", name);
    if (!libname)
    {
        /* TODO: Add errors for reindeer_error */
        goto FAIL;
    }

    /* FIXME: Make sure it checks the libtool version of the library?  */
    lt_dlerror ();
    lt_dlhandle libhandle = lt_dlopenext (libname);
    const char *errorstr = lt_dlerror ();
    if (errorstr)
    {
        /* FIXME: Does not seems like lt_dlerror gives anything if the library
        was not found.  */
        _ren_throw_error ("%s", errorstr);
        goto FAIL;
    }
    if (!libhandle)
    {
        _ren_throw_error ("Could not open backend library for %s", name);
        goto FAIL;
    }

    free (libname);
    return libhandle;

    FAIL:
    if (libname) free (libname);
    return NULL;
}

static RenBackend*
backend_get (const char* name)
{
    const char *i = name;
    while (*(++i) != '\0')
    {
        if (*i < 'a' || *i > 'z')
        {
            _ren_throw_error ("Not a valid backend name: %s", name);
            return NULL;
        }
    }

    if (backend_list != NULL)
    {
        _RenSList *first = backend_list;
        do
        {
            RenBackend *backend =
                (RenBackend *) backend_list->data;
            if (strcmp (backend->name, name) == 0)
                return backend;
            backend_list = backend_list->next;
        } while (backend_list != first);
    }

    lt_dlhandle libhandle = 0;
    RenBackend *backend = NULL;
    _RenSList *newitem = NULL;

    libhandle = backend_load (name);
    if (!libhandle)
        goto FAIL;

    backend = calloc (1, sizeof (struct _RenBackend));
    if (!backend)
        goto FAIL;
    backend->name = strdup (name);
    if (!backend->name)
        goto FAIL;
    /*
    backend->name = strdup (lt_dlsym (libhandle, "reindeer_backend_desc"));
    backend->desc = strdup (lt_dlsym (libhandle, "reindeer_backend_version"));
    */

    newitem = (_RenSList *) malloc (sizeof (_RenSList));
    if (!newitem)
        goto FAIL;
    newitem->data = backend;
    if (backend_list)
    {
        newitem->next = backend_list->next;
        backend_list->next = newitem;
    }
    else
    {
        newitem->next = newitem;
    }
    backend_list = newitem;

    lt_dlerror ();
    if (lt_dlclose (libhandle) != 0)
    {
        _ren_throw_error ("%s", lt_dlerror ());
    }
    return backend;

    FAIL:
    if (libhandle) lt_dlclose (libhandle);
    if (backend)
    {
        if (backend->name) free (backend->name);
        if (backend->name) free (backend->name);
        if (backend->desc) free (backend->desc);
        free (backend);
    }
    if (newitem) free (newitem);
    _ren_throw_error ("Could not load backend");
    return NULL;
}

static void
backend_search ()
{
    int func (const char *filename, void * data)
    {
        /* TODO */
    }
    lt_dlforeachfile (lt_dlgetsearchpath (), func, NULL);
}

static inline void*
backend_symbol (lt_dlhandle libhandle, const char *name, const char* func)
{
    char *symbolname = NULL;
    asprintf (&symbolname, "ren__%s__%s", name, func);
    void *symbol = lt_dlsym (libhandle, symbolname);
    free (symbolname);
    return symbol;
}

static ren_bool
backend_ref (RenBackend *backend)
{
    if (++(backend->ref) != 1)
        return REN_TRUE;

    lt_dlhandle libhandle = NULL;
    struct _RenFuncTable *ft = NULL;

    const char *name = backend->name;

    libhandle = backend_load (name);
    if (!libhandle)
    {
        /* TODO: Add errors for reindeer_error */
        goto FAIL;
    }

    RenBackendInitFunc *backend_init =
        backend_symbol (libhandle, name, "backend_init");
    if (backend_init != NULL)
        if (!backend_init (backend))
        {
            _ren_throw_error ("Backend initialization failed");
            goto FAIL;
        }

    ft = (struct _RenFuncTable *) calloc (1, sizeof (struct _RenFuncTable));
    if (!ft)
    {
        /* TODO: Add errors for reindeer_error */
        goto FAIL;
    }

    ren_bool loading_error = REN_FALSE;
    #define XSTR(s) STR(s)
    #define STR(s) #s
    #define LOAD_SYMBOL(func,type)\
        if ((ft->func = (type *)\
            backend_symbol (libhandle, name, XSTR(func))) == NULL)\
        {\
            _ren_throw_error ("Function %s could not be found.", XSTR(func));\
            loading_error = REN_TRUE;\
        }
    #define _REN_FUNC(F)\
        LOAD_SYMBOL(_REN_FNM(F), _REN_FTP(F))
    #define _REN_FUNC_T(F, T)\
        LOAD_SYMBOL(_REN_FNM_T(F, T), _REN_FTP_T(F, T))
    #define _REN_FUNC_TN(F, T, N)\
        LOAD_SYMBOL(_REN_FNM_TN(F, T, N), _REN_FTP_TN(F, T, N))
    #include <ren/funcs.h>
    #undef LOAD_SYMBOL
    #undef STR
    #undef XSTR
    #undef MANGLE
    if (loading_error)
        goto FAIL;

    backend->libhandle = libhandle;
    backend->ft = ft;

    return REN_TRUE;

    FAIL:
    --(backend->ref);
    if (libhandle) lt_dlclose (libhandle);
    if (ft) free (ft);
    return REN_FALSE;
}

static void
backend_unref (RenBackend *backend)
{
    if (--(backend->ref) == 0)
    {
        RenBackendFiniFunc *backend_fini =
            backend_symbol (backend->libhandle, backend->name, "backend_fini");
        if (backend_fini != NULL)
            if (!backend_fini ())
            {
                _ren_throw_error ("Backend finalization failed");
            }

        lt_dlerror ();
        if (lt_dlclose (backend->libhandle) != 0)
        {
            _ren_throw_error ("%s", lt_dlerror ());
        }
        backend->libhandle = 0;

        free (backend->ft);
        backend->ft = NULL;
    }
}

static void
backend_free_all (void)
{
    if (backend_list != NULL)
    {
        _RenSList *first = backend_list;
        do
        {
            _RenSList *current = backend_list;
            RenBackend *backend =
                (RenBackend *) current->data;
            if (backend->ref > 0)
            {
                _ren_throw_error (
                "Backend is freed when it's still loaded.\n"
                "This is likely because of a bug in Reindeer.");
            }
            free (backend);
            backend_list = current->next;
            free (current);
        } while (backend_list != first);
        backend_list = NULL;
    }
}

#endif /* _REN_BACKEND_H */
