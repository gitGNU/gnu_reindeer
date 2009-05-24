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

struct _RenMatrix
{
    ren_uint32 ref_count;

    const void *data;
    ren_size width;
    ren_size height;
    RenType type;
    ren_bool transposed;

    GList *context_updater_list;
    GList *backend_updater_list;
};

typedef struct _RenMatrixContextUpdater _RenMatrixContextUpdater;
struct _RenMatrixContextUpdater
{
    _RenContextUpdater base;
    RenMatrixContextData *context_data;
    ren_bool changed;
};

typedef struct _RenMatrixBackendUpdater _RenMatrixBackendUpdater;
struct _RenMatrixBackendUpdater
{
    _RenBackendUpdater base;
    RenMatrixBackendData *backend_data;
    ren_bool changed;
};

static void
remove_context_updater (RenMatrix *matrix, GList *updater_item);
static void
remove_backend_updater (RenMatrix *matrix, GList *updater_item);

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

    matrix->context_updater_list = NULL;
    matrix->backend_updater_list = NULL;

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

    CLEAR_UPDATER_LISTS (Matrix,matrix,matrix);

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

RenMatrixContextData*
ren_matrix_context_data (RenMatrix *matrix, RenReindeer *r)
{
    RETURN_CONTEXT_DATA(Matrix,matrix,matrix,
        context_updater->changed = TRUE;
        ,
        if (context_updater->changed)
        {
            context_data_update (matrix, context_data);
            context_updater->changed = FALSE;
        }
    );
}

RenMatrixBackendData*
ren_matrix_backend_data (RenMatrix *matrix, RenReindeer *r)
{
    RETURN_BACKEND_DATA(Matrix,matrix,matrix,
        backend_updater->changed = TRUE;
        ,
        if (backend_updater->changed)
        {
            backend_data_update (matrix, backend_data);
            backend_updater->changed = FALSE;
        }
    );
}

static void
remove_context_updater (RenMatrix *matrix, GList *updater_item)
{
    REMOVE_CONTEXT_UPDATER(Matrix,matrix,matrix,{});
}

static void
remove_backend_updater (RenMatrix *matrix, GList *updater_item)
{
    REMOVE_BACKEND_UPDATER(Matrix,matrix,matrix,{});
}
