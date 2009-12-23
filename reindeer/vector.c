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
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenVectorBackDataItem _RenVectorBackDataItem;

struct _RenVector
{
    ren_uint32 ref_count;

    const void *data;
    ren_size length;
    RenType type;

    _RenVectorBackDataItem *bd_list;
};

struct _RenVectorBackDataKey
{
    ren_uint32 ref_count;

    ren_size size;
    RenVectorBackDataInitFunc init;
    RenVectorBackDataFiniFunc fini;
    RenVectorBackDataUpdateFunc update;

    _RenVectorBackDataItem *bd_list;
};

struct _RenVectorBackDataItem
{
    _RenBackDataItem base;
    ren_bool changed;
};

RenVector*
ren_vector_new (const void *data, ren_size length, RenType type)
{
    if (type != REN_TYPE_SFLOAT && type != REN_TYPE_DFLOAT)
        return NULL;

    RenVector *vector = g_new (RenVector, 1);

    vector->ref_count = 1;

    vector->data = data;
    vector->length = length;
    vector->type = type;

    vector->bd_list = NULL;

    return vector;
}

void
ren_vector_destroy (RenVector *vector)
{
    ren_vector_unref (vector);
}

void
ren_vector_changed (RenVector *vector)
{
    _REN_RES_BACK_DATA_LIST_UPDATE (Vector,vector,vector,
        if (key->update != NULL)
            item->changed = TRUE;
    );
}

RenVector*
ren_vector_ref (RenVector *vector)
{
    ++(vector->ref_count);
    return vector;
}

void
ren_vector_unref (RenVector *vector)
{
    if (--(vector->ref_count) > 0)
        return;

    g_free (vector);
}

void
ren_vector_data (RenVector *vector,
    const void **data_p, ren_size *length_p, RenType *type_p)
{
    if (data_p)
        (*data_p) = vector->data;
    if (length_p)
        (*length_p) = vector->length;
    if (type_p)
        (*type_p) = vector->type;
}

RenVectorBackDataKey*
ren_vector_back_data_key_new (ren_size size, RenVectorBackDataInitFunc init,
    RenVectorBackDataFiniFunc fini, RenVectorBackDataUpdateFunc update)
{
    RenVectorBackDataKey *key = g_new (RenVectorBackDataKey, 1);

    key->ref_count = 1;

    key->size = size;
    key->init = init;
    key->fini = fini;
    key->update = update;

    key->bd_list = NULL;

    return key;
}

RenVectorBackDataKey*
ren_vector_back_data_key_ref (RenVectorBackDataKey *key)
{
    ++(key->ref_count);
    return key;
}

void
ren_vector_back_data_key_unref (RenVectorBackDataKey *key)
{
    if (--(key->ref_count) > 0)
        return;

    _REN_KEY_BACK_DATA_LIST_CLEAR (Vector,vector,key,vector,
        if (key->fini != NULL)
            key->fini (vector, data);
        g_free (data);
    );

    g_free (key);
}

RenVectorBackData*
ren_vector_back_data (RenVector *vector, RenVectorBackDataKey *key)
{
    _REN_BACK_DATA_GET_OR_NEW (Vector,vector,vector,key,
        if (key->init != NULL)
            key->init (vector, data);
        if (key->update != NULL)
            item->changed = TRUE;
    );
    if (key->update != NULL && item->changed)
    {
        key->update (vector, data);
        item->changed = FALSE;
    }
    return data;
}
