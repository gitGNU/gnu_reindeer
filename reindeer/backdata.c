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

#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

static GHashTable *bd_table;

void
_ren_back_data_table_init ()
{
    bd_table = g_hash_table_new (g_direct_hash, g_direct_equal);
}

void
_ren_back_data_table_exit ()
{
    g_hash_table_destroy (bd_table);
}

_RenBackDataItem*
_ren_back_data_table_get (void *resource, void *key)
{
    void *hash = (void *) ((ren_size) resource ^ (ren_size) key);
    _RenBackDataItem* item = g_hash_table_lookup (bd_table, hash);
    if (item != NULL)
        item = item->next_by_hash;
    else
        return NULL;
    while (item != NULL && (item->resource != resource || item->key != key))
    {
        item = item->next_by_hash;
    }
    return item;
}

void
_ren_back_data_table_ins (_RenBackDataItem *item)
{
    void *hash = (void *) ((ren_size) item->resource ^ (ren_size) item->key);
    _RenBackDataItem* tmp;
    
    tmp = g_hash_table_lookup (bd_table, hash);
    if (tmp == NULL)
    {
        tmp = g_new0 (_RenBackDataItem, 1);
        g_hash_table_insert (bd_table, hash, tmp);
        tmp->next_by_hash = item;
        item->prev_by_hash = tmp;
    }
    else
    {
        item->prev_by_hash = tmp;
        if (tmp->next_by_hash != NULL)
            tmp->next_by_hash->prev_by_hash = item;
        item->next_by_hash = tmp->next_by_hash;
        tmp->next_by_hash = item;
    }
}

void
_ren_back_data_table_del (_RenBackDataItem *item)
{
    _RenBackDataItem *prev, *next;

    prev = item->prev_by_hash;
    next = item->next_by_hash;
    if (next != NULL)
    {
        prev->next_by_hash = next;
        next->prev_by_hash = prev;
    }
    else if (prev->prev_by_hash == NULL)
    {
        g_free (prev);
        void *hash = (void *) ((ren_size) item->resource ^ (ren_size) item->key);
        g_hash_table_remove (bd_table, hash);
    }
    else
    {
        prev->next_by_hash = next;
    }
}

void
_ren_back_data_item_unlink_by_res (_RenBackDataItem **list, _RenBackDataItem *item)
{
    _RenBackDataItem *prev, *next;

    prev = item->prev_by_res;
    next = item->next_by_res;
    if (prev != NULL)
        prev->next_by_res = next;
    if (next != NULL)
        next->prev_by_res = prev;
    if (item == *list)
        *list = next;
}

void
_ren_back_data_item_unlink_by_key (_RenBackDataItem **list, _RenBackDataItem *item)
{
    _RenBackDataItem *prev, *next;

    prev = item->prev_by_key;
    next = item->next_by_key;
    if (prev != NULL)
        prev->next_by_key = next;
    if (next != NULL)
        next->prev_by_key = prev;
    if (item == *list)
        *list = next;
}
