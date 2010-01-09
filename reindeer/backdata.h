/*
    This file is part of Reindeer.

    Copyright (C) 2009 - Patrik Olsson

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

#ifndef _REN_BACK_DATA_H
#define _REN_BACK_DATA_H

typedef struct _RenBackDataItem _RenBackDataItem;
struct _RenBackDataItem
{
    void *resource;
    void *key;
    void *data;
    _RenBackDataItem *prev_by_res;
    _RenBackDataItem *next_by_res;
    _RenBackDataItem *prev_by_key;
    _RenBackDataItem *next_by_key;
    _RenBackDataItem *prev_by_hash;
    _RenBackDataItem *next_by_hash;
};

void
_ren_back_data_table_init ();

void
_ren_back_data_table_exit ();

_RenBackDataItem*
_ren_back_data_table_get (void *resource, void *key);

void
_ren_back_data_table_ins (_RenBackDataItem *item);

void
_ren_back_data_table_del (_RenBackDataItem *item);

void
_ren_back_data_item_unlink_by_res (_RenBackDataItem **list, _RenBackDataItem *item);

void
_ren_back_data_item_unlink_by_key (_RenBackDataItem **list, _RenBackDataItem *item);

/*
In the macros below the parameter [ResName] is the type name in camel case (e.g.
DataBlock), [res_name] is the type name in lower case (e.g. data_block), [res]
is the resource variable (usually the same as [res_name]), and [key] is the
key variable.

They depend on a few things. The resource struct and key struct need a field
called "bd_list" which is a pointer to the derived back data item type for the
resource. The key-struct also needs a field called "data_size" which is of type
"ren_size" and needs to be set at key construction to the size of the back data
for that key (this is usually specified in a parameter to the key constructor).
The derived back data item type needs to have its first field called "base" and
be of type "_RenBackDataItem" (to inherit all the fields of that struct).
*/

/*
This macro should in most cases be used in the back data retrieval function for
a resource. INIT_FUNC is called if a back data item had to be created,
and is of type "void INIT_FUNC (res, key, item, data)". UPDATE_FUNC is called
just before returning the data and is of the same type as INIT_FUNC.
*/
#define _REN_BACK_DATA_RETURN(ResName,res_name,res,key,INIT_FUNC,UPDATE_FUNC)\
    G_STMT_START {\
    _Ren##ResName##BackDataItem *item = (void *) _ren_back_data_table_get (res, key);\
    Ren##ResName##BackData *data;\
    if (item == NULL)\
    {\
        data = g_malloc0 (key->data_size);\
        item = g_new0 (_Ren##ResName##BackDataItem, 1);\
        item->base.resource = res;\
        item->base.key = key;\
        item->base.data = data;\
        INIT_FUNC (res, key, item, data);\
        _ren_back_data_table_ins ((_RenBackDataItem *) item);\
        if (res->bd_list != NULL)\
            res->bd_list->base.prev_by_res = (_RenBackDataItem *) item;\
        res->bd_list = item;\
        if (key->bd_list != NULL)\
            key->bd_list->base.prev_by_key = (_RenBackDataItem *) item;\
        key->bd_list = item;\
    }\
    else\
    {\
        data = item->base.data;\
    }\
    UPDATE_FUNC (res, key, item, data);\
    return data;\
    } G_STMT_END

/*
Iterate through the back data associated with a resource and for each item call
FUNC, which is of type "void FUNC (res, key, item)". This macro is typically
used in functions that signal changes in the application owned part of a
resource.
*/
#define _REN_RES_BACK_DATA_LIST_ITERATE(ResName,res_name,res,ITER_FUNC)\
    G_STMT_START {\
    _Ren##ResName##BackDataItem *item = res->bd_list;\
    while (item != NULL)\
    {\
        Ren##ResName##BackDataKey *key = item->base.key;\
        ITER_FUNC (res, key, item);\
        item = (_Ren##ResName##BackDataItem *) item->base.next_by_res;\
    }\
    } G_STMT_END

/*
Remove all back data associated with a resource. This macro is typically used
when a resource has been unrefed to 0. FINI_FUNC is called for each back data
item, and is of the same type as INIT_FUNC in _REN_BACK_DATA_RETURN.
*/
#define _REN_RES_BACK_DATA_LIST_CLEAR(ResName,res_name,res,FINI_FUNC)\
    G_STMT_START {\
    _Ren##ResName##BackDataItem *item = res->bd_list, *next;\
    while (item != NULL)\
    {\
        Ren##ResName##BackDataKey *key = item->base.key;\
        Ren##ResName##BackData *data = item->base.data;\
        _ren_back_data_table_del ((_RenBackDataItem *) item);\
        _ren_back_data_item_unlink_by_key (\
            (_RenBackDataItem **) &(key->bd_list), (_RenBackDataItem *) item);\
        FINI_FUNC (res, key, item, data);\
        g_free (data);\
        next = (_Ren##ResName##BackDataItem *) item->base.next_by_res;\
        g_free (item);\
        item = next;\
    }\
    } G_STMT_END

/*
Remove all back data associated with a key. This macro is typically used when a
key has been unrefed to 0. FINI_FUNC is called for each back data item, and
is of the same type as INIT_FUNC in _REN_BACK_DATA_RETURN.
*/
#define _REN_KEY_BACK_DATA_LIST_CLEAR(ResName,res_name,key,FINI_FUNC)\
    G_STMT_START {\
    _Ren##ResName##BackDataItem *item = key->bd_list, *next;\
    while (item != NULL)\
    {\
        Ren##ResName *res = item->base.resource;\
        Ren##ResName##BackData *data = item->base.data;\
        _ren_back_data_table_del ((_RenBackDataItem *) item);\
        _ren_back_data_item_unlink_by_res (\
            (_RenBackDataItem **) &(res->bd_list), (_RenBackDataItem *) item);\
        FINI_FUNC (res, key, item, data);\
        g_free (data);\
        next = (_Ren##ResName##BackDataItem *) item->base.next_by_key;\
        g_free (item);\
        item = next;\
    }\
    } G_STMT_END

/*
The following macros implement common init, fini, update and iter functions for
resource types that only keep track of any change at all. They require the
key struct to have three more fields:
void (* init) (Ren##ResName *res, Ren##ResName##BackData *data);
void (* fini) (Ren##ResName *res, Ren##ResName##BackData *data);
void (* update) (Ren##ResName *res, Ren##ResName##BackData *data);
Additionally, the derived back data item struct needs a field called "changed"
of type "ren_bool".
*/
#define _REN_BACK_DATA_SIMPLE_INIT_FUNC(res, key, item, data)\
    G_STMT_START {\
        if (key->init != NULL)\
            key->init (res, data);\
        if (key->update != NULL)\
            item->changed = TRUE;\
    } G_STMT_END

#define _REN_BACK_DATA_SIMPLE_FINI_FUNC(res, key, item, data)\
    G_STMT_START {\
        if (key->fini != NULL)\
            key->fini (res, data);\
    } G_STMT_END

#define _REN_BACK_DATA_SIMPLE_UPDATE_FUNC(res, key, item, data)\
    G_STMT_START {\
        if (key->update != NULL && item->changed)\
        {\
            key->update (res, data);\
            item->changed = FALSE;\
        }\
    } G_STMT_END

/* Used in ITERATE macro in the res_name##_changed() function.  */
#define _REN_BACK_DATA_SIMPLE_CHANGED_FUNC(res, key, item)\
    G_STMT_START {\
        if (key->update != NULL)\
            item->changed = TRUE;\
    } G_STMT_END

#endif /* _REN_BACK_DATA_H */ 
