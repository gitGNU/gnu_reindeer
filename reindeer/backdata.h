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

typedef void (* _RenBackDataItemFunc) (_RenBackDataItem *item);

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
In the macros below the parameter Res is the type name in camel case, res is
the type name in lower case and res_o the resource variable.
*/

/*
This macro should in most cases be used in the beginning of a back data
retrieval function for a resource. It will declare and define the variables
'item' and 'data', which can be used in INIT_CODE and after the macro (e.g. data
should eventually be returned in such functions too). Do not use this macro in
single line scopes.
*/
#define _REN_BACK_DATA_GET_OR_NEW(Res,res,res_o,key,INIT_CODE)\
    _Ren##Res##BackDataItem *item = (void *) _ren_back_data_table_get (res_o, key);\
    Ren##Res##BackData *data;\
    if (item == NULL)\
    {\
        data = g_malloc0 (key->size);\
        item = g_new0 (_Ren##Res##BackDataItem, 1);\
        item->base.resource = res_o;\
        item->base.key = key;\
        item->base.data = data;\
        { INIT_CODE }\
        _ren_back_data_table_ins ((_RenBackDataItem *) item);\
        if (res_o->bd_list != NULL)\
            res_o->bd_list->base.prev_by_res = (_RenBackDataItem *) item;\
        res_o->bd_list = item;\
        if (key->bd_list != NULL)\
            key->bd_list->base.prev_by_key = (_RenBackDataItem *) item;\
        key->bd_list = item;\
    }\
    else\
    {\
        data = item->base.data;\
    }

/*
Iterate through the back data associated with a resource and call UPDATE_CODE.
This macro is typically used in "signal change" functions for a resource.
*/
#define _REN_RES_BACK_DATA_LIST_UPDATE(Res,res,res_o,UPDATE_CODE)\
    G_STMT_START {\
    _Ren##Res##BackDataItem *item = res_o->bd_list;\
    while (item != NULL)\
    {\
        Ren##Res##BackDataKey *key = item->base.key;\
        { UPDATE_CODE }\
        item = (_Ren##Res##BackDataItem *) item->base.next_by_res;\
    }\
    } G_STMT_END

/*
Remove all back data associated with a resource. This macro is typically used
when a resource has been unrefed to 0. FINI_CODE can use variables item, key,
data and the resource that was passed to the macro.
*/
#define _REN_RES_BACK_DATA_LIST_CLEAR(Res,res,res_o,FINI_CODE)\
    G_STMT_START {\
    _Ren##Res##BackDataItem *item = res_o->bd_list, *next;\
    while (item != NULL)\
    {\
        Ren##Res##BackDataKey *key = item->base.key;\
        Ren##Res##BackData *data = item->base.data;\
        _ren_back_data_table_del ((_RenBackDataItem *) item);\
        _ren_back_data_item_unlink_by_key (\
            (_RenBackDataItem **) &(key->bd_list), (_RenBackDataItem *) item);\
        { FINI_CODE }\
        next = (_Ren##Res##BackDataItem *) item->base.next_by_res;\
        g_free (item);\
        item = next;\
    }\
    } G_STMT_END

/*
Remove all back data associated with a key. This macro is typically used when a
key has been unrefed to 0. FINI_CODE can use the variables item and data. The
resource variable can be found with the same name as that is passed in res_name.
*/
#define _REN_KEY_BACK_DATA_LIST_CLEAR(Res,res,key,res_name,FINI_CODE)\
    G_STMT_START {\
    _Ren##Res##BackDataItem *item = key->bd_list, *next;\
    while (item != NULL)\
    {\
        Ren##Res *res_name = item->base.resource;\
        Ren##Res##BackData *data = item->base.data;\
        _ren_back_data_table_del ((_RenBackDataItem *) item);\
        _ren_back_data_item_unlink_by_res (\
            (_RenBackDataItem **) &(res_name->bd_list), (_RenBackDataItem *) item);\
        { FINI_CODE }\
        next = (_Ren##Res##BackDataItem *) item->base.next_by_key;\
        g_free (item);\
        item = next;\
    }\
    } G_STMT_END

#endif /* _REN_BACK_DATA_H */ 
