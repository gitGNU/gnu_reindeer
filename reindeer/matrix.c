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

#include <ren/matrix.h>
#include <ren/base.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenMatrixBackDataItem _RenMatrixBackDataItem;

struct _RenMatrix
{
    ren_uint32 ref_count;

    _RenMatrixBackDataItem *bd_list;
    ren_uint32 change;

    void *data;
    ren_size width;
    ren_size height;
    RenType type;
    ren_bool transposed;
};

struct _RenMatrixBackDataKey
{
    ren_uint32 ref_count;

    _RenMatrixBackDataItem *bd_list;

    ren_size data_size;
    void* user_data;
    RenMatrixBackDataInitFunc init;
    RenMatrixBackDataFiniFunc fini;
    RenMatrixBackDataUpdateFunc update;
};

struct _RenMatrixBackDataItem
{
    _RenBackDataItem base;
    ren_uint32 change;
};

RenMatrix*
ren_matrix_new (ren_size width, ren_size height,
    RenType type, ren_bool transposed)
{
    if (type != REN_TYPE_SFLOAT && type != REN_TYPE_DFLOAT)
        return NULL;

    RenMatrix *matrix = g_new (RenMatrix, 1);

    matrix->ref_count = 1;

    matrix->bd_list = NULL;
    matrix->change = 1;

    matrix->data = g_malloc (ren_type_sizeof (type) * width * height);
    matrix->width = width;
    matrix->height = height;
    matrix->type = type;
    matrix->transposed = transposed;

    return matrix;
}

RenMatrix*
ren_matrix_ref (RenMatrix *matrix)
{
    ++(matrix->ref_count);
    return matrix;
}

void
ren_matrix_unref (RenMatrix *matrix)
{
    if (--(matrix->ref_count) > 0)
        return;

    _REN_RES_BACK_DATA_LIST_CLEAR (Matrix, matrix,
        matrix, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

    g_free (matrix->data);
    g_free (matrix);
}

void*
ren_matrix_begin_edit (RenMatrix *matrix)
{
    return matrix->data;
}

void
ren_matrix_end_edit (RenMatrix *matrix)
{
    _REN_BACK_DATA_SIMPLE_CHANGED (Matrix, matrix, matrix);
}

void
ren_matrix_data (RenMatrix *matrix,
    const void **data_p, ren_size *width_p, ren_size *height_p,
    RenType *type_p, ren_bool *transposed_p)
{
    if (data_p)
        (*data_p) = matrix->data;
    if (width_p)
        (*width_p) = matrix->width;
    if (height_p)
        (*height_p) = matrix->height;
    if (type_p)
        (*type_p) = matrix->type;
    if (transposed_p)
        (*transposed_p) = matrix->transposed;
}

RenMatrixBackDataKey*
ren_matrix_back_data_key_new (ren_size data_size,
    RenMatrixBackDataInitFunc init,
    RenMatrixBackDataFiniFunc fini,
    RenMatrixBackDataUpdateFunc update)
{
    RenMatrixBackDataKey *key = g_new (RenMatrixBackDataKey, 1);

    key->ref_count = 1;

    key->bd_list = NULL;

    key->data_size = data_size;
    key->user_data = NULL;
    key->init = init;
    key->fini = fini;
    key->update = update;

    return key;
}

void
ren_matrix_back_data_key_user_data (RenMatrixBackDataKey *key,
    void *user_data)
{
    key->user_data = user_data;
}

RenMatrixBackDataKey*
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key)
{
    ++(key->ref_count);
    return key;
}

void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key)
{
    if (--(key->ref_count) > 0)
        return;

    _REN_KEY_BACK_DATA_LIST_CLEAR (Matrix, matrix,
        key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

    g_free (key);
}

RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key)
{
    _REN_BACK_DATA_RETURN (Matrix, matrix,
        matrix, key,
        _REN_BACK_DATA_SIMPLE_INIT_FUNC,
        _REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
