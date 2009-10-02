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

typedef struct _RenMatrixBackDataItem _RenMatrixBackDataItem;

struct _RenMatrix
{
    ren_uint32 ref_count;

    const void *data;
    ren_size width;
    ren_size height;
    RenType type;
    ren_bool transposed;

    _RenMatrixBackDataItem *bd_list;
};

struct _RenMatrixBackDataKey
{
    ren_uint32 ref_count;

    ren_size size;
    RenMatrixBackDataInitFunc init;
    RenMatrixBackDataFiniFunc fini;
    RenMatrixBackDataUpdateFunc update;

    _RenMatrixBackDataItem *bd_list;
};

struct _RenMatrixBackDataItem
{
    _RenBackDataItem base;
    ren_bool changed;
};

RenMatrix*
ren_matrix_new (const void *data, ren_size width, ren_size height,
    RenType type, ren_bool transposed)
{
    if (type != REN_TYPE_SFLOAT && type != REN_TYPE_DFLOAT)
        return NULL;

    RenMatrix *matrix = g_new (RenMatrix, 1);

    matrix->ref_count = 1;

    matrix->data = data;
    matrix->width = width;
    matrix->height = height;
    matrix->type = type;
    matrix->transposed = transposed;

    matrix->bd_list = NULL;

    return matrix;
}

void
ren_matrix_destroy (RenMatrix *matrix)
{
    ren_matrix_unref (matrix);
}

void
ren_matrix_changed (RenMatrix *matrix)
{
    _REN_RES_BACK_DATA_LIST_UPDATE (Matrix,matrix,matrix,
        if (key->update != NULL)
            item->changed = TRUE;
    );
}

void
ren_matrix_ref (RenMatrix *matrix)
{
    ++(matrix->ref_count);
}

void
ren_matrix_unref (RenMatrix *matrix)
{
    if (--(matrix->ref_count) > 0)
        return;

    _REN_RES_BACK_DATA_LIST_CLEAR (Matrix,matrix,matrix,
        if (key->fini != NULL)
            key->fini (matrix, data);
        g_free (data);
    );

    g_free (matrix);
}

void
ren_matrix_data (RenMatrix *matrix,
    const void **datap, ren_size *widthp, ren_size *heightp,
    RenType *typep, ren_bool *transposedp)
{
    if (datap)
        (*datap) = matrix->data;
    if (widthp)
        (*widthp) = matrix->width;
    if (heightp)
        (*heightp) = matrix->height;
    if (typep)
        (*typep) = matrix->type;
    if (transposedp)
        (*transposedp) = matrix->transposed;
}

RenMatrixBackDataKey*
ren_matrix_back_data_key_new (ren_size size, RenMatrixBackDataInitFunc init,
    RenMatrixBackDataFiniFunc fini, RenMatrixBackDataUpdateFunc update)
{
    RenMatrixBackDataKey *key = g_new (RenMatrixBackDataKey, 1);

    key->ref_count = 1;

    key->size = size;
    key->init = init;
    key->fini = fini;
    key->update = update;

    key->bd_list = NULL;

    return key;
}

void
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key)
{
    ++(key->ref_count);
}

void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key)
{
    if (--(key->ref_count) > 0)
        return;

    _REN_KEY_BACK_DATA_LIST_CLEAR (Matrix,matrix,key,matrix,
        if (key->fini != NULL)
            key->fini (matrix, data);
        g_free (data);
    );

    g_free (key);
}

RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key)
{
    _REN_BACK_DATA_GET_OR_NEW (Matrix,matrix,matrix,key,
        if (key->init != NULL)
            key->init (matrix, data);
        if (key->update != NULL)
            item->changed = TRUE;
    );
    if (key->update != NULL && item->changed)
    {
        key->update (matrix, data);
        item->changed = FALSE;
    }
    return data;
}
