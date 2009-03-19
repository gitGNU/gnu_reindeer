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
#include <ren/impl.h>

#include <stdlib.h>
#include <string.h>
#include <ltdl.h>

#include "reindeer.h"
#include "backend.h"

static ren_bool initialized = REN_FALSE;

static ren_bool
initialize (void)
{
    if (!initialized)
    {
        lt_dlerror ();
        if (lt_dlinit () != 0)
        {
            _ren_throw_error ("%s", lt_dlerror ());
            return REN_FALSE;
        }
    }
    return REN_TRUE;
}

RenReindeer*
ren_reindeer_load (RenBackend *backend)
{
    if (!initialize ()) return NULL;

    if (!backend_ref (backend))
        return NULL;

    RenReindeer *r = NULL;

    r = (RenReindeer *) calloc (1, sizeof (struct _RenReindeer));
    if (!r)
        goto FAIL;

    struct _RenFuncTable *backend_ft = backend->ft;
    #define SET_FUNCTION(f) r->ft.f = backend_ft->f;
    #define _REN_FUNC(F) SET_FUNCTION(_REN_FNM(F))
    #define _REN_FUNC_T(F, T) SET_FUNCTION(_REN_FNM_T(F, T))
    #define _REN_FUNC_TN(F, T, N) SET_FUNCTION(_REN_FNM_TN(F, T, N))
    #include <ren/funcs.h>
    #undef SET_FUNCTION

    /* TODO: Implement the share mechanism.  */

    r->backend = backend;

    if (!ren_init (r))
    {
        _ren_throw_error ("Reindeer initialization failed");
        goto FAIL;
    }

    //r->listitem = reindeer_list = _ren_dlist_prepend (reindeer_list, r);

    return r;

    FAIL:
    backend_unref (backend);
    if (r) free (r);
    return NULL;
}

void
ren_reindeer_unload (RenReindeer *r)
{
    if (!ren_fini (r))
        _ren_throw_error ("Reindeer finalization failed");
    backend_unref (r->backend);
    //reindeer_list = _ren_dlist_delete_link (reindeer_list, r->listitem);
    free (r);
}

void
ren_clean_up (void)
{
    /*
    if (reindeer_list != NULL)
    {
        _ren_throw_error ("Calling reindeer_fini with reindeers still "
        "loaded. Forcing unload...");
        while (reindeer_list)
            reindeer_unload (reindeer_list->data);
        res = REN_FALSE;
    }
    */

    backend_free_all ();

    if (initialized)
    {
        lt_dlerror ();
        if (lt_dlexit () != 0)
        {
            _ren_throw_error ("%s", lt_dlerror ());
        }
    }
}

RenBackend*
ren_backend_find (const char *name)
{
    if (initialize ())
        return backend_get (name);
    else
        return NULL;
}

ren_uint
ren_backend_list (ren_size count, const char *names[], RenBackend *result[])
{
    if (!initialize ())
        return 0;
    ren_bool res = 0;
    int i;
    for (i = 0; i < count; ++i)
    {
        RenBackend *backend = backend_get (names[i]);
        result[i] = backend;
        if (backend != NULL)
            ++res;
    }
    return res;
}

RenBackend*
ren_backend (RenReindeer *r)
{
    return r->backend;
}

void*
_ren_get_backend_data (RenReindeer *r/*, RenBackend *backend*/)
{
    return r->backend_data;
}

void
_ren_set_backend_data (RenReindeer *r/*, RenBackend *backend*/, void *data)
{
    r->backend_data = data;
}

#define _REN_FUNC(F)\
    _REN_RET(F) _REN_SYM(F) _REN_PRM(F) {\
        return r->ft._REN_FNM(F) _REN_ARG(F);\
    }
#define _REN_FUNC_T(F, T)\
    _REN_RET_T(F, T) _REN_SYM_T(F, T) _REN_PRM_T(F, T) {\
        return r->ft._REN_FNM_T(F, T) _REN_ARG(F);\
    }
#define _REN_FUNC_TN(F, T, N)\
    _REN_RET_TN(F, T, N) _REN_SYM_TN(F, T, N) _REN_PRM_TN(F, T, N) {\
        return r->ft._REN_FNM_TN(F, T, N) _REN_ARG(F);\
    }
#include <ren/funcs.h>
