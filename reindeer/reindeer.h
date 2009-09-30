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

#ifndef _REN_REINDEER_H
#define _REN_REINDEER_H

#include <ren/types.h>
#include <ren/impl.h>
#include <ltdl.h>

typedef struct _RenBackendData _RenBackendData;
struct _RenBackendData
{
    GHashTable *updater_table;

    struct
    {
        #define _REN_FUNC(F)\
            _REN_FTP(F) _REN_FNM(F);
        #include <ren/funcs.h>
        #undef _REN_FUNC
    } ren;

    struct
    {
        void (* change_material) (RenReindeer *r,
            RenMaterial *front, RenMaterial *back, void *user_data);
    } object;

    ren_size context_data_size;
    RenContextDataInitFunc context_data_init;
    RenContextDataFiniFunc context_data_fini;

    #define UPDATER_SYMS(Type,type)\
        struct {\
        ren_size type##_context_data_size;\
        Ren##Type##ContextDataInitFunc type##_context_data_init;\
        Ren##Type##ContextDataFiniFunc type##_context_data_fini;\
        Ren##Type##ContextDataUpdateFunc type##_context_data_update;\
        ren_size type##_backend_data_size;\
        Ren##Type##BackendDataInitFunc type##_backend_data_init;\
        Ren##Type##BackendDataFiniFunc type##_backend_data_fini;\
        Ren##Type##BackendDataUpdateFunc type##_backend_data_update;\
        }
    UPDATER_SYMS(Matrix,matrix);
    UPDATER_SYMS(Color,color);
    #undef UPDATER_SYMS
};

struct _RenReindeer
{
    RenBackend *backend;
    GHashTable *updater_table;
    _RenBackendData backend_data;
    /* Hidden: _RenContextData context_data; */
};

struct _RenBackend
{
    ren_uint32 ref_count;
    char *name;
    char *version;
    lt_dlhandle libhandle;

    _RenBackendData *data;
};

typedef void (* _RenRemoveUpdaterFunc) (gpointer object, GList *updater_item);

typedef struct _RenContextUpdater _RenContextUpdater;
struct _RenContextUpdater
{
    RenReindeer *context;
    _RenRemoveUpdaterFunc remove_updater;
    GList *updater_item;
};

typedef struct _RenBackendUpdater _RenBackendUpdater;
struct _RenBackendUpdater
{
    RenBackend *backend;
    _RenRemoveUpdaterFunc remove_updater;
    GList *updater_item;
};

#define CLEAR_UPDATER_LISTS(Type,type,object)\
    G_STMT_START {\
    GList *updater_item;\
    while ((updater_item = (object)->context_updater_list) != NULL)\
    {\
        _Ren##Type##ContextUpdater *context_updater = updater_item->data;\
        RenReindeer *context = context_updater->base.context;\
        g_hash_table_remove (context->updater_table, (object));\
        remove_context_updater ((object), updater_item);\
    }\
    while ((updater_item = (object)->backend_updater_list) != NULL)\
    {\
        _Ren##Type##BackendUpdater *backend_updater = updater_item->data;\
        RenBackend *backend = backend_updater->base.backend;\
        g_hash_table_remove (backend->data->updater_table, (object));\
        remove_backend_updater ((object), updater_item);\
    }\
    } G_STMT_END

#define RETURN_C_OR_B_DATA( Type,type,object,SET_UP_CODE,UPDATE_CODE,\
                            Abc,abc,TABLE,ABC)\
    G_STMT_START {\
    GHashTable *table = TABLE;\
    _Ren##Type##Abc##Updater *abc##_updater =\
        g_hash_table_lookup (table, (object));\
    Ren##Type##Abc##Data *abc##_data = NULL;\
    if (abc##_updater == NULL)\
    {\
        abc##_updater = g_new (_Ren##Type##Abc##Updater, 1);\
        abc##_data = g_malloc (r->backend_data.type##_##abc##_data_size);\
        Ren##Type##Abc##DataInitFunc abc##_data_init =\
            r->backend_data.type##_##abc##_data_init;\
        if (abc##_data_init != NULL)\
            abc##_data_init ((object), abc##_data);\
        GList *item;\
        (object)->abc##_updater_list = item =\
            g_list_prepend ((object)->abc##_updater_list, abc##_updater);\
        g_hash_table_insert (table, (object), abc##_updater);\
        abc##_updater->base.abc = ABC;\
        abc##_updater->base.remove_updater =\
            (_RenRemoveUpdaterFunc) remove_##abc##_updater;\
        abc##_updater->base.updater_item = item;\
        abc##_updater->abc##_data = abc##_data;\
        { SET_UP_CODE }\
    }\
    else\
    {\
        abc##_data = abc##_updater->abc##_data;\
    }\
    Ren##Type##Abc##DataUpdateFunc abc##_data_update =\
        r->backend_data.type##_##abc##_data_update;\
    if (abc##_data_update != NULL)\
    {\
        UPDATE_CODE\
    }\
    return abc##_data;\
    } G_STMT_END
#define RETURN_CONTEXT_DATA(Type,type,object,SET_UP_CODE,UPDATE_CODE)\
    RETURN_C_OR_B_DATA(Type,type,object,SET_UP_CODE,UPDATE_CODE,\
        Context,context,r->updater_table,r)
#define RETURN_BACKEND_DATA(Type,type,object,SET_UP_CODE,UPDATE_CODE)\
    RETURN_C_OR_B_DATA(Type,type,object,SET_UP_CODE,UPDATE_CODE,\
        Backend,backend,r->backend_data.updater_table,r->backend)

#define REMOVE_C_OR_B_UPDATER(Type,type,object,CLEAN_UP_CODE,Abc,abc,FINI)\
    G_STMT_START {\
    _Ren##Type##Abc##Updater *abc##_updater = updater_item->data;\
    Ren##Type##Abc##Data *abc##_data = abc##_updater->abc##_data;\
    Ren##Type##Abc##DataFiniFunc abc##_data_fini = FINI;\
    if (abc##_data_fini != NULL)\
        abc##_data_fini ((object), abc##_data);\
    { CLEAN_UP_CODE }\
    g_free (abc##_data);\
    g_free (abc##_updater);\
    (object)->abc##_updater_list =\
        g_list_delete_link ((object)->abc##_updater_list, updater_item);\
    } G_STMT_END
#define REMOVE_CONTEXT_UPDATER(Type,type,object,CLEAN_UP_CODE)\
    REMOVE_C_OR_B_UPDATER(Type,type,object,CLEAN_UP_CODE,Context,context,\
        context_updater->base.context->backend_data.type##_context_data_fini)
#define REMOVE_BACKEND_UPDATER(Type,type,object,CLEAN_UP_CODE)\
    REMOVE_C_OR_B_UPDATER(Type,type,object,CLEAN_UP_CODE,Backend,backend,\
        backend_updater->base.backend->data->type##_backend_data_fini)

#endif /* _REN_REINDEER_H */
